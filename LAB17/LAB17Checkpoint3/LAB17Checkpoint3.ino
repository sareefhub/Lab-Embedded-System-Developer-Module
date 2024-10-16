#include <TM1638.h>
#include <Keypad.h>
#include "my_EEPROM.h"

#define TM1638_DIO 2
#define TM1638_CLK 3
#define TM1638_STB 4

TM1638 module(TM1638_DIO, TM1638_CLK, TM1638_STB);

const byte ROWS = 4; // 4 แถวของคีย์แพ็ด
const byte COLS = 4; // 4 คอลัมน์ของคีย์แพ็ด

// การกำหนดพินของคีย์แพ็ด
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};   // พินแถวของคีย์แพ็ด
byte colPins[COLS] = {5, 4, 3, 2};   // พินคอลัมน์ของคีย์แพ็ด

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

enum State {ST1, ST2, ST3, ST4, ST5, ST6};
State systemState = ST1;

uint16_t eepromAddress = 0; // ที่อยู่ใน EEPROM
uint8_t dataToWrite = 0; // ข้อมูลที่จะเขียนลงใน EEPROM
bool addressInputComplete = false; // สถานะการป้อนที่อยู่
bool dataInputComplete = false; // สถานะการป้อนข้อมูล
uint8_t pageData[16]; // ข้อมูลที่จะเขียนเป็น page
uint16_t numToDelete = 0; // จำนวนที่จะลบใน ST5

void setup() {
  Serial.begin(38400);
  module.clearDisplay(); // ดับแอลอีดี 7 ส่วนทั้งหมด
  Serial.println("System in ST1");
}

void loop() {
  char key = keypad.getKey();

  switch (systemState) {
    case ST1:
      module.clearDisplay();
      if (module.getButtons() & 0x80) { // กดปุ่ม S8
        systemState = ST2;
        Serial.println("Entering ST2: Input EEPROM Address");
      }
      if (module.getButtons() & 0x01) { // กดปุ่ม S1 เพื่อเข้าสู่สถานะ ST3
        systemState = ST3;
        Serial.println("Entering ST3: Write to EEPROM page");
      }
      if (module.getButtons() & 0x02) { // กดปุ่ม S2 เพื่อเข้าสู่สถานะ ST4
        systemState = ST4;
        Serial.println("Entering ST4: Multi-write to EEPROM");
      }
      if (module.getButtons() & 0x04) { // กดปุ่ม S3 เพื่อเข้าสู่สถานะ ST5
        systemState = ST5;
        Serial.println("Entering ST5: Erase EEPROM");
      }
      break;

    case ST2: // สถานะที่เขียนค่าในหน่วยความจำที่ระบุ
      if (!addressInputComplete) {
        blinkAddressInput();
        if (key) {
          if (isdigit(key) && eepromAddress < 10000) {
            eepromAddress = eepromAddress * 10 + (key - '0');
            Serial.print("Address Input: ");
            Serial.println(eepromAddress);
          }
        }
        if (module.getButtons() & 0x80) { // กดปุ่ม S8 เพื่อยืนยันที่อยู่
          addressInputComplete = true;
        }
      } else {
        blinkDataInput();
        if (key) {
          dataToWrite = processHexInput(key);
          Serial.print("Data Input: ");
          Serial.println(dataToWrite, HEX);
        }
        if (module.getButtons() & 0x01) { // กดปุ่ม S1 เพื่อเขียนข้อมูลลงใน EEPROM
          EEPROM_Erase_and_Write1B(eepromAddress, dataToWrite);
          systemState = ST1;
          Serial.println("Data written to EEPROM");
        }
      }
      break;

    case ST3: // สถานะเขียนข้อมูลครั้งละ 1 page (16 ตำแหน่ง)
      Serial.println("Enter EEPROM Address to start writing a page");
      addressInputComplete = false;
      if (!addressInputComplete) {
        blinkAddressInput();
        if (key) {
          if (isdigit(key) && eepromAddress < 10000) {
            eepromAddress = eepromAddress * 10 + (key - '0');
            Serial.print("Page Address Input: ");
            Serial.println(eepromAddress);
          }
        }
        if (module.getButtons() & 0x80) { // กดปุ่ม S8 เพื่อยืนยันที่อยู่
          addressInputComplete = true;
          systemState = ST1;
        }
      } else {
        Serial.println("Enter data to write to page");
        for (int i = 0; i < 16; i++) {
          blinkDataInput();
          if (key) {
            pageData[i] = processHexInput(key);
            Serial.print("Page Data Input: ");
            Serial.println(pageData[i], HEX);
          }
          if (module.getButtons() & 0x01) { // กดปุ่ม S1 เพื่อเขียนข้อมูลลงใน page
            for (int i = 0; i < 16; i++) {
              EEPROM_Erase_and_Write1B(eepromAddress + i, pageData[i]);
            }
            systemState = ST1;
            Serial.println("Page data written to EEPROM");
          }
        }
      }
      break;

    case ST4: // สถานะเขียนค่าหลายตำแหน่ง
      Serial.println("Multi-write to EEPROM mode");
      addressInputComplete = false;
      if (!addressInputComplete) {
        blinkAddressInput();
        if (key) {
          if (isdigit(key) && eepromAddress < 10000) {
            eepromAddress = eepromAddress * 10 + (key - '0');
            Serial.print("Starting Address Input: ");
            Serial.println(eepromAddress);
          }
        }
        if (module.getButtons() & 0x80) { // กดปุ่ม S8 เพื่อยืนยันที่อยู่
          addressInputComplete = true;
        }
      } else {
        blinkDataInput();
        if (key) {
          dataToWrite = processHexInput(key);
          EEPROM_Erase_and_Write1B(eepromAddress, dataToWrite);
          Serial.print("Data written to address ");
          Serial.print(eepromAddress);
          Serial.print(": ");
          Serial.println(dataToWrite, HEX);
          eepromAddress++; // เขียนในตำแหน่งถัดไป
        }
        if (module.getButtons() & 0x40) { // กดปุ่ม S6 เพื่อออกจากโหมด ST4
          systemState = ST1;
          Serial.println("Exited Multi-write mode");
        }
      }
      break;

    case ST5: // สถานะลบข้อมูล
      Serial.println("Erase EEPROM mode");
      blinkAddressInput();
      if (key) {
        if (isdigit(key) && eepromAddress < 10000) {
          eepromAddress = eepromAddress * 10 + (key - '0');
          Serial.print("Erase Start Address Input: ");
          Serial.println(eepromAddress);
        } else if (isdigit(key)) {
          numToDelete = numToDelete * 10 + (key - '0');
          Serial.print("Number of Bytes to Erase: ");
          Serial.println(numToDelete);
        }
      }
      if (module.getButtons() & 0x80) { // กดปุ่ม S8 เพื่อเริ่มลบข้อมูล
        for (uint16_t i = 0; i < numToDelete; i++) {
          EEPROM_Erase_only(eepromAddress + i);
        }
        systemState = ST1;
        Serial.println("Data erased from EEPROM");
      }
      break;

    case ST6:
      // สถานะ ST6 สำหรับโหมดอื่น ๆ ที่อาจเพิ่มเติม
      break;
  }
}

void blinkAddressInput() {
  static bool blink = false;
  static unsigned long lastBlinkTime = 0;
  if (millis() - lastBlinkTime > 500) {
    lastBlinkTime = millis();
    blink = !blink;
    if (blink) {
      module.setDisplayToDecNumber(eepromAddress, true, 4, 0);
    } else {
      module.clearDisplayDigit(0, 4);
    }
  }
}

void blinkDataInput() {
  static bool blink = false;
  static unsigned long lastBlinkTime = 0;
  if (millis() - lastBlinkTime > 500) {
    lastBlinkTime = millis();
    blink = !blink;
    if (blink) {
      module.setDisplayToHexNumber(dataToWrite, true, 2, 6);
    } else {
      module.clearDisplayDigit(6, 2);
    }
  }
}

uint8_t processHexInput(char key) {
  if (key >= '0' && key <= '9') {
    return key - '0';
  } else if (key == '*') {
    return 0xE; // ปุ่ม * แทนค่า E
  } else if (key == '#') {
    return 0xF; // ปุ่ม # แทนค่า F
  } else {
    return 0;
  }
}

void displayEEPROMData() {
  for (uint16_t i = 0; i < EEPROM_SIZE; i++) {
    uint8_t data = EEPROM_read1byte(i);
    Serial.print("EEPROM[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(data, HEX);
  }
}