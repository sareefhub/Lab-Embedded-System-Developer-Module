int soundPin = 2;                 //กำหนด Pin PD2 รับ sound  
int ledPin = 6;                   //กำหนด Pin PD6 แสดง LED  
unsigned int state;               //สร้างตัวแปรเก็บค่าสถานะของหลอดไฟ
unsigned long previous_time = 0;  //สร้างตัวแปรเก็บข้อมูลเวลา
int num_clap = 0;                 //Function check clap
//bool state = false;
void setup(){
  pinMode(soundPin, INPUT);   //กำหนดรับค่าจาก port เพื่อตรวจสอบว่ามีการปรบมือหรือไม่
  pinMode(ledPin, OUTPUT);    //แสดง output ที่ได้
  attachInterrupt(digitalPinToInterrupt(2), clap, FALLING); //Set Interrupt (Interrupt 0 )
  //เกิดการ Interrupt เมื่อสัญญาณเปลี่ยนจากตรรกะสูงเป็นตรรกะต่ำ
  state = 0;                    //เมื่อมีการปรบมือจะมีการ reset state เท่ากับ 0
  Serial.begin(9600);
}

unsigned long first_time_clap;  //สร้างตัวแปรเก็บข้อมูลการปรบมือครั้งที่ 1
int val;
void loop(){
}

void clap(){
    if((millis() - previous_time)>15){    //ตรวจสอบว่าเวลาว่ามีระยะห่างกัน 15ms หรือไม่
      previous_time = millis(); 
      //เก็บค่าเวลาปัจจุบันไว้ใน previous_time เพื่อรอตรวจสอบการปรบมือครั้งถัดไป
      Serial.println("Clap");
    }
    else{//ระยะเวลาห่างกันไม่เกิน 15 ms
      previous_time = millis();
      //เก็บค่าเวลาปัจจุบันไว้ใน previous_time เพื่อรอตรวจสอบการปรบมือครั้งถัดไป
      return;
   }
   if((millis() - first_time_clap) > 500){  //ตรวจสอบว่าเวลาว่ามีระยะห่างกัน 500ms หรือไม่
      Serial.println("Clap 0");             
      num_clap = 0;                         //reset ค่าการปรบมือเท่ากับ 0
    }
    
   if(!num_clap){                           //ถ้าเป็นการปรบมือครั้งแรก num_clap เท่ากับ 0 และมีการ return ค่าให้เท่ากับ 1
      first_time_clap = millis();           //เวลาที่เกิดการปรบมือครั้งที่ 1
      Serial.println("Clap 1");
      num_clap++;                           //จำนวนครั้งในการปรบมือเท่ากับ 1
      return;
    }
    Serial.println("Clap 2");           
    num_clap++;
    
    if(num_clap == 2){                      //ถ้านับการปรบมือได้ 2 ครั้ง
      num_clap = 0;                         //reset เป็น 0
      state = !state;                       //หากปรบมือจะส่งผลให้หลอดไฟดับและติดสลับกัน
      digitalWrite(ledPin, state);
    } 
}