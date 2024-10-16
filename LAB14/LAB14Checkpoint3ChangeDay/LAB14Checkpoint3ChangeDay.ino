#include <LiquidCrystal_I2C.h>
#include <Keypad_I2C.h> //นำ KEYPAD มาใช้ในรูปแบบ I2C
#include <Keypad.h>
#include <Wire.h>
#include <TimeLib.h>  //TimeLibary 
#include <DS1307RTC.h>

#define I2CADDR 0x20  
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};

byte rowPins[ROWS] = {7, 6, 5, 4};
byte colPins[COLS] = {3, 2, 1, 0};
unsigned long last_time;
bool blink_txt;
LiquidCrystal_I2C lcd(0x3f, 16, 2); //Module IIC/I2C Interface บางรุ่นอาจจะใช้ N 0x3f
Keypad_I2C keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);
tmElements_t tm; //for DS1307
int Hour, Minute;
int Year, Month, Day;

const char* dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void print2digit(int val){
  if(val < 10 && val >= 0)
     lcd.print(0);
  lcd.print(val);
}

unsigned int mode, digit, date; //digit เอาไว้ check ว่ากดเลือก digit ไหน
void setup()
{
 Serial.begin(38400);
 Wire.begin();
 keypad.begin();
 lcd.init();
 lcd.backlight(); // เปิด backlight
 lcd.home(); 
 date = 0;
}

bool flag = false;  //ตรวจสอบว่ามีการแก้ไขหรือไม่
                    //flag = false ก็ต่อเมื่อกลับไปเป็นโหมดปกติ
bool blink_tx = false;

void loop()
{
  if(RTC.read(tm)){ //ตรวจสอบว่ามีการกดปุ่ม A หรือ B
    char key = keypad.getKey();
    if(key == 'A'){
      date < 2 ? date++ : date = 0;
      mode = 0;
      flag = true;
    }else if(key == 'B'){
      digit = !digit;
      mode = 1;
      flag = true;
    }
    else if(key == 'C'){
      if(!mode){
        if(date == 0)
          Day < 31 ? Day++ : Day = 1;  // Modified to start from 1
        else if(date == 1)
          Month < 12 ? Month++ : Month = 1;  // Modified to start from 1
        else if(date == 2)
          Year < 2099 ? Year++ : Year = 2000;  // Modified to start from 2000
      }else{
        Serial.println("Time config");
        if(!digit)
          Hour < 23 ? Hour++ : Hour = 0;
        else
          Minute < 59 ? Minute++ : Minute = 0;
      }
    }else if(key == 'D'){
      if(!mode){
        if(date == 0)
          Day > 1 ? Day-- : Day = 31;  // Modified to wrap to 31
        else if(date == 1)
          Month > 1 ? Month-- : Month = 12;  // Modified to wrap to 12
        else if(date == 2)
          Year > 2000 ? Year-- : Year = 2099;  // Modified to wrap to 2099
      }else{
        if(!digit)
          Hour > 0 ? Hour-- : Hour = 23;
        else
          Minute > 0 ? Minute-- : Minute = 59;
      }
    }else if(key == '#'){
      tm.Day = Day;
      tm.Month = Month;
      tm.Year = CalendarYrToTm(Year);
      tm.Hour = Hour;
      tm.Minute = Minute;
      RTC.write(tm);
      flag = false;
    }
    
    lcd.setCursor(0,0);
    lcd.print("D/M/Y=");
    if(flag && mode == 0){
      if((millis() - last_time) > 1000){
        blink_tx = !blink_tx;
      }
      if(date == 0){
        blink_tx ? (void)lcd.print("  ") : print2digit(Day);
        lcd.print("/");
        print2digit(Month);
        lcd.print("/");
        lcd.print(Year);
      }else if(date == 1){
        print2digit(Day);
        lcd.print("/");
        blink_tx ? (void)lcd.print("  ") : print2digit(Month);
        lcd.print("/");
        lcd.print(Year);
      }else if(date == 2){
        print2digit(Day);
        lcd.print("/");
        print2digit(Month);
        lcd.print("/");
        blink_tx ? lcd.print("    ") : lcd.print(Year);
      }
    }else{
      Day = tm.Day;
      Month = tm.Month;
      Year = tmYearToCalendar(tm.Year);
      lcd.print(Day);
      lcd.print("/");
      print2digit(Month);
      lcd.print("/");
      print2digit(Year);
    }
    
    lcd.setCursor(0, 1);
    lcd.print("Day = ");
    lcd.print(dayNames[calculateDayOfWeek(Day, Month, Year) - 1]);  // Display the abbreviated day name
  }
}

int calculateDayOfWeek(int d, int m, int y) {
  // Using Zeller's Congruence algorithm
  if (m < 3) {
    m += 12;
    y -= 1;
  }
  int K = y % 100;
  int J = y / 100;
  int f = d + ((13 * (m + 1)) / 5) + K + (K / 4) + (J / 4) + (5 * J);
  return (f % 7) + 1; // Day of week (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
}
