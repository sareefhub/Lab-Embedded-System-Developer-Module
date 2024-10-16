//1500 - 
//1355 - 925
//1210 - 826
//1065 - 727
//920 - 628
//775 - 529
//630 - 430
//485 - 331
//340 - 232
//195 - 133
//50 - 34
//Lab16Ch2
#include <Arduino_FreeRTOS.h>  //เรียกใช้คลังโปรแกรมสำหรับเรียกฟังก์ชัน Arduino_FreeRTOS.h
#define R_LED 3                //กำหนด PIN3 สำหรับแสดงแอลอีดี RED
#define G_LED 4                //กำหนด PIN4 สำหรับแสดงแอลอีดี GREEN
#define Y_LED 5                //กำหนด PIN5 สำหรับแสดงแอลอีดี YELLOW
#define O_LED 6                //กำหนด PIN5 สำหรับแสดงแอลอีดี ORANGE
#define PUSH_BTN 2             //กำหนด PIN2 สำหรับรับค่าจากการกดปุ่มสวิตช์
#define POTEN 14               //กำหนด PIN14 สำหรับรับค่าจาก Potentiometer
uint8_t mode = 0;              //สร้างตัวแปร mode เพื่อกำหนดโหมดการแสดงผลของแอลอีดี
unsigned int delay_time = 500; //ประกาศตัวแปร delay_time เป็นหน่วงเวลา 500ms ในการติด-ดับแอลอีดี

void setup() {
//สร้าง Task 3 ฟังก์ชัน
  xTaskCreate(Read_Button, "READ BUTTON", 128, NULL, 1, NULL);
  xTaskCreate(Read_Poten, "READ POTENTIOMETER", 128, NULL, 1, NULL);
  xTaskCreate(Display_LED, "DISPLAY LED", 128, NULL, 1, NULL);
}
//สร้าง Task Display_LED (Red, Green, Yellow, Orange LED)
//--เพื่อแสดง OUTPUT ของแอลอีดี
void Display_LED(void *pvParameter){
  pinMode(R_LED, OUTPUT); //กำหนด PINMODE R_LED เป็น OUTPUT
  pinMode(G_LED, OUTPUT); //กำหนด PINMODE G_LED เป็น OUTPUT
  pinMode(Y_LED, OUTPUT); //กำหนด PINMODE Y_LED เป็น OUTPUT
  pinMode(O_LED, OUTPUT); //กำหนด PINMODE O_LED เป็น OUTPUT

  while(1){         //มีการวนลูป
    if(mode == 0){  //ตรวจสอบว่าค่าในตัวแปร mode เท่ากับ 0 หรือไม่ ?
      //ถ้าค่าในตัวแปร mode เท่ากับ 0 
      //--แอลอีดีสีแดง, สีเขียว, สีเหลือง และสีส้ม สว่างตามลำดับ
      //--โดยเปลี่ยนภาวะการแสดงผลของแอลอีดีทุกๆ 500ms
      //--และเข้าสู่ภาวะที่ไม่มีแอลอีดีดวงใดติดเลย และเป็นเช่นนี้ไปเรื่อยๆ
      digitalWrite(R_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 0);
      digitalWrite(G_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 0);
      digitalWrite(Y_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 0);
      digitalWrite(O_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));

    }else if(mode == 1){  //ตรวจสอบว่าค่าในตัวแปร mode เท่ากับ 1 หรือไม่ ?
      //ถ้าค่าในตัวแปร mode เท่ากับ 1
      //--แอลอีดีสีส้ม, สีเหลือง, สีเขียว และสีแดง สว่างตามลำดับ
      //--โดยเปลี่ยนภาวะการแสดงผลของแอลอีดีทุกๆ 500ms
      //--และเข้าสู่ภาวะที่ไม่มีแอลอีดีดวงใดติดเลย และเป็นเช่นนี้ไปเรื่อยๆ  
      digitalWrite(O_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 0);
      digitalWrite(Y_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 0);
      digitalWrite(G_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 0);
      digitalWrite(R_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));

    }else if(mode == 2){  //ตรวจสอบว่าค่าในตัวแปร mode เท่ากับ 2 หรือไม่ ?
      //ถ้าค่าในตัวแปร mode เท่ากับ 2
      //--เริ่มเข้าสู่ภาวะที่ไม่มีแอลอีดีติดเลย
      //--และแอลอีดีสีแดงติด, แอลอีดีสีแดงและสีเขียวติดพร้อมกัน
      //--แอลอีดีสีแดง เหลือง เขียว ติดพร้อมกัน, แอลอีดีสีแดง เขียว เหลือง ส้ม ติดพร้อมกัน
      //--แอลอีดีสีแดง เขียว เหลือง ติดพร้อมกัน, แอลอีดีสีแดงและสีเขียวติดพร้อมกัน
      //--แอลอีดีสีแดงติดเพียงดวงเดียว ตามลำดับ เป็นเช่นนี้ไปเรื่อยๆ
      digitalWrite(R_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));

    }else if(mode == 3){  //ตรวจสอบว่าค่าในตัวแปร mode เท่ากับ 3 หรือไม่ ?
      //ถ้าค่าในตัวแปร mode เท่ากับ 3
      //--เริ่มเข้าสู่ภาวะที่ไม่มีแอลอีดีติดเลย
      //--และแอลอีดีสีส้มติด, แอลอีดีสีส้มและสีเหลืองติดพร้อมกัน
      //--แอลอีดีสีส้ม เหลือง เขียว ติดพร้อมกัน, แอลอีดีสีส้ม เหลือง เขียว แดง ติดพร้อมกัน
      //--แอลอีดีสีส้ม เหลือง เขียว ติดพร้อมกัน, แอลอีดีสีส้มและสีเหลืองติดพร้อมกัน
      //--แอลอีดีสีส้มติดเพียงดวงเดียว ตามลำดับ เป็นเช่นนี้ไปเรื่อยๆ 
      digitalWrite(O_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
    }
  }
}

void Read_Poten(void *pvParameter){ 
//สร้าง Task Read_Poten สำหรับอ่านค่า Potentiometer
  pinMode(POTEN, INPUT);  //กำหนด PINMODE POTEN เป็น INPUT
  while(1){               //มีการวนลูป
    int val = analogRead(POTEN); 
    //ค่าที่อ่านได้จาก Potentiometer มีทั้งหมด 10 เงื่อไข
    //--โดยในแต่ละเงื่อนไขจะ set ค่า delay ของการติด-ดับของแอลอีดี
    if(val > 925)
      delay_time = 1500;
    else if(val > 826)
      delay_time = 1355;
    else if(val > 727)
      delay_time = 1210;
    else if(val > 628)
      delay_time = 1065;
    else if(val > 529)
      delay_time = 920;
    else if(val > 430)
      delay_time = 775;
    else if(val > 331)
      delay_time = 630;
    else if(val > 232)
      delay_time = 485;
    else if(val > 133)
      delay_time = 340;
    else if(val > 34)
      delay_time = 195;
    else if(val <= 34)
      delay_time = 50;
  }
}

void Read_Button(void *pvParameter){ //สร้าง Task Read_Button สำหรับอ่านค่าการกดปุ่มสวิตช์
  pinMode(PUSH_BTN, INPUT); //กำหนด PINMODE PUSH_BTN เป็น INPUT
  while(1){
    if(!digitalRead(PUSH_BTN)){ //ตรวจสอบว่ามีการกดปุ่มบนสวิตช์หรือไม่ ?
      vTaskDelay(pdMS_TO_TICKS(10));  
      if(!digitalRead(PUSH_BTN)){
        while(!digitalRead(PUSH_BTN)){};
        mode < 3 ? mode++ : mode = 0; 
      }
    }
  }
}
void loop() {} 