#define Y_LED 0   // หลอดแอลอีดีสีเหลืองต่อกับ GPIO0
#define R_LED 15  // หลอดแอลอีดีสีแดงต่อกับ GPIO15
#define G_LED 2   // หลอดแอลอีดีสีเขียวต่อกับ GPIO2
#define SW1 4     // สวิตช์ต่อกับ GPIO4

/*************************************************************
 This is a simple demo of sending and receiving some data.
 Be sure to check out other examples!
 *************************************************************/
#define BLYNK_TEMPLATE_ID "TMPL66eimJyl_"  // ID ของ Template ที่ใช้งานใน Blynk
#define BLYNK_TEMPLATE_NAME "Checkpoint2"  // ชื่อของ Template ที่ใช้งานใน Blynk
#define BLYNK_AUTH_TOKEN "_5oVydeqDvF09sjKhvM9uLBpF0-NsOmu"  // โทเค็นสำหรับเชื่อมต่อกับ Blynk

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial  // ใช้พิมพ์ข้อความผ่าน Serial Monitor
#include <ESP8266WiFi.h>  // ไฟล์เฮดเดอร์สำหรับเชื่อมต่อ WiFi
#include <BlynkSimpleEsp8266.h>  // ไฟล์เฮดเดอร์สำหรับเชื่อมต่อ Blynk ผ่าน ESP8266

// ข้อมูล Wi-Fi ที่ใช้งาน
char ssid[] = "iPhone ของ sola";  // ชื่อ Wi-Fi
char pass[] = "////////";  // รหัสผ่าน Wi-Fi
BlynkTimer timer;  // ตัวจับเวลา Blynk

// ฟังก์ชันนี้ถูกเรียกเมื่อสถานะของ Virtual Pin 0 เปลี่ยนแปลง
BLYNK_WRITE(V0)
{
  int value = param.asInt();  // รับค่าจาก Virtual Pin V0 และเก็บไว้ในตัวแปร value
  if (value == 1)  // ถ้าค่าเป็น 1 (เปิด)
    digitalWrite(R_LED, HIGH);  // เปิด LED สีแดง
  else  // ถ้าค่าเป็น 0 (ปิด)
    digitalWrite(R_LED, LOW);  // ปิด LED สีแดง
}

// ฟังก์ชันนี้ถูกเรียกเมื่อสถานะของ Virtual Pin 1 เปลี่ยนแปลง
BLYNK_WRITE(V1)
{
  int value = param.asInt();  // รับค่าจาก Virtual Pin V1 และเก็บไว้ในตัวแปร value
  analogWrite(G_LED, value);  // ปรับความสว่างของ LED สีเขียวตามค่า value
}

// ฟังก์ชันนี้ถูกเรียกเมื่ออุปกรณ์เชื่อมต่อกับ Blynk Cloud สำเร็จ
BLYNK_CONNECTED() {
  // เปลี่ยนภาพปุ่มในแอป Blynk เมื่อเชื่อมต่อสำเร็จ
  Blynk.setProperty(V1, "offImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V1, "onImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V100, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstartdevice-was-made");
}


// ฟังก์ชันนี้ส่งค่าจากสวิตช์ SW1 ไปยัง Virtual Pin V3 ทุกๆ วินาที
void ReadSW()
{
  uint8_t d = digitalRead(SW1);  // อ่านค่าจากสวิตช์ SW1
  Blynk.virtualWrite(V3, d);  // ส่งค่าจากสวิตช์ไปยัง Virtual Pin V3
  digitalWrite(Y_LED, d);
}

void setup()
{
  pinMode(Y_LED, OUTPUT);  // ตั้งค่า GPIO0 เป็นเอาต์พุต (สำหรับ LED สีเหลือง)
  pinMode(G_LED, OUTPUT);  // ตั้งค่า GPIO2 เป็นเอาต์พุต (สำหรับ LED สีเขียว)
  pinMode(R_LED, OUTPUT);  // ตั้งค่า GPIO15 เป็นเอาต์พุต (สำหรับ LED สีแดง)
  pinMode(SW1, INPUT);  // ตั้งค่า GPIO4 เป็นอินพุต (สำหรับสวิตช์ SW1)
  Serial.begin(115200);  // เริ่มการสื่อสารผ่าน Serial ที่ความเร็ว 115200 bps
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  // เริ่มการเชื่อมต่อกับ Blynk
  timer.setInterval(100L, ReadSW);  // เรียกใช้ฟังก์ชัน ReadSW ทุกๆ 100 มิลลิวินาที
}

void loop()
{
  Blynk.run();  // จัดการการเชื่อมต่อกับ Blynk
  timer.run();  // เรียกใช้ฟังก์ชันตามช่วงเวลาที่ตั้งไว้
}
