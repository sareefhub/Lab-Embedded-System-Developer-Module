#include "my_EEPROM.h"

#define EEPROM_SIZE 1024

uint16_t scan_EEPROM(void) {
  int data_location = -1;
  int number_data_found = 0;

  Serial.println("Performing EEPROM scanning...");

  // สแกน EEPROM เพื่อหาข้อมูลที่ไม่ใช่ 0xFF
  for (int i = 0; i < EEPROM_SIZE; i++) {
    uint8_t d = EEPROM_read1byte(i);

    if (d != 0xFF) {
      data_location = i;
      number_data_found++;
      Serial.print(d);
      Serial.print(" is found in EEPROM location[");
      Serial.print(i);
      Serial.println("]");
    }
  }

  if (number_data_found > 1 && number_data_found < 3) {
    Serial.println("More than one data are found in EEPROM");

    // ลบข้อมูลทั้งหมดที่พบ
    for (int i = 0; i < EEPROM_SIZE; i++) {
      if (EEPROM_read1byte(i) != 0xFF) {
        EEPROM_Erase_only(i);  // ใช้ฟังก์ชันลบข้อมูล
      }
    }
  } else if (number_data_found == 0) {
    Serial.println("No data found in EEPROM");

    // เขียนค่า 0xFA, 0xCE, 0x0F ลงในตำแหน่ง 1021-1023
    EEPROM_Erase_and_Write1B(1021, 0xFA);
    EEPROM_Erase_and_Write1B(1022, 0xCE);
    EEPROM_Erase_and_Write1B(1023, 0x0F);
  } else {
    // ตรวจหาค่า 0xFA, 0xCE, 0x0F ที่อยู่ติดกัน
    for (int i = 0; i < EEPROM_SIZE - 2; i++) {
      if (EEPROM_read1byte(i) == 0xFA && EEPROM_read1byte(i + 1) == 0xCE && EEPROM_read1byte(i + 2) == 0x0F) {
        Serial.print("0xFA, 0xCE, 0x0F found in EEPROM at location ");
        Serial.println(i);

        // ลบข้อมูลที่พบ
        EEPROM_Erase_only(i);
        EEPROM_Erase_only(i + 1);
        EEPROM_Erase_only(i + 2);

        // เขียนข้อมูลใหม่ในตำแหน่งที่ต่ำกว่า (ถ้าตำแหน่งใหม่ไม่เกินขอบเขต EEPROM)
        if (i >= 3) {
          Serial.println("Writing 0xFA, 0xCE, 0x0F to lower addresses...");
          EEPROM_Erase_and_Write1B(i - 3, 0xFA);
          EEPROM_Erase_and_Write1B(i - 2, 0xCE);
          EEPROM_Erase_and_Write1B(i - 1, 0x0F);
        } else {
          Serial.println("No space to write new data.");
        }

        break; // หยุดการตรวจสอบหลังจากพบและลบข้อมูลแล้ว
      }
    }
  }

  return number_data_found;
}

void setup() {
  Serial.begin(38400);
  display_all_data_in_EEPROM(); // แสดงข้อมูลใน EEPROM ทั้งหมด
  scan_EEPROM();
}

void loop() {
  // ไม่มีการทำงานใน loop
}