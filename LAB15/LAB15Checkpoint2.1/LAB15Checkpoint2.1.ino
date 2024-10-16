void setup() {
  DDRD = 0xF0;  //กำหนด PORTD เป็นทิศทางการรับเข้าข้อมูล
  DDRB = 0x0F;  //กำหนด PORTB เป็นทิศทางการส่งออกข้อมูล
  PORTD = 0xFF; //แปลงบิต PORTD ให้เป็นตรรกะสูงทุกบิต (LED ดับทุกดวง)
  PORTB = 0xFF; //แปลงบิต PORTB ให้เป็นตรรกะสูงทุกบิต (LED ดับทุกดวง)
}

void loop() {
  if(!digitalRead(2) || !digitalRead(3)){ 
  //ตรวจสอบค่าการกดสวิตช์ S1 และ S2 โดยไม่มีการเข้าสู่ภาวะหลับ
    delay(10);
    if(!digitalRead(2)){  //หากมีการกดปุ่ม S1
      led_run_in();       //มีการเรียกใช้ฟังก์ชัน led_run_in
    }
    if(!digitalRead(3)){  //หากกดปุ่ม S2
      led_run_out();      //การเรียกใช้ฟังก์ชัน led_run_out
    }
  }
}

void led_run_in(){    
//Function led_run_in (การกดปุ่ม S1)
//--หากกด S1 ทำให้ไฟวิ่งติดครั้งละสองดวง
//--โดยวิ่งจากขอบนอกเข้าสู่ตรงกลาง
  for(int i=0; i < 3;i++){ 
    for(int j = 0; j < 4; j++){
      PORTD = ~(0x10 << j);
      PORTB = ~(0x08 >> j);
      delay(100);
    }
  }
  PORTD = 0xFF; //LED ดับทุกดวง
  PORTB = 0xFF; //LED ดับทุกดวง
}

void led_run_out(){   //Function led_run_out (การกดปุ่ม S2)
//--หากกด S2 ทำให้ไฟวิ่งติดครั้งละสองดวง
//--โดยวิ่งจากตรงกลางออกไปสู่ขอบนอก
  for(int i=0; i < 3; i++){
    for(int j = 0; j < 4; j++){
      PORTD = ~(0x80 >> j);
      PORTB = ~(0x01 << j);
      delay(100);
    }
  }
  PORTD = 0xFF; //LED ดับทุกดวง
  PORTB = 0xFF; //LED ดับทุกดวง
}

