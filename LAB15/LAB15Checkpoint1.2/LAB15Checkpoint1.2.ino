#include <avr/sleep.h>  //เรียกใช้คลังโปรแกรมสำหรับเรียกฟังก์ชัน sleep_cpu
#define SLEEP_POWER_DOWN 2 //ค่าสำหรับตั้งให้เข้าสู่สภาวะหลับแบบลดพลังงาน
#define INT0_PIN 2      //กำหนด PIN2 สำหรับการขัดจังหวะ INTO0
#define INT1_PIN 3      //กำหนด PIN3 สำหรับการขัดจังหวะ INTO1

volatile uint8_t count; //สร้างตัวแปร count ใช้เก็บการเพิ่ม-ลดค่าของ OUTPUT

void SLEEP_DISABLE()    //ฟังก์ชันสำหรับปิดทางไม่ให้ตัวประมวลผลเข้าสู่ภาวะหลับ
{                       //เริ่มฟังก์ชัน SLEEP_DISABLE
  SMCR &= 0xFE;         //สั่งให้บิต SE(บิตล่างสุด) ในเรจิสเตอร์ SMCR มีค่าตรรกะต่ำ
}                       //สิ้นสุดฟังก์ชัน SLEEP_DISABLE

void SLEEP_INITIALIZE(uint8_t m)    //ฟังก์ชันสำหรับเปิดทางให้ CPU สามารถเข้าสู่สภาวะหลับได้
{                                   //--และรับพารามิเตอร์ระบุค่าหมายเลขแบบวิธีของภาวะหลับ
  SMCR = (m << 1) | 0x01;           //ตั้งค่าแบบวิธีภาวะหลับ และตั้งให้บิต SE มีค่าตรรกะสูง
}                                   //สิ้นสุดฟังก์ชัน SLEEP_INITIALIZE

void setup() {
  pinMode(INT0_PIN, INPUT_PULLUP);  //กำหนด INPUT PIN INT0
  pinMode(INT1_PIN, INPUT_PULLUP);  //กำหนด INPUT PIN INT1
  attachInterrupt(digitalPinToInterrupt(INT0_PIN), INT0_ISR, FALLING);
  //กำหนดการเกิดบริการขัดจังหวะ INT0
  attachInterrupt(digitalPinToInterrupt(INT1_PIN), INT1_ISR, FALLING);
  //กำหนดการเกิดบริการขัดจังหวะ INT1
  DDRD = 0xF0;  //กำหนด PORTD เป็นทิศทางการรับเข้า-ส่งออกข้อมูล
  DDRB = 0x0F;  //กำหนด PORTB เป็นทิศทางการรับเข้า-ส่งออกข้อมูล
  count = 255; //LED 8 ดวง ที่ใช้แสดงผล 
  display_LED(count); //นำค่าจากตัวแปร count ไปแสดง OUTPUT บน LED
}

void loop() {
  SLEEP_INITIALIZE(SLEEP_POWER_DOWN);  //ตั้งค่าการทำงานในภาวะหลับแบบเดินเครื่องเปล่า
  sleep_cpu();          //สั่งให้ตัวประมวลผลเข้าสู่ภาวะหลับ
  SLEEP_DISABLE();      //เมื่อตื่นจากภาวะหลับ ให้ปิดการทางการเข้าสู่ภาวะหลับ
}

void display_LED(uint8_t val){  //เลื่อนบิตในพารามิเตอร์ val เพื่อแบ่งบิตในการแสดง OUTPUT
  PORTD = (val << 4); //แสดง OUTPUT 4 บิตบน ทาง PORTD
  PORTB = (val >> 4); //แสดง OUTPUT 4 บิตล่าง ทาง PORTB
}

void INT0_ISR(){  //Function Interrupt ของ INT0
  _delay_ms(15);
  if(!digitalRead(INT0_PIN)){ //หากมีการกดปุ่ม S1
    count < 256 ? count++ : count = 0;
    //ตรวจสอบว่าค่าในตัวแปร count มีค่าน้อยกว่า 256 หรือไม่
    //--ถ้าใช่ให้เพิ่มค่าในตัวแปร count ขึ้นหนึ่งค่า (ค่าบน LED เพิ่มขึ้นครั้งละ 1 ค่า)
    //-ถ้าไม่ใช่ให้ค่าในตัวแปร count เท่ากับ 0
    display_LED(~count);  //นำค่าจากตัวแปร count ไปแสดงผล OUTPUT บน LED Cathode
  }
}

void INT1_ISR(){  //Function Interrupt ของ INT1 
  _delay_ms(15);
  if(!digitalRead(INT1_PIN)){ //หากมีการกดปุ่ม S2
    count > 0 ? count-- : count = 255;
    //ตรวจสอบว่าค่าในตัวแปร count มีค่ามากกว่า 0 หรือไม่
    //--ถ้าใช่ให้ลดค่าในตัวแปร count ลงหนึ่งค่า (ค่าบน LED ลดลงครั้งละ 1 ค่า)
    //--ถ้าไม่ใช่ให้ค่าในตัวแปร count เท่ากับ 0
    display_LED(~count);  //นำค่าจากตัวแปร count ไปแสดงผล OUTPUT บน LED Cathode
  }
}
