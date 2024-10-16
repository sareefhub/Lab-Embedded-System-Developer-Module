#include<LiquidCrystal.h>
#include<DHT.h>            //เรียกใช้คลังโปรแกรม DHT.h
#define DHTPIN 2          //กรณีมีการต่อสัญญาณของเซนเซอร์ DHT11 เข้ากับขา 2
#define DHTTYPE DHT11     //DHT 11
DHT dht(DHTPIN, DHT11);   //สั่งให้ใช้มอดูล DHT11 กับขา 2 ของบอร์ด Arduino
LiquidCrystal lcd1(8, 9, 10, 11, 12, 13); //RS,E,D4,D5,D6,D7
void setup() {
  // put your setup code here, to run once:
  lcd1.begin(16, 2); //การใช้งาน LCD แบบหน้าจอ 2 บรรทัด แต่ละบรรทัดมีความยาว 16 ตัวอักษร
  lcd1.clear();      //คำสั่งให้ลบหน้าจอทั้งหมด
  dht.begin();      //สั่งให้ DHT11 ทำงาน
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  lcd1.clear();
  float h = dht.readHumidity();     //อ่านค่าความชื้นสัมพัทธ์ใส่ตัวแปร h
  float t = dht.readTemperature();  //อ่านค่าอุณหภูมิในหน่วยองศาเซลเซียสใส่ตัวแปร t
  lcd1.setCursor(0, 0); 
  lcd1.print("Humidity:");
  lcd1.print(h);                    //แสดง output ของค่าความชื้นสัมพัทธ์
  lcd1.print(" %");                 //หน่วยแสดงค่าความชื้นสัมพัทธ์
  lcd1.setCursor(0, 1); 
  lcd1.print("Temp:");
  lcd1.print(t);                    //แสดง output ของอุณหภูมิ
  lcd1.print((char)223);            //ใช้คำสั่ง char เพื่อให้มีเครื่องหมายหน้าหน่วยองศาเซลเซียส
  lcd1.print("C");                  //หน่วยองศาเซลเซียส
}
