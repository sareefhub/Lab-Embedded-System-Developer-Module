#define INT0_PIN 2  //กำหนด PIN2 สำหรับการขัดจังหวะ INTO0
#define INT1_PIN 3  //กำหนด PIN3 สำหรับการขัดจังหวะ INTO1

volatile uint8_t count; //สร้างตัวแปร count เก็บการเพิ่ม-ลดค่าของ OUTPUT
void setup() {
  // put your setup code here, to run once:
  pinMode(INT0_PIN, INPUT_PULLUP);  //กำหนด INPUT PIN INT0
  pinMode(INT1_PIN, INPUT_PULLUP);  //กำหนด INPUT PIN INT1
  attachInterrupt(digitalPinToInterrupt(INT0_PIN), INT0_ISR, FALLING);  
  //กำหนดการเกิดบริการขัดจังหวะ INT0
  attachInterrupt(digitalPinToInterrupt(INT1_PIN), INT1_ISR, FALLING);
  //กำหนดการเกิดบริการขัดจังหวะ INT1
  DDRD = 0xF0;  //กำหนด PORTD เป็นทิศทางการรับเข้า-ส่งออกข้อมูล
  DDRB = 0x0F;  //กำหนด PORTB เป็นทิศทางการรับเข้า-ส่งออกข้อมูล
  count = 255;  //LED 8 ดวง ที่ใช้แสดงผล 
  display_LED(count); //นำค่าจากตัวแปร count ไปแสดง OUTPUT บน LED
}

void loop() {

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
    display_LED(~count); //นำค่าจากตัวแปร count ไปแสดงผล OUTPUT บน LED Cathode
  }
}

void INT1_ISR(){  //Function Interrupt ของ INT1 
  _delay_ms(15);
  if(!digitalRead(INT1_PIN)){ //หากมีการกดปุ่ม S2
    count > 0 ? count-- : count = 0;
    //ตรวจสอบว่าค่าในตัวแปร count มีค่ามากกว่า 0 หรือไม่
    //--ถ้าใช่ให้ลดค่าในตัวแปร count ลงหนึ่งค่า (ค่าบน LED ลดลงครั้งละ 1 ค่า)
    //--ถ้าไม่ใช่ให้ค่าในตัวแปร count เท่ากับ 0
    display_LED(~count);  //นำค่าจากตัวแปร count ไปแสดงผล OUTPUT บน LED Cathode
  }
}

