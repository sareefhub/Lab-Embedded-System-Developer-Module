#include <avr/sleep.h>  //เรียกใช้คลังโปรแกรมสำหรับเรียกฟังก์ชัน sleep_cpu
#define INT0_PIN 2      //กำหนด PIN2 สำหรับการขัดจังหวะ INTO0
#define INT1_PIN 3      //กำหนด PIN3 สำหรับการขัดจังหวะ INTO1

void SLEEP_DISABLE()    //ฟังก์ชันสำหรับปิดทางไม่ให้ตัวประมวลผลเข้าสู่ภาวะหลับ
{                       //เริ่มฟังก์ชัน SLEEP_DISABLE
  SMCR &= 0xFE;         //สั่งให้บิต SE(บิตล่างสุด) ในเรจิสเตอร์ SMCR มีค่าตรรกะต่ำ
}                       //สิ้นสุดฟังก์ชัน SLEEP_DISABLE

void SLEEP_INITIALIZE(uint8_t m)  //ฟังก์ชันสำหรับเปิดทางให้ CPU สามารถเข้าสู่สภาวะหลับได้
{                                 //--และรับพารามิเตอร์ระบุค่าหมายเลขแบบวิธีของภาวะหลับ
  SMCR = (m << 1) | 0x01;         //ตั้งค่าแบบวิธีภาวะหลับ และตั้งให้บิต SE มีค่าตรรกะสูง
}                                 //สิ้นสุดฟังก์ชัน SLEEP_INITIALIZE

void setup() {
  pinMode(INT0_PIN, INPUT_PULLUP);  //กำหนด INPUT PIN INT0
  pinMode(INT1_PIN, INPUT_PULLUP);  //กำหนด INPUT PIN INT1
  attachInterrupt(digitalPinToInterrupt(INT0_PIN), INT0_ISR, FALLING);
  //กำหนดการเกิดบริการขัดจังหวะ INT0
  attachInterrupt(digitalPinToInterrupt(INT1_PIN), INT1_ISR, FALLING);
  //กำหนดการเกิดบริการขัดจังหวะ INT1
  DDRD = 0xF0;  //กำหนด PORTD เป็นทิศทางการรับเข้าข้อมูล
  DDRB = 0x0F;  //กำหนด PORD เป็นทิศทางการส่งออกข้อมูล
  PORTD = 0xFF; //แปลงบิต PORTD ให้เป็นตรรกะสูงทุกบิต (LED ดับทุกดวง)
  PORTB = 0xFF; //แปลงบิต PORTB ให้เป็นตรรกะสูงทุกบิต (LED ดับทุกดวง)
}

void loop() {
  SLEEP_INITIALIZE(1);  //ตั้งค่าการทำงานในภาวะหลับแบบเดินเครื่องเปล่า
  sleep_cpu();          //สั่งให้ตัวประมวลผลเข้าสู่ภาวะหลับ
  SLEEP_DISABLE();      //เมื่อตื่นจากภาวะหลับ ให้ปิดการทางการเข้าสู่ภาวะหลับ 
}

void INT0_ISR(){  //Function Interrupt ของ INT0
  _delay_ms(15);
  if(!digitalRead(INT0_PIN)){
  //หากมีการกด S1 
  //--ทำให้ไฟวิ่งติดครั้งละสองดวง
  //--โดยวิ่งจากขอบนอกเข้ามาสู่ตรงกลาง
    for(int i=0; i < 3;i++){
      for(int j = 0; j < 4; j++){
        PORTD = ~(0x10 << j);
        PORTB = ~(0x08 >> j);
        _delay_ms(100);
      }
    }
    PORTD = 0xFF; //LED ดับทุกดวง
    PORTB = 0xFF; //LED ดับทุกดวง
  }
}

void INT1_ISR(){  //Function Interrupt ของ INT1
  _delay_ms(15);
  if(!digitalRead(INT1_PIN)){
  //หากมีการกด S2 
  //--ทำให้ไฟวิ่งติดครั้งละสองดวง
  //--โดยวิ่งจากตรงกลางออกไปสู่ขอบนอก
    for(int i=0; i < 3; i++){
      for(int j = 0; j < 4; j++){
        PORTD = ~(0x80 >> j);
        PORTB = ~(0x01 << j);
        _delay_ms(100);
      }
    }
    PORTD = 0xFF; //LED ดับทุกดวง
    PORTB = 0xFF; //LED ดับทุกดวง
  }
}



