#include "my_EEPROM.h"
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>

#define I2CADDR 0x20

static int start_address;
const int address_marker = 1;

const int strobe = 4;
const int clock = 3;
const int data = 2;
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {7, 6, 5, 4};
byte colPins[COLS] = {3, 2, 1, 0};
Keypad_I2C keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);

int dataToWrite[2]; // Array to store up to 2 digits of data
int dataIndex = 0;  // Index to track which digit we are currently entering
bool save_done = false; // Flag to track if save operation has been completed

void I2C_bus_scan(void);
void sendCommand(uint8_t value);
void reset();
uint8_t readTM1638_Buttons(void);
void setTM1638LED(uint8_t value, uint8_t position);

void save_start_address_to_EEPROM(int addr) {
  EEPROM_Erase_and_Write1B(address_marker, addr >> 8); // Save MSB
  EEPROM_Erase_and_Write1B(address_marker + 1, addr & 0xFF); // Save LSB
}

int read_start_address_from_EEPROM() {
  int addr = (EEPROM_read1byte(address_marker) << 😎 | EEPROM_read1byte(address_marker + 1);
  return addr;
}

void setup() {
  Serial.begin(38400);
  start_address = read_start_address_from_EEPROM();
  if (start_address < 0 || start_address >= 1024) {
    start_address = 1; 
  }

  display_all_data_in_EEPROM();
  scan_EEPROM();
  save_start_address_to_EEPROM(start_address);

  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);

  Wire.begin();
  keypad.begin();
  I2C_bus_scan(); // Scan I2C devices
  sendCommand(0x8f);  // Activate TM1638
  reset(); // Reset TM1638
}

void loop() {
  char key = keypad.getKey();
  uint8_t buttons = readTM1638_Buttons();

  if (key != NO_KEY) {
    Serial.print(key);
    Serial.println(" is pressed");

    int value = -1;
    if (key >= '0' && key <= '9') {
      value = key - '0';
    } else if (key == '*') {
      value = 14; // E
    } else if (key == '#') {
      value = 15; // F
    }

    if (value >= 0 && value < 16) {
      dataToWrite[dataIndex] = value; // Store the value in dataToWrite array
      dataIndex++;
      
      if (dataIndex >= 2) {
        dataIndex = 2; // Limit index to 2 to handle 2 digits
        Serial.print("Data to write: ");
        Serial.print(dataToWrite[0], HEX);
        Serial.print(dataToWrite[1], HEX);
        Serial.println();
      }
    }
  }

  if (buttons & 0x01) {  // Check if S1 is pressed
    if (!save_done) { // Proceed only if not yet saved
      if (dataIndex > 0) { // Ensure at least one digit is entered
        if (dataIndex == 1) {
          EEPROM_Erase_and_Write1B(start_address, dataToWrite[0]);
          Serial.print("Saved ");
          Serial.print(dataToWrite[0], HEX);
          Serial.print(" to EEPROM at address ");
          Serial.println(start_address);
        } else if (dataIndex == 2) {
          EEPROM_Erase_and_Write1B(start_address, (dataToWrite[0] << 4) | dataToWrite[1]);
          Serial.print("Saved ");
          Serial.print(dataToWrite[0], HEX);
          Serial.print(dataToWrite[1], HEX);
          Serial.print(" to EEPROM at address ");
          Serial.println(start_address);
        }

        start_address++;
        save_start_address_to_EEPROM(start_address);

        int eeprom_value = EEPROM_read1byte(start_address - 1);
        Serial.print("Read from EEPROM: ");
        Serial.println(eeprom_value, HEX);

        save_done = true; // Mark save operation as done
        dataIndex = 0; // Reset index after saving
      } else {
        Serial.println("Error: No data to save.");
      }
    }
  } else {
    save_done = false; // Reset save_done flag when button is not pressed
  }

  for (uint8_t position = 0; position < 8; position++) {
    uint8_t mask = 0x1 << position;
    setTM1638LED((buttons & mask) ? 1 : 0, position);
  }
}

void I2C_bus_scan(void) {
  byte count = 0;
  Wire.begin();
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found address: ");
      Serial.print(i, DEC);
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.println(")");
      count++;
      delay(1);
      for (uint8_t position = 0; position < 8; position++) {
        setTM1638LED(1, position);
      }
      delay(1000);
    }
  }
  Serial.println("Done.");
  Serial.print("Found ");
  Serial.print(count);
  Serial.println(" devices.");
}

void sendCommand(uint8_t value) {
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

void reset() {
  sendCommand(0x40); // Set auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0); // Set starting address to 0
  for (uint8_t i = 0; i < 16; i++) {
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
}

uint8_t readTM1638_Buttons(void) {
  uint8_t buttons = 0;
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0x42);

  pinMode(data, INPUT);
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t v = shiftIn(data, clock, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(data, OUTPUT);
  digitalWrite(strobe, HIGH);
  return buttons;
}

void setTM1638LED(uint8_t value, uint8_t position) {
  pinMode(data, OUTPUT);
  
  sendCommand(0x44);
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC1 + (position << 1));
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

uint16_t scan_EEPROM(void) {
  int i, data_location, number_data_found;
  uint8_t d;
  number_data_found = 0;
  Serial.println("Performing EEPROM scanning...");
  
  for (i = 0; i < 1024; i++) {
    d = EEPROM_read1byte(i);
    if (d != 0xFF) {
      data_location = i;
      number_data_found++;
      Serial.print(d, HEX);
      Serial.print(" is found in EEPROM location[");
      Serial.print(i);
      Serial.println("]");
    }
  }

  if (number_data_found > 1) {
    Serial.println("More than one data are found in EEPROM");
  } else if (number_data_found == 0) {
    Serial.println("No data found in EEPROM");
  }
  
  return number_data_found; // Return the number of data found
}