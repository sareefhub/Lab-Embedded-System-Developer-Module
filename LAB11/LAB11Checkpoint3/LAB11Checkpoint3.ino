const int strobe = 7; // กำหนดขา strobe
const int clock = 9; // กำหนดขา clock
const int data = 8; // กำหนดขา data

void sendCommand(uint8_t value) {
  digitalWrite(strobe, LOW); // เริ่มการส่งคำสั่ง
  shiftOut(data, clock, LSBFIRST, value); // ส่งข้อมูลผ่าน data และ clock
  digitalWrite(strobe, HIGH); // สิ้นสุดการส่งคำสั่ง
}

void reset() {
  sendCommand(0x40); // ตั้งค่า auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0); // ตั้งค่าเริ่มต้น address เป็น 0
  for(uint8_t i = 0; i < 16; i++) { // วนลูปเคลียร์การแสดงผล 16 ตำแหน่ง
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
}

uint8_t readTM1638_Buttons(void) {
  uint8_t buttons = 0;
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0x42); // ส่งคำสั่งอ่านข้อมูลปุ่ม

  pinMode(data, INPUT); // ตั้งค่า pin data เป็น input เพื่อรับข้อมูลชั่วคราว
  for (uint8_t i = 0; i < 4; i++) { // วนลูป 4 ครั้งเพื่ออ่านค่าแต่ละปุ่ม
    uint8_t v = shiftIn(data, clock, LSBFIRST) << i; // อ่านค่า 1 ไบต์แล้ว shift ไปทางซ้ายตามรอบ
    buttons |= v; // นำค่าในตัวแปร v มาทำ OR กับ buttons
  }
  pinMode(data, OUTPUT); // ตั้งค่า pin data กลับเป็น output
  digitalWrite(strobe, HIGH);
  return buttons; // คืนค่าปุ่มที่ถูกกด
}

void setTM1638LED(uint8_t value, uint8_t position) {
  pinMode(data, OUTPUT);

  sendCommand(0x44); // ตั้งค่า single address mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC1 + (position << 1)); // ตั้งค่า address ของตำแหน่ง LED
  shiftOut(data, clock, LSBFIRST, value); // ส่งข้อมูลไปยัง LED
  digitalWrite(strobe, HIGH);
}

void setup() {
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT); // ตั้งค่า pin data เป็น output

  sendCommand(0x8f); // เปิดใช้งาน TM1638
  reset(); // รีเซ็ตการแสดงผล
}

void loop() {
  reset(); // รีเซ็ตการแสดงผลในแต่ละ loop
  static uint8_t mode; // ตัวแปร mode สำหรับเก็บค่าการกดปุ่มสวิตช์
  if(readTM1638_Buttons()) // ถ้ามีการกดปุ่ม
    mode = readTM1638_Buttons(); // เก็บค่าที่ได้จากปุ่มในตัวแปร mode
  switch(mode) {
    case 0x01: shiftRight(); break; // ถ้ากดปุ่ม 1 ให้เรียกฟังก์ชัน shiftRight
    case 0x02: setTM1638LED(1, 1); break; // ถ้ากดปุ่ม 2 ให้ LED ตำแหน่ง 1 สว่าง
    case 0x04: setTM1638LED(1, 2); break; // ถ้ากดปุ่ม 3 ให้ LED ตำแหน่ง 2 สว่าง
    case 0x08: setTM1638LED(1, 3); break; // ถ้ากดปุ่ม 4 ให้ LED ตำแหน่ง 3 สว่าง
    case 0x10: setTM1638LED(1, 4); break; // ถ้ากดปุ่ม 5 ให้ LED ตำแหน่ง 4 สว่าง
    case 0x20: setTM1638LED(1, 5); break; // ถ้ากดปุ่ม 6 ให้ LED ตำแหน่ง 5 สว่าง
    case 0x40: setTM1638LED(1, 6); break; // ถ้ากดปุ่ม 7 ให้ LED ตำแหน่ง 6 สว่าง
    case 0x80: shiftLeft(); break; // ถ้ากดปุ่ม 8 ให้เรียกฟังก์ชัน shiftLeft
  }
}

void shiftRight() {
  for(int i = 0; i < 8; i++) { // วนลูป 8 รอบสำหรับ LED 8 ดวง
    setTM1638LED(1, i); // เปิด LED ทีละดวงจากซ้ายไปขวา
    delay(100); // หน่วงเวลา 100 มิลลิวินาที
  }
  for(int i = 0; i < 8; i++) {
    setTM1638LED(0, i); // ปิด LED ทีละดวงจากซ้ายไปขวา
    delay(100); // หน่วงเวลา 100 มิลลิวินาที
  }
  delay(100); // หน่วงเวลา 100 มิลลิวินาที ก่อนจบฟังก์ชัน
}

void shiftLeft() {
  for(int i = 7; i >= 0; i--) { // วนลูป 8 รอบสำหรับ LED 8 ดวงจากขวาไปซ้าย
    setTM1638LED(1, i); // เปิด LED ทีละดวงจากขวาไปซ้าย
    delay(100); // หน่วงเวลา 100 มิลลิวินาที
  }
  for(int i = 7; i >= 0; i--) {
    setTM1638LED(0, i); // ปิด LED ทีละดวงจากขวาไปซ้าย
    delay(100); // หน่วงเวลา 100 มิลลิวินาที
  }
  delay(100); // หน่วงเวลา 100 มิลลิวินาที ก่อนจบฟังก์ชัน
}
