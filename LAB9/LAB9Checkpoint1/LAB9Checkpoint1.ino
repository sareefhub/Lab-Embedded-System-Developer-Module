#include<avr/wdt.h>
int status;
int n_led;
void turn_on_led(int led){
  switch(led){
    case 0 : DDRC = 0b0011; if(status)PORTC = 0b0001;else PORTC = 0;break;
    case 1 : DDRC = 0b0011; if(status)PORTC = 0b0010;else PORTC = 0;break;
    case 2 : DDRC = 0b0110; if(status)PORTC = 0b0010;else PORTC = 0;break;
    case 3 : DDRC = 0b0110; if(status)PORTC = 0b0100;else PORTC = 0;break;
    case 4 : DDRC = 0b1100; if(status)PORTC = 0b0100;else PORTC = 0;break;
    case 5 : DDRC = 0b1100; if(status)PORTC = 0b1000;else PORTC = 0;break;
    case 6 : DDRC = 0b0101; if(status)PORTC = 0b0001;else PORTC = 0;break;
    case 7 : DDRC = 0b0101; if(status)PORTC = 0b0100;else PORTC = 0;break;
    case 8 : DDRC = 0b1010; if(status)PORTC = 0b0010;else PORTC = 0;break;
    case 9 : DDRC = 0b1010; if(status)PORTC = 0b1000;else PORTC = 0;break;
    case 10 : DDRC = 0b1001; if(status)PORTC = 0b0001;else PORTC = 0;break;
    case 11 : DDRC = 0b1001; if(status)PORTC = 0b1000;else PORTC = 0;break;
  }
}
  
ISR(TIMER1_OVF_vect) { // เมื่อเกิดการขัดจังหวะจาก Timer 1 overflow
  status = !status; // สลับสถานะของ status (จาก 1 เป็น 0 หรือจาก 0 เป็น 1)
}

void setup() {
  TCCR1A = 0; // ตั้งค่า TCCR1A เป็น 0
  TCCR1B = (0<<CS10 | 0<<CS11 | 1<<CS12); // ตั้งค่า TCCR1B เพื่อกำหนดการนับของ Timer 1
  TIMSK1 = 0x01; // เปิดการขัดจังหวะเมื่อ Timer 1 overflow
  TCNT1 = 3036; // ตั้งค่าเริ่มต้นของการนับใน Timer 1
  status = 1; // ตั้งค่าสถานะเริ่มต้นเป็น 1 (เปิด)
  WDTCSR = (1<<WDCE | 1<<WDE); // เปิดการเขียนลงในรีจิสเตอร์ Watchdog Timer
  WDTCSR = (1<<WDP1 | 1<<WDP2 | 1<<WDE); // ตั้งค่า Watchdog Timer เพื่อหน่วงเวลาประมาณ 1 วินาที
}

void loop() {
  wdt_reset(); // รีเซ็ต Watchdog Timer เพื่อป้องกันการรีเซ็ตไมโครคอนโทรลเลอร์
  turn_on_led(n_led); // เรียกใช้ฟังก์ชันเพื่อเปิดหรือปิด LED ตามหมายเลข n_led
  if(n_led < 11)
    n_led++; // ถ้า n_led น้อยกว่า 11 เพิ่มค่า n_led ทีละ 1
  else
    n_led = 0; // ถ้า n_led เท่ากับ 11 ตั้งค่า n_led กลับเป็น 0
}