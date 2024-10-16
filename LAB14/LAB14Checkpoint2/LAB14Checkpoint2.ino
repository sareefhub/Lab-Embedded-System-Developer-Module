#include <LiquidCrystal_I2C.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>
#include <TimeLib.h>
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
LiquidCrystal_I2C lcd(0x3f, 16, 2); //Module IIC/I2C Interface บางรุFนอาจจะใชN 0x3f
Keypad_I2C keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);
tmElements_t tm; //for DS1307
int Hour, Minute;

void print2digit(int val){
  if(val < 10 && val >= 0)
     lcd.print(0);
  lcd.print(val);
}

void setup()
{
 Serial.begin(38400);
 Wire.begin();
 keypad.begin();
 lcd.init();
 lcd.backlight(); // เปิด backlight
 lcd.home(); 
 
}
unsigned int digit;
bool flag = false;
bool blink_tx = false;
void loop()
{
  char key = keypad.getKey();
    if(key == 'B'){
      Serial.println("B");
      digit = !digit;
      flag = true;
    }else if(key == 'C'){
      if(!digit)
        Hour < 23 ? Hour++ : Hour = 0;
      else
        Minute < 59 ? Minute++ : Minute = 0;
    }else if(key == 'D'){
      if(!digit)
        Hour > 0 ? Hour-- : Hour = 23;
      else
        Minute > 0 ? Minute-- : Minute = 59;
    }else if(key == '#'){
      tm.Hour = Hour;
      tm.Minute = Minute;
      RTC.write(tm);
      flag = false;
    }
  if(RTC.read(tm)){
    lcd.setCursor(0,0);
    lcd.print("D/M/Y= ");
    lcd.print(tm.Day);
    lcd.print("/");
    print2digit(tm.Month);
    lcd.print("/");
    lcd.print(tmYearToCalendar(tm.Year));
    lcd.setCursor(0, 1);
    lcd.print("Time = ");
    if(flag){
      if((millis() - last_time) > 1000){
        blink_tx = !blink_tx;
      }
      if(!digit){
        if(blink_tx)
          lcd.print("  ");
        else
          print2digit(Hour);
        lcd.print(":");
        print2digit(Minute);
      }else{
        print2digit(Hour);
        lcd.print(":");
        if(blink_tx)
          lcd.print("  ");
        else
          print2digit(Minute);
      }
    }else{
      Hour = tm.Hour;
      Minute = tm.Minute;
      print2digit(Hour);
      lcd.print(":");
      print2digit(Minute);
    }
  }
}
