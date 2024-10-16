#include <LiquidCrystal.h>

byte dinosaurGround[8] = {
  B00000,
  B00100,
  B01110,
  B01110,
  B11110,
  B01100,
  B01010,
  B01010
};

byte dinosaurJump[8] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B01110,
  B11110,
  B01100,
  B01010
};

byte cactus[8] = {
  B00100,
  B00100,
  B10100,
  B10100,
  B10100,
  B11111,
  B00100,
  B00100
};

LiquidCrystal lcd(1, 2, 4, 5, 6, 7);  // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

void setup() {
  lcd.begin(16, 2);  // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  lcd.createChar(0, dinosaurGround);  // Create custom character for dinosaur on the ground
  lcd.createChar(1, dinosaurJump);    // Create custom character for dinosaur jumping
  lcd.createChar(2, cactus);          // Create custom character for cactus

  // Clears the LCD screen
  lcd.clear();
}

void loop() {
  // Dinosaur on the ground
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.write(byte(0));  // Display the dinosaur on the ground
  lcd.setCursor(5, 1);
  lcd.write(byte(2));  // Display the cactus
  delay(500);

  // Dinosaur jumps
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(1));  // Display the dinosaur jumping
  lcd.setCursor(5, 1);
  lcd.write(byte(2));  // Display the cactus
  delay(500);
}
