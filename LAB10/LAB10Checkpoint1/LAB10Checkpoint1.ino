#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,10,11,12,13); // RS,E,D4,D5,D6,D7
int analog;
float voltage;
void setup() {
 lcd.begin(16,2); // 16 chars on 2 lines
 lcd.clear();
 pinMode(14, INPUT);
}
void loop() {
 analog = analogRead(14);
 //voltage = analog * (5.0/1023.0);
 voltage =(analog /1023.00)*5;
 lcd.setCursor(0, 0);
 lcd.print(" Display Analog ");
 lcd.setCursor(0, 1);
 lcd.print("VPoten = ");
 lcd.print(voltage);
 lcd.print(" V.");
 delay(500);
 lcd.clear();
}