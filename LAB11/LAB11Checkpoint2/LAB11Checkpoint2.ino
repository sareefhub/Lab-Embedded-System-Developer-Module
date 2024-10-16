const int strobe = 7; // กำหนดขา strobe
const int clock = 9; // กำหนดขา clock
const int data = 8; // กำหนดขา data
volatile unsigned int hour, minute, sec; // ตัวแปรที่ใช้เก็บค่าชั่วโมง นาที และวินาที

ISR(TIMER1_COMPA_vect){ // ใช้ interrupt เมื่อ Timer1 เกิดการเปรียบเทียบ (compare match)
  update_time(); // อัพเดทเวลา
  display_7seg(); // แสดงผลบน 7-segment display
}  

void sendCommand(uint8_t value) {
  digitalWrite(strobe, LOW); // เริ่มส่งคำสั่ง
  shiftOut(data, clock, LSBFIRST, value); // ส่งข้อมูลผ่าน data และ clock
  digitalWrite(strobe, HIGH); // สิ้นสุดการส่งคำสั่ง
}

void reset() {
  sendCommand(0x40); // set auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0); // set starting address to 0
  for(uint8_t i = 0; i < 16; i++) { // เคลียร์การแสดงผล 16 ตำแหน่ง (LED 16 ตัว)
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
}

void setup() {
  TCCR1A = 0; // เคลียร์การตั้งค่า Timer1
  TCCR1B = (1 << CS12 | 0 << CS11 | 1 << CS10 | 1 << WGM12); // ตั้งค่า prescaler 1024
  TIMSK1 = (1 << OCIE1A); // เปิดใช้งาน interrupt ของ Timer1
  TCNT1 = 0; // รีเซ็ตค่า counter
  OCR1A = 15625; // ค่าเปรียบเทียบเพื่อสร้างการหน่วงเวลา 1 วินาที (16x10^6/1024)
  pinMode(strobe, OUTPUT); // กำหนดขา strobe เป็นขา output
  pinMode(clock, OUTPUT); // กำหนดขา clock เป็นขา output
  pinMode(data, OUTPUT); // กำหนดขา data เป็นขา output
  hour = 0; // ตั้งค่าเริ่มต้นของชั่วโมงเป็น 0
  minute = 0; // ตั้งค่าเริ่มต้นของนาทีเป็น 0
  sec = 0; // ตั้งค่าเริ่มต้นของวินาทีเป็น 0
  sendCommand(0x8f); // เปิดใช้งานการแสดงผล
  reset(); // รีเซ็ตการแสดงผล
  display_7seg(); // แสดงผลเบื้องต้น
}

void update_time() {
  if (sec == 59) {
    sec = 0;
    if (minute == 59) {
      minute = 0;
      hour == 23 ? 0 : hour++; // ถ้าชั่วโมงถึง 23 ให้รีเซ็ตเป็น 0 ไม่ใช่ก็เพิ่ม 1
    } else {
      minute++; // เพิ่มนาที
    }
  } else {
    sec++; // เพิ่มวินาที
  }
}

void display_7seg() {
   uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f }; // ตัวเลขที่ใช้แสดงผลใน 7-segment display
   sendCommand(0x44); // set single address mode
   // แสดงผลชั่วโมง
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc0);
   shiftOut(data, clock, LSBFIRST, digits[hour / 10]);
   digitalWrite(strobe, HIGH);
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc2);
   shiftOut(data, clock, LSBFIRST, digits[hour % 10]);
   digitalWrite(strobe, HIGH);
   // แสดงผลตัวคั่น
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc4);
   shiftOut(data, clock, LSBFIRST, 0x40);
   digitalWrite(strobe, HIGH);
   // แสดงผลนาที
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc6);
   shiftOut(data, clock, LSBFIRST, digits[minute / 10]);
   digitalWrite(strobe, HIGH);
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc8);
   shiftOut(data, clock, LSBFIRST, digits[minute % 10]);
   digitalWrite(strobe, HIGH);
   // แสดงผลตัวคั่น
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xca);
   shiftOut(data, clock, LSBFIRST, 0x40);
   digitalWrite(strobe, HIGH);
   // แสดงผลวินาที
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xcc);
   shiftOut(data, clock, LSBFIRST, digits[sec / 10]);
   digitalWrite(strobe, HIGH);
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xce);
   shiftOut(data, clock, LSBFIRST, digits[sec % 10]);
   digitalWrite(strobe, HIGH);
}

void loop() {
  // ไม่มีการทำงานใน loop เพราะใช้ interrupt
}
