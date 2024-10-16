#include <avr/sleep.h>    //เรียกใช้คลังโปรแกรมสำหรับเรียกฟังก์ชัน sleep_cpu
#define INT0_PIN 2        //กำหนด PIN2 สำหรับการขัดจังหวะ INTO0
#define INT1_PIN 3        //กำหนด PIN3 สำหรับการขัดจังหวะ INTO1
const int strobe = 14;    //กำหนด PIN14 ใช้ส่ง strobe ไปยัง TM1638
const int clock = 15;     //กำหนด PIN15 ใช้ส่ง clock ไปยัง TM1638
const int data = 16;      //กำหนด PIN16 ใช้ส่ง data ไปยัง TM1638
uint8_t sec, subsec;      //ประกาศตัวแปร sec, subsec เก็บค่าวินาทีและ 1/100 วินาที
uint8_t mode;             //ประกาศตัวแปร mode เพื่อเปลี่ยนสถานะตัวแปร startCount (Ture/False)
bool startCount = false;  //ประกาศตัวแปร startCount สั่งเริ่มนับถอยกลัง
unsigned long startTime, prevTime;  //ประกาศตัวแปร startTime, prevTime ใช้นับถอยหลัง

void SLEEP_DISABLE()      //ฟังก์ชันสำหรับปิดทางไม่ให้ตัวประมวลผลเข้าสู่ภาวะหลับ
{                         //เริ่มฟังก์ชัน SLEEP_DISABLE
  SMCR &= 0xFE;           //สั่งให้บิต SE(บิตล่างสุด) ในเรจิสเตอร์ SMCR มีค่าตรรกะต่ำ
}                         //สิ้นสุดฟังก์ชัน SLEEP_DISABLE

void SLEEP_INITIALIZE(uint8_t m)    //ฟังก์ชันสำหรับเปิดทางให้ CPU สามารถเข้าสู่สภาวะหลับได้
{                                   //--และรับพารามิเตอร์ระบุค่าหมายเลขแบบวิธีของภาวะหลับ
  SMCR = (m << 1) | 0x01;           //ตั้งค่าแบบวิธีภาวะหลับ และตั้งให้บิต SE มีค่าตรรกะสูง
}                                   //สิ้นสุดฟังก์ชัน SLEEP_INITIALIZE

uint8_t readTM1638_Buttons(void)    
//ฟังก์ชันรีเทิร์นค่าใช้สำหรับการอ่านค่าการกดปุ่มบน TM1638 แล้วส่งค่าคืนกลับมายังบอร์ด Arduino
{
  uint8_t buttons = 0;  //เริ่มต้นกำหนดให้ buttons เท่ากับ 0  
  digitalWrite(strobe, LOW);  //สั่งขา strobe มีค่าเป็นตรรกะต่ำ
  shiftOut(data, clock, LSBFIRST, 0x42);

  pinMode(data, INPUT);

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(data, clock, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(data, OUTPUT);
  digitalWrite(strobe, HIGH); //สั่งขา strobe มีค่าเป็นตรรกะสูง
  return buttons;
}

void sendCommand(uint8_t value) 
//ฟังก์ชันที่ใช้ในการส่งคำสั่งและข้อมูลจากบอร์ด Arduino ไปยัง TM1638
{
  digitalWrite(strobe, LOW);  //สั่งขา strobe มีค่าเป็นตรรกะต่ำ
  shiftOut(data, clock, LSBFIRST, value); //กำหนดคำสั่งและข้อมูลที่จะส่ง
  digitalWrite(strobe, HIGH); //สั่งขา strobe มีค่าเป็นตรรกะสูงเพื่อเริ่มต้นส่งข้อมูล
}


void reset() 
//ฟังก์ชันรีเช็ตการรับ-ส่งข้อมูลระหว่างงบอร์ด Arduino และ TM1638
//--เป็นการสั่งให้ LED และ 7-segments หยุดการทำงาน
{
  sendCommand(0x40); // set auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(strobe, OUTPUT);  //กำหนดทิศทาง PINPORT สำหรับ strobe เป็น OUTPUT
  pinMode(clock, OUTPUT);   //กำหนดทิศทาง PINPORT สำหรับ clock เป็น OUTPUT
  pinMode(data, OUTPUT);    //กำหนดทิศทาง PINPORT สำหรับ data เป็น OUTPUT
  startTime = millis();     //ประกาศตัวแปร startTime = millis เริ่มนับเวลาการทำงาน
  sec = 12;                 //กำหนด sec เท่ากับ 12 
  subsec = 0;               //กำหนด subsec เท่ากับ 0
  sendCommand(0x8f);        //activate เริ่มการทำงานของ TM1638
  reset();                  //reset ค่าบนบอร์ด TM1638
  display_7seg();           //7-segments เริ่มทำงาน
}

void loop() {
  if((millis() - startTime) > 5000 && !startCount){
  //ตรวจสอบว่าไม่มีการกด S8 นานเกิน 5s หรือไม่
    reset(); //หากกด S8 เกิน 5s จะ reset ค่าบอร์ด TM1638
    SLEEP_INITIALIZE(2);  //ตั้งค่าการทำงานในภาวะหลับแบบเดินเครื่องเปล่า
    sleep_cpu();          //สั่งให้ตัวประมวลผลเข้าสู่ภาวะหลับ
  }
  //หากกด S8 ภายในเวลา 5s 
  mode = readTM1638_Buttons();  //มีการอ่านค่าสวิตช์และนำค่ามาเก็บในตัวแปร mode
  Serial.println(mode);         
  if(mode == 0x80){ //หากสวิตช์ที่กดเป็น S8
    startCount = true;  //กำหนดค่าตัวแปร startCount เท่ากับ true
  }

  if(startCount){ //ตรวจสอบว่า startCount เท่ากับ true หรือไม่
    if((millis() - prevTime) > 10){ //หากพบว่าใช่ จะมีการกดสวิตช์ S8 ภายใน 5s
      prevTime = millis();  //เริ่มนับถอยหลังโดยการลดค่าวินาทีทุกๆ 1/100 วินาที
      if(sec == 0){ //ตรวจสอบว่า sec มีค่าเท่ากับ 0 หรือไม่
        subsec = 0; 
        //ตรวจสอบพบว่า sec มีค่าเท่ากับ 0 
        //--จะมีการนับถอยหลังจนถึงค่า 0
        //--และกระพริบเลข 00-00 จำนวน 2 ครั้ง
        display_7seg();
        delay(500);
        reset();
        delay(500);
        display_7seg();
        delay(500);
        reset();
        delay(500);
        display_7seg();
        delay(500);
        reset();
        SLEEP_INITIALIZE(2);  //ตั้งค่าการทำงานในภาวะหลับแบบเดินเครื่องเปล่า
        sleep_cpu();          //สั่งให้ตัวประมวลผลเข้าสู่ภาวะหลับ
      }else{  //ตรวจสอบพบว่า sec มีค่าไม่เท่ากับ 0
        (subsec > 0) ? subsec-- : (subsec = 99, sec--); //นับถอยหลังต่อไป
        display_7seg(); //OUTPUT แสดงผลบน 7-segments
      }
    }
  }
  
}

void display_7seg() //ฟังก์ชันการแสดงผล OUTPUT บน 7-segments
{
  //กำหนดการติด-ดับของ 7-segments
   uint8_t ch[] = {0b01011110, 0b01010100};
   uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };
   sendCommand(0x44); // set single address

  //ตัวอักษร d และ n
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc0);
   shiftOut(data, clock, LSBFIRST, startCount ? 0 : ch[0]);
   digitalWrite(strobe, HIGH);
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc2);
   shiftOut(data, clock, LSBFIRST, startCount ? 0 : ch[1]);
   digitalWrite(strobe, HIGH);

   //Sec
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc6);
   shiftOut(data, clock, LSBFIRST, digits[sec/10]);
   digitalWrite(strobe, HIGH);
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc8);
   shiftOut(data, clock, LSBFIRST, digits[sec%10]);
   digitalWrite(strobe, HIGH);

   //Seperate
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xca);
   shiftOut(data, clock, LSBFIRST, 0x40);
   digitalWrite(strobe, HIGH);

   //1/100 Sec (Subsec)
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xcc);
   shiftOut(data, clock, LSBFIRST, digits[subsec/10]);
   digitalWrite(strobe, HIGH);
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xce);
   shiftOut(data, clock, LSBFIRST, digits[subsec%10]);
   digitalWrite(strobe, HIGH);
}


