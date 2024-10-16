#include <LiquidCrystal.h>
#define echoPin 3
#define trigPin 2
LiquidCrystal lcd(8,9,10,11,12,13); // RS,E,D4,D5,D6,D7
long duration, distance;
void setup() {
 lcd.begin(16,2); // 16 chars on 2 lines
 lcd.clear();
  pinMode(trigPin, OUTPUT); // TrigerPin
  pinMode(echoPin, INPUT); //EchoPin
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm");
  lcd.setCursor(0, 1);
  if(distance > 200){
    lcd.print("Out of Range");
  }
  else if(distance < 5){
    lcd.print("TOO CLOSE!");
  }
  else{
    lcd.print("Range OK");
  }
  delay(500);
 lcd.clear();
}