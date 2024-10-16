int soundPin = 5;                         //กำหนด Pin PD5 รับ sound 
int ledPin = 2;                           //กำหนด Pin PD2 แสดง LED 
unsigned int state;                       //สร้างตัวแปรเก็บค่าสถานะของหลอดไฟ
unsigned long previous_time = 0;          //สร้างตัวแปรเก็บข้อมูลเวลา
bool check_clap() {                       //Function check clap
  static unsigned long current_time;
  char sw;                                //สร้างตัวแปรเพื่อเก็บข้อมูลจากมอดูล sound
  sw = digitalRead(soundPin);             //อ่านค่าจาก output soundpin
  if (sw) {        //ตรวจสอบว่ามีการปรบมือหรือไม่
    return false;  // ถ้า if เท่ากับ 1 แสดงว่าไม่มีการปรบมือให้ return false
  }
  else {          //ถ้า if เท่ากับ 0 แสดงว่ามีการปรบมือ
    current_time = millis();    //สร้างตัวแปรเก็บข้อมูลเวลาปัจจุบันในรูปแบบ millisec

    if ((current_time - previous_time) > 25) {
    //ตรวจสอบว่า current_time มีระยะห่างจาก previous_time อยู่ 25ms หรือไม่
      previous_time = millis();               
      //ระยะเวลาห่างกันเกิน 25ms ให้เก็บค่า current_time ใน previous_time เพื่อรอตรวจสอบการปรบมือครั้งถัดไป
      return true;
    }
    else { //ระยะเวลาห่างกันไม่เกิน 25ms
      previous_time = millis();    
      //เก็บค่า current_time ใน previous_time เพื่อรอตรวจสอบการปรบมือครั้งถัดไป
      return false;
    }
  }
}

//bool state = false;
void setup() {
  pinMode(soundPin, INPUT); //กำหนดรับค่าจาก port เพื่อตรวจสอบว่ามีการปรบมือหรือไม่
  pinMode(ledPin, OUTPUT);  //แสดง output ที่ได้ 
  Serial.begin(9600);
  state = 0;                //เมื่อมีการปรบมือจะมีการ reset state เท่ากับ 0
}

int val;
void loop() {

  val = check_clap();  //ตรวจสอบว่ามีการปรบมือหรือไม่
  if (val == true) {  //ตรวจสอบแล้วพบว่ามีการปรบมือ
    state = !state;   //หากปรบมือจะส่งผลให้หลอดไฟดับและติดสลับกัน
    digitalWrite(ledPin, state);
  }
}