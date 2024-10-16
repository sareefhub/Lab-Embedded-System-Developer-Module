#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <DS1307RTC.h> 

#define EEPROM_SIZE 4096  // ขนาดของ EEPROM 24C32 คือ 32Kbits = 4KB
#define EEPROM_ADDR 0x50  // ที่อยู่ของ EEPROM

// ข้อมูลการเชื่อมต่อ Blynk
#define BLYNK_TEMPLATE_ID "TMPL6zHD0gKY-"
#define BLYNK_TEMPLATE_NAME "Checkpoint4"
#define BLYNK_AUTH_TOKEN "PN4tNquDq288kGxxVQfoLWJVoo7Pv1k_"

#define BLYNK_PRINT Serial  // ใช้พิมพ์ข้อความผ่าน Serial Monitor
char ssid[] = "Yindee";
char pass[] = "phoorin3799";

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h> 
BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); //Module IIC/I2C Interface บางรุ่นอาจจะใช้ 0x3f

BLYNK_CONNECTED() {
  // เปลี่ยนภาพปุ่มในแอป Blynk เมื่อเชื่อมต่อสำเร็จ
  Blynk.setProperty(V1, "offImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V1, "onImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V100, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstartdevice-was-made");
}

void readEEPROMAndDisplay() {
  char message[9];  // ข้อความที่ยาวไม่เกิน 8 ตัวอักษร + ตัวอักษร null
  for (int i = 0; i < 8; i++) {
    message[i] = EEPROM.read(i);  // อ่านข้อมูลทีละไบต์จาก EEPROM
  }
  message[8] = '\0';  // ทำให้ข้อความเป็นสตริง
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);  // แสดงข้อความบนจอ LCD
}

BLYNK_WRITE(V1) {
  String newMessage = param.asStr();  // รับข้อความจากแอป Blynk
  if (newMessage.length() > 8) {
    newMessage = newMessage.substring(0, 8);  // ตัดข้อความให้เหลือ 8 ตัวอักษร
  }
  for (int i = 0; i < newMessage.length(); i++) {
    EEPROM.write(i, newMessage[i]);  // เขียนข้อความทีละไบต์ลงใน EEPROM
  }
  EEPROM.commit();  // เขียนข้อมูลลงในหน่วยความจำถาวร
  readEEPROMAndDisplay();  // อ่านและแสดงข้อความใหม่
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  EEPROM.begin(EEPROM_SIZE);  // เริ่มต้นใช้งาน EEPROM
  lcd.init();
  lcd.backlight();  // เปิด backlight
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  // เริ่มการเชื่อมต่อกับ Blynk
  readEEPROMAndDisplay();  // อ่านและแสดงข้อความจาก EEPROM เมื่อเริ่มต้น
}

void loop() {
  Blynk.run();  // จัดการการเชื่อมต่อกับ Blynk
  timer.run();  // เรียกใช้ฟังก์ชันตามช่วงเวลาที่ตั้งไว้
}
