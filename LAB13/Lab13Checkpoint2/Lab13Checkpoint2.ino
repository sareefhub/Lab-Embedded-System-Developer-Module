#include <avr/io.h>           //เรียกใช้คลังโปรแกรม io.h
#include <avr/interrupt.h>    //เรียกใช้คลังโปรแกรมสำหรับขัดจังหวะ
#include "my_twi.h"           //เรียกใช้คลังโปรแกรมจัดการมอดูลทีดับเบิลยูไอ
#include "my_pcf8574.h"       //เรียกใช้คลังโปรแกรมจัดการไอซี PCF8574
                              //---ส่วนบิตอื่นที่เหลือเป็นเอาต์พุต
uint8_t mode;                //ตั้งค่าตัวแปรส่วนกลาง (global variable)
unsigned long last_time;
unsigned long previous_time; //สร้างตัวแปรเพื่อใช้ในการนับเวลา 1s

uint8_t prepare_data(uint8_t d)
{
  uint8_t tmp;
  tmp = ~d;                   //กลับทุกบิตเป็นตรงข้ามเนื่องจาก LED เป็น Common A
  tmp |= 0x80;                //เขียนค่ารหัสขับแอลอีดีโดยมิให้มีผล                               
  return tmp;                 //--กระทบต่อบิต MSB ที่ใช้เป็นอินพุต 
}

void setup()
{
  last_time =0;  
  Serial.begin(38400);
  uint8_t tmp, i;             //ประกาศตัวแปร tmp และตัวแปร i ขนาด 8 บิต
  init_twi_module();          //สั่งให้มอดูลทีดับเบิลยูไอเริ่มทำงาน
  mode = 0;                   //สั่งให้ตัวแปร count มีค่าเริ่มต้นเท่ากับศูนย์       
}                             //จบการทำงานของฟังก์ชันหลัก

void loop(){
  
  uint8_t tmp;

  tmp = PCF8574_read(0);
  tmp &= 0x80;  //กรองบิต
  if(!tmp)      //ตรวจสอบว่ามีการกดปุ่มของสวิตช์หรือไม่
  {
    if( millis() - last_time > 20) 
    //ตรวจสอบว่าค่าเวลาในปัจจุบันมีระยะห่างจากครั้งก่อนหน้าอยู่ 20ms หรือไม่
    {
      last_time = millis();   //ให้ตัวแปร last_time เก็บค่าเวลาก่อนหน้าในรูปแบบมิลลิวินาที
      tmp = PCF8574_read(0);
      tmp &= 0x80;
      if (!tmp)   //หากพบว่ามีการกดปุ่มของสวิตช์ 
      {
        //Serial.println("Keypressed detected");
        mode < 3 ? mode++ : mode = 0; 
        //ตรวจสอบว่าค่าในตัวแปร mode มีค่าน้อยกว่า 3 หรือไม่
        //ถ้าใช่ให้เพิ่มค่าในตัวแปร mode ขึ้นหนึ่งค่า (ทำงานในโหมดถัดไป)
        //ถ้าไม่ใช่ให้ค่าในตัวแปร mode เท่ากับ 0 (กลับมาเริ่มโหมดเดิมใหม่อีกครั้ง)
      }
      do
      {
        tmp = PCF8574_read(0);    //อ่านค่าสถานะทางตรรกะจากพอร์ตของ PCF8574
        tmp &= 0x80;
      }
      while(!tmp);    
      previous_time = millis();   //ให้ตัวแปร previous_time เก็บค่าเวลาก่อนหน้าในรูปแบบมิลลิวินาที
    }
  }
  
  switch(mode){                   //Set ค่าการทำงานของสวิตช์ 4 โหมด
    case 0 : count(); break;      //Mode M1 LED เพิ่มค่าขึ้นทีละ 1 
    case 1 : shiftRight(); break; //Mode M2 เลื่อนทุกบิตไปทางขวา
    case 2 : shiftLeft(); break;  //Mode M3 เลื่อนทุกบิตไปทางซ้าย
    case 3 : led_blink(); break;  //Mode M4 LED ติดสลับดับ
  }
}

void count(){           //Mode M1 LED เพิ่มค่าขึ้นทีละ 1 
  static uint8_t count; //สร้าง static ใช้ในการนับ int
  uint8_t tmp;
  if((millis() - previous_time) > 1000){
  //ตรวจสอบว่าค่าเวลาในปัจจุบันมีระยะห่างจาก previous_time อยู่ 1ms หรือไม่
    previous_time = millis(); 
    //ระยะห่างกันเกิน 1ms ให้เก็บค่าเวลาปัจจุบันไว้ในตัวแปร previous_time เพื่อรอเปรียบเทียบครั้งถัดไป
    count < 127 ? count++ : count = 0;
    //ตรวจสอบว่าค่าในตัวแปร count มีค่าน้อยกว่า 127 หรือไม่ 
    //ถ้าใช่ให้เพิ่มค่าในตัวแปร count ขึ้นหนึ่งค่า (แอลอีดีเพิ่มขึ้นหนึ่งค่าทุกครั้งที่มีการกดสวิตช์)
    //ถ้าไม่ใช่ให้ค่าในตัวแปร count เท่ากับ 0 (แอลอีดีจะดับทุกดวงใหม่อีกครั้ง)
  }
  tmp = prepare_data(count);
  PCF8574_write(0, tmp); 
}

void shiftRight(){ //Mode M2 เลื่อนทุกบิตไปทางขวา
  static uint8_t position;    
  static uint8_t led = 0x40; //0100 000 LED ติดจากซ้ายสุดแล้วเลื่อนไปทางขวา
  uint8_t tmp;
  if((millis() - previous_time) > 1000){
  //ตรวจสอบว่าค่าเวลาในปัจจุบันมีระยะห่างจาก previous_time อยู่ 1ms หรือไม่
    previous_time = millis();
    //ระยะห่างกันเกิน 1ms ให้เก็บค่าเวลาปัจจุบันไว้ในตัวแปร previous_time เพื่อรอเปรียบเทียบครั้งถัดไป
    position < 6 ? position++ : position = 0; 
    //ตรวจสอบว่าค่าในตัวแปร position น้อยกว่า 6 หรือไม่
    //ถ้าใช่ให้เพิ่มค่าในตัวแปร position ขึ้นหนึ่งค่า ถ้าไม่ใช่ให้ค่าในตัวแปร position เท่ากับ 0
    led = 0x40 >> position; //ชิบบิตไปทางขวา
  }
  tmp = prepare_data(led);  //ส่งค่าข้อมูล LED ไปให้กับตัวแปร tmp
  PCF8574_write(0, tmp);    //นำค่าที่เก็บไว้ในตัวแปร tmp ไปแสดงผล 
}

void shiftLeft(){ //Mode M3 เลื่อนทุกบิตไปทางซ้าย
  static uint8_t position;
  static uint8_t led = 0x01; //0000 0001 LED ติดเริ่มจากขวาสุดแล้วเลื่อนไปทางซ้าย
  uint8_t tmp;
  if((millis() - previous_time) > 1000){
  //ตรวจสอบว่าค่าเวลาในปัจจุบันมีระยะห่างจาก previous_time อยู่ 1ms หรือไม่
    previous_time = millis();
    //ระยะห่างกันเกิน 1ms ให้เก็บค่าเวลาปัจจุบันไว้ในตัวแปร previous_time เพื่อรอเปรียบเทียบครั้งถัดไป
    position < 6 ? position++ : position = 0;
    //ตรวจสอบว่าค่าในตัวแปร position น้อยกว่า 6 หรือไม่
    //ถ้าใช่ให้เพิ่มค่าในตัวแปร position ขึ้นหนึ่งค่า ถ้าไม่ใช่ให้ค่าในตัวแปร position เท่ากับ 0
    led = 0x01 << position; //ชิบบิตไปทางซ้าย
  }
  tmp = prepare_data(led);  //ส่งค่าข้อมูล LED ไปเก็บไว้ในตัวแปร tmp
  PCF8574_write(0, tmp);    //นำค่าที่เก็บไว้ในตัวแปร tmp ไปแสดงผล
}

void led_blink(){ //Mode M4 LED ติดสลับดับ
  static uint8_t led = 0xFF; //LED ติด 1111 1111 
  uint8_t tmp;
  if((millis() - previous_time) > 1000){
  //ตรวจสอบว่าค่าเวลาในปัจจุบันมีระยะห่างจาก previous_time อยู่ 1ms หรือไม่
    previous_time = millis();
    //ระยะห่างกันเกิน 1ms ให้เก็บค่าเวลาปัจจุบันไว้ในตัวแปร previous_time เพื่อรอเปรียบเทียบครั้งถัดไป
    led = ~led; //ทุกๆ 1บิต จะมีการกลับบิต
  }
  tmp = prepare_data(led);  //ส่งค่าข้อมูล LED ไปเก็บไว้ในตัวแปร tmp
  PCF8574_write(0, tmp);    //master ส่งค่าไปให้ borad | 0 = ค่าaddress A0 A2
}
