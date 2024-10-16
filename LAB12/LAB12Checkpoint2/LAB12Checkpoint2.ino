int soundPin = 5;                   //กำหนด Pin PD5 รับ sound 
int ledPin = 2;                     //กำหนด Pin PD2 แสดง LED 
unsigned int state;                 //สร้างตัวแปรเก็บค่าสถานะของหลอดไฟ
unsigned long previous_time = 0;    //สร้างตัวแปรเก็บข้อมูลเวลา
int num_clap = 0;                   //Function check clap
bool check_clap(){
  static unsigned long current_time;
  char sw;                         //สร้างตัวแปรเพื่อเก็บข้อมูลจากมอดูล sound 
  sw = digitalRead(soundPin);     //อ่านค่าจาก output soundpin
  if(sw){           //ตรวจสอบว่ามีการปรบมือหรือไม่
    return false;   // ถ้า if เท่ากับ 1 แสดงว่าไม่มีการปรบมือให้ return false
  }
  else{             //ถ้า if เท่ากับ 0 แสดงว่ามีการปรบมือ             
    current_time = millis();  //สร้างตัวแปรเก็บข้อมูลเวลาปัจจุบันในรูปแบบ millisec
    if((current_time - previous_time)>15){  
    //ตรวจสอบว่า current_time มีระยะห่างจาก previous_time อยู่ 25ms หรือไม่
      previous_time = millis();             
      //ระยะเวลาห่างกันเกิน 25ms ให้เก็บค่า current_time ใน previous_time เพื่อรอตรวจสอบการปรบมือครั้งถัดไป
      Serial.println("Clap");
      return true;
    }
    else{//ระยะเวลาห่างกันไม่เกิน 25ms 
      previous_time = millis();   
      //เก็บค่า current_time ใน previous_time เพื่อรอตรวจสอบการปรบมือครั้งถัดไป
      return false;
    }
  }
}

//bool state = false;
void setup(){
  pinMode(soundPin, INPUT);      //กำหนดรับค่าจาก port เพื่อตรวจสอบว่ามีการปรบมือหรือไม่
  pinMode(ledPin, OUTPUT);       //แสดง output ที่ได้ 
  state = 0;                    //เมื่อมีการปรบมือจะมีการ reset state เท่ากับ 0
  Serial.begin(9600);
}

unsigned long first_time_clap; //สร้างตัวแปรเก็บข้อมูลการปรบมือครั้งที่ 1 
int val;
void loop(){
  if((millis() - first_time_clap) > 1000){  
  //ตรวจสอบว่า first_time_clap มีระยะห่างจาก current_time อยู่ 1000ms หรือไม่
      num_clap = 0;
    }
  val = check_clap();                //ตรวจสอบว่ามีการปรบมือหรือไม่
  if(val == true){                   //ตรวจสอบแล้วพบว่ามีการปรบมือ
    if(!num_clap){                   
    //ตรวจสอบว่าเป็นการปรบมือครั้งที่ 1 หรือไม่ถ้าเป็นการปรบมือครั้งที่ 1 num_clap จะเท่ากับ 0 และมีการ return ค่าให้เท่ากับ 1 
      first_time_clap = millis();    //เวลาที่เกิดการปรบมือครั้งที่ 1
      num_clap++;                   //จำนวนครั้งในการปรบมือเท่ากับ 1
      Serial.println("Clap 1");
      return;
    }
    if((millis() - first_time_clap) < 1000){ 
    //ตรวจสอบว่า first_time_clap มีระยะห่างจาก current_time อยู่ 1000ms หรือไม่
      Serial.println("Clap 2");
      num_clap++;                           //ตรวจสอบแล้วพบว่าเวลาห่างกัน 1000ms ให้เพิ่มการปรบมือเป็นเท่ากับ 2
    }else{                                  //ตรวจสอบแล้วพบว่าเวลาไม่ได้อยู่ห่างกัน 1000ms
      Serial.println("Clap 0");
      Serial.println(num_clap);
      num_clap = 0;                         //reset ค่าการปรบมือเท่ากับ 0
    }
    
    if(num_clap == 2){                     //ถ้านับจำนวนการปรบมือได้ 2 ครั้ง
      num_clap = 0;                        //reset เป็น 0
      state = !state;                      //หากปรบมือจะส่งผลให้หลอดไฟดับและติดสลับกัน
      digitalWrite(ledPin, state);
    }
  }
}