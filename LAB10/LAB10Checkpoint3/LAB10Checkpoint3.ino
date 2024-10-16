#include <LiquidCrystal.h>
byte heart[8] = {  // Array of bytes
                   // B stands for binary
                   //formatter and the five
  B00000,          //numbers are the pixels
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};
byte smile[8] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};
byte lock[8] = {
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};
byte character[8] = {
  B11111,
  B10101,
  B11111,
  B01010,
  B01110,
  B11111,
  B01110,
  B01110
};

LiquidCrystal lcd(1, 2, 4, 5, 6, 7);  // Creates an LC object.Parameters : (rs, enable, d4, d5, d6,d7) 
                        
void setup() {
  lcd.begin(16, 2);  // Initializes the interface to the LCD screen, and specifies the dimensions(width and height) of the display
  lcd.createChar(0, heart);  // Create a custom character
  lcd.createChar(1, smile);
  lcd.createChar(2, lock);
  lcd.createChar(3, character);

  // Clears the LCD screen
  lcd.clear();

  // Print a message to the LCD
  lcd.print("Custom Character");
}
void loop() {
  lcd.setCursor(1, 1);
  lcd.write(byte(0));  // Display the custom character 0, the heart
  lcd.setCursor(5, 1);
  lcd.write(byte(1));
  lcd.setCursor(9, 1);
  lcd.write(byte(2));
  lcd.setCursor(13, 1);
  lcd.write(byte(3));
}