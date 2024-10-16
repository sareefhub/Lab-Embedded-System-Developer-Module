//LAB11Ch1 นับเวลาเรื่อยๆโดยไม่ใช้ Interrupt
const int strobe = 7; //กำหนด PIN7 สำหรับ strobe 
                      //--ใช้ในการกำหนดว่าตอนนี้ให้อ่านข้อมูลไหม
                      //--ส่งข้อมูล TM1638 = LOW
const int clock = 9;  //กำหนด PIN9 สำหรับ clk ขานี้ใช้สำหรับส่งและรับข้อมูลระหว่าง TM1638 และ Arduino
const int data = 8;   //กำหนด PIN8 สำหรับ data ขานี้ใช้สำหรับสัญญาณนาฬิกาเพื่อซิงโครไนซ์ข้อมูลระหว่าง TM1638 และ Arduino
unsigned int hour, minute, sec;

void sendCommand(uint8_t value) //รับค่ามา 1 ค่า ขนาด 8 บิต
{
  digitalWrite(strobe, LOW);  //ก่อนจะส่งข้อมูลให้ Set Stobe = LOW
  shiftOut(data, clock, LSBFIRST, value); 
  //shiftOut = การส่งข้อมูลแบบ serial communication
  //--shiftOut ต้องการทราบว่าขาไหนเป็นขา data, clock
  //--เป็นกำหนดว่าให้บิตไหนทำงานก่อน 
  //--LSBFIRST = บิตขวาสุดถูกส่งไปก่อน
  digitalWrite(strobe, HIGH); //กลับไปยังโหมดเดิม
}

void reset()
{
  sendCommand(0x40); //set auto increment mode
                     //--ส่งค่า 0x40 ไปก่อน
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);   //set starting address to 0
  for(uint8_t i = 0; i < 16; i++) //ตัว display แสดงค่า 16 ตำแหน่ง (LED 16 ตัว)
  {
    shiftOut(data, clock, LSBFIRST, 0x00);  //ส่งให้ไฟดับ
  }
  digitalWrite(strobe, HIGH);
}

void setup()
{
  pinMode(strobe, OUTPUT);  //SET UP PINMODE
  pinMode(clock, OUTPUT);   //SET UP PINMODE
  pinMode(data, OUTPUT);    //SET UP PINMODE
  hour = 0;
  minute = 0;
  sec = 0;
  sendCommand(0x8f);  //เริ่มการทำงานของบอร์ด เป็นการส่ง command ให้ TM1638
  reset();  //เรียก Function 1รอบ เพื่อเคลียค่าบอร์ดให้หมด 
  Serial.begin(9600);
}

void update_time(){
  if(sec == 59){
    sec = 0; ////ถ้าวนวินาทีนาทีครบ 59 นาทีแล้วจะวนกลับไปเริ่มนับใหม่เป็น 0
    if(minute == 59){ //ถ้าวนนาทีครบ 59 นาทีแล้วจะวนกลับไปเริ่มนับใหม่เป็น 0
      minute = 0;
      hour == 23. ? 0 : hour++; //ถ้าวนลูปครบ 23 ชม แล้วจะวนกลับไปเริ่มนับใหม่เป็น 0
                                //--ถ้ายังวนไม่ครบถึง 23 ก็ให้เพิ่มขึน 1 ค่า
                                //--แล้วนาทีเป็น 59 เมื่อเพิ่มขึ้น 1 ค่า ก็จะเท่ากับ 0 ให้บวก hr เพิ่ม
                                //--ถ้านาทียังไม่ถึง 59 ก็เพิ่มค่านาทีขึ้น 1 ค่า
    }
    else{
      minute++;
    }
  }
  else{
    sec++;    //วินาทีก็ทำเหมือนกัน
  }
}

void display_7seg()
{
 uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f }; //แสดงค่า output เป็ยเลข 0-9 
 sendCommand(0x44); // set single address
 //Hour
 digitalWrite(strobe, LOW); //ก่อนที่จะส่งข้อมูลต้อง set strobe เป็น Low
 shiftOut(data, clock, LSBFIRST, 0xc0); //shiftOut เพื่อบอกว่าจะส่งข้อมูลไหนไปยังตำแหน่งไหน
 shiftOut(data, clock, LSBFIRST, digits[hour/10]); //ข้อมูลที่ต้องการจะส่ง (ค่าชั่วโมงหลักสิบ)
 digitalWrite(strobe, HIGH); //ส่งข้อมูลเสร็จ ser strobe เป็น high
 digitalWrite(strobe, LOW); //set strobe เป็น LOW เพื่อนเตรียมส่งข้อมูลถัดไป
 shiftOut(data, clock, LSBFIRST, 0xc2); //ส่งข้อมูลไปยังตำแหน่งไหน (ตำแหน่ง 0xc2)
 shiftOut(data, clock, LSBFIRST, digits[hour%10]); //ค่าชั่วโมงหลักหน่วย
 digitalWrite(strobe, HIGH);
 //Seperate ขีดขั้นกลาง
 digitalWrite(strobe, LOW); 
 shiftOut(data, clock, LSBFIRST, 0xc4); //ส่งข้อมูลไปยังตำแหน่งไหน
 shiftOut(data, clock, LSBFIRST, 0x40); //ให้ตำแหน่งที่ขีดตรงกลาง G ติด
 digitalWrite(strobe, HIGH);
 //Min
 digitalWrite(strobe, LOW);
 shiftOut(data, clock, LSBFIRST, 0xc6);
 shiftOut(data, clock, LSBFIRST, digits[minute/10]);
 digitalWrite(strobe, HIGH);
 digitalWrite(strobe, LOW);
 shiftOut(data, clock, LSBFIRST, 0xc8);
 shiftOut(data, clock, LSBFIRST, digits[minute%10]);
 digitalWrite(strobe, HIGH);
 //Seperate
 digitalWrite(strobe, LOW);  
 shiftOut(data, clock, LSBFIRST, 0xca);
 shiftOut(data, clock, LSBFIRST, 0x40);
 digitalWrite(strobe, HIGH);
 //Sec
 digitalWrite(strobe, LOW);
 shiftOut(data, clock, LSBFIRST, 0xcc);
 shiftOut(data, clock, LSBFIRST, digits[sec/10]);
 digitalWrite(strobe, HIGH);
 digitalWrite(strobe, LOW);
 shiftOut(data, clock, LSBFIRST, 0xce);
 shiftOut(data, clock, LSBFIRST, digits[sec%10]);
 digitalWrite(strobe, HIGH);
 Serial.println(digits[sec/10], HEX);
 Serial.println(digits[sec%10], HEX);
}

void loop()
{
  Serial.print(hour);
  Serial.print(minute);
  Serial.print(sec);
  Serial.println();
  display_7seg();
  delay(1000);  //1000ms เพื่อให้ครบ 1s แล้วไปทำใน update time
  update_time();
}