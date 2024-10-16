//Lab14ch4
const int strobe = 7; // ขา STROBE สำหรับเชื่อมต่อกับโมดูล TM1638
const int clock = 9;  // ขา CLOCK สำหรับเชื่อมต่อกับโมดูล TM1638
const int data = 8;   // ขา DATA สำหรับเชื่อมต่อกับโมดูล TM1638
volatile unsigned int hour, minute, sec; // ประกาศตัวแปรสำหรับเก็บค่าเวลา ชั่วโมง นาที และวินาที
int count = 0; // ประกาศ count สำหรับนับ 100ms ต้องนับ 10ครั้ง จึงเท่ากับ 0.1s

ISR(TIMER1_COMPA_vect) {
  if (count == 10) {
    update_time(); // เรียกฟังก์ชันอัปเดตเวลา
    display_7seg(); // แสดงผลเวลาใน 7-segment display
    count = 0; // รีเซ็ต count กลับเป็น 0
  }
  uint8_t mode = readTM1638_Buttons(); // อ่านค่าจากการกดปุ่มสวิตช์
  if (mode) { // หากมีการกดปุ่มสวิตช์ ให้มาทำงานที่โหมด
    switch (mode) {
      case 0x01: hour < 23 ? hour++ : hour = 0; break; // เพิ่มค่าชั่วโมง หากมากกว่า 23 ให้กลับเป็น 0
      case 0x02: hour > 0 ? hour-- : hour = 23; break; // ลดค่าชั่วโมง หากน้อยกว่า 0 ให้กลับเป็น 23
      case 0x08: minute < 59 ? minute++ : minute = 0; break; // เพิ่มค่านาที หากมากกว่า 59 ให้กลับเป็น 0
      case 0x10: minute > 0 ? minute-- : minute = 59; break; // ลดค่านาที หากน้อยกว่า 0 ให้กลับเป็น 59
      case 0x40: sec = 59; break; // ตั้งค่าวินาทีเป็น 59
      case 0x80: sec = 0; break; // ตั้งค่าวินาทีเป็น 0
    }
    display_7seg(); // แสดงผลเวลาใน 7-segment display
  }
  count++;  // ถ้า count ไม่ถึง 10 ให้นับเพิ่มขึ้นจนถึง 10
}

uint8_t readTM1638_Buttons(void) {
  uint8_t buttons = 0; // ประกาศตัวแปรสำหรับเก็บค่าปุ่มที่กด
  digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
  shiftOut(data, clock, LSBFIRST, 0x42); // ส่งคำสั่งอ่านค่าปุ่ม
  
  pinMode(data, INPUT); // ตั้งค่า DATA เป็นโหมด INPUT
  
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t v = shiftIn(data, clock, LSBFIRST) << i; // อ่านค่าปุ่มแต่ละปุ่ม
    buttons |= v; // บันทึกค่าปุ่มที่กดลงในตัวแปร buttons
  }
  
  pinMode(data, OUTPUT); // ตั้งค่า DATA เป็นโหมด OUTPUT
  digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
  return buttons; // ส่งค่าปุ่มที่กดกลับ
}

void sendCommand(uint8_t value) {
  digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
  shiftOut(data, clock, LSBFIRST, value); // ส่งคำสั่ง
  digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
}

void reset() {
  sendCommand(0x40); // ตั้งโหมด auto increment
  digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
  shiftOut(data, clock, LSBFIRST, 0xc0); // ตั้งค่าเริ่มต้นที่ address 0
  for (uint8_t i = 0; i < 16; i++) {
    shiftOut(data, clock, LSBFIRST, 0x00); // รีเซ็ตค่าทั้งหมด
  }
  digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
}

void setup() {
  TCCR1A = 0; // ตั้งค่า Timer1
  TCNT1 = 0; // ตั้งค่าเริ่มต้นของ Timer1
  OCR1A = 6249; // ตั้งค่าเปรียบเทียบสำหรับ Timer1 
  TCCR1B = (1 << CS12 | 0 << CS11 | 0 << CS10 | 1 << WGM12); // ตั้งค่า prescaler เป็น 256
  TIMSK1 = (1 << OCIE1A); // เปิดใช้งาน Timer1 Compare Match A interrupt
  pinMode(strobe, OUTPUT); // ตั้งค่า STROBE เป็นโหมด OUTPUT
  pinMode(clock, OUTPUT); // ตั้งค่า CLOCK เป็นโหมด OUTPUT
  pinMode(data, OUTPUT); // ตั้งค่า DATA เป็นโหมด OUTPUT
  Serial.begin(9600); // เริ่มต้น Serial communication
  hour = 0; // ตั้งค่าเริ่มต้นของชั่วโมงเป็น 0
  minute = 0; // ตั้งค่าเริ่มต้นของนาทีเป็น 0
  sec = 0; // ตั้งค่าเริ่มต้นของวินาทีเป็น 0
  sendCommand(0x8f); // เปิดใช้งานโมดูล TM1638
  reset(); // รีเซ็ตค่าทั้งหมด
  display_7seg(); // แสดงผลเวลาใน 7-segment display
}

void update_time() {
  if (sec == 59) { // ถ้าวินาทีเท่ากับ 59
    sec = 0; // รีเซ็ตวินาทีเป็น 0
    if (minute == 59) { // ถ้านาทีเท่ากับ 59
      minute = 0; // รีเซ็ตนาทีเป็น 0
      hour == 23 ? 0 : hour++; // ถ้าชั่วโมงเท่ากับ 23 ให้รีเซ็ตเป็น 0 หากไม่ใช่ให้เพิ่มค่าชั่วโมง
    } else {
      minute++; // เพิ่มค่านาที
    }
  } else {
    sec++; // เพิ่มค่าวินาที
  }
}

void display_7seg() {
   uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f }; // ตั้งค่าโค้ดสำหรับตัวเลข 0-9 ใน 7-segment display
   sendCommand(0x44); // ตั้งค่า single address mode
   
   // Hour
   digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
   shiftOut(data, clock, LSBFIRST, 0xc0); // ตั้งค่า address สำหรับแสดงผลชั่วโมงหลักสิบ
   shiftOut(data, clock, LSBFIRST, digits[hour / 10]); // ส่งข้อมูลชั่วโมงหลักสิบ
   digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
   
   digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
   shiftOut(data, clock, LSBFIRST, 0xc2); // ตั้งค่า address สำหรับแสดงผลชั่วโมงหลักหน่วย
   shiftOut(data, clock, LSBFIRST, digits[hour % 10]); // ส่งข้อมูลชั่วโมงหลักหน่วย
   digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
   
   // Seperator
   digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
   shiftOut(data, clock, LSBFIRST, 0xc4); // ตั้งค่า address สำหรับแสดงผลตัวคั่น
   shiftOut(data, clock, LSBFIRST, 0x40); // ส่งข้อมูลตัวคั่น
   digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
   
   // Minute
   digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
   shiftOut(data, clock, LSBFIRST, 0xc6); // ตั้งค่า address สำหรับแสดงผลนาทีหลักสิบ
   shiftOut(data, clock, LSBFIRST, digits[minute / 10]); // ส่งข้อมูลนาทีหลักสิบ
   digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
   
   digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
   shiftOut(data, clock, LSBFIRST, 0xc8); // ตั้งค่า address สำหรับแสดงผลนาทีหลักหน่วย
   shiftOut(data, clock, LSBFIRST, digits[minute % 10]); // ส่งข้อมูลนาทีหลักหน่วย
   digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
   
   // Seperator
   digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
   shiftOut(data, clock, LSBFIRST, 0xca); // ตั้งค่า address สำหรับแสดงผลตัวคั่น
   shiftOut(data, clock, LSBFIRST, 0x40); // ส่งข้อมูลตัวคั่น
   digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
   
   // Second
   digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
   shiftOut(data, clock, LSBFIRST, 0xcc); // ตั้งค่า address สำหรับแสดงผลวินาทีหลักสิบ
   shiftOut(data, clock, LSBFIRST, digits[sec / 10]); // ส่งข้อมูลวินาทีหลักสิบ
   digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
   
   digitalWrite(strobe, LOW); // เริ่มการส่งข้อมูล
   shiftOut(data, clock, LSBFIRST, 0xce); // ตั้งค่า address สำหรับแสดงผลวินาทีหลักหน่วย
   shiftOut(data, clock, LSBFIRST, digits[sec % 10]); // ส่งข้อมูลวินาทีหลักหน่วย
   digitalWrite(strobe, HIGH); // จบการส่งข้อมูล
}

void loop() {
  // ว่าง ไม่มีการทำงานใน loop
}
