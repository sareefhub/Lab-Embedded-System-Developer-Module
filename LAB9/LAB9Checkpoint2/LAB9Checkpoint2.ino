#include <avr/wdt.h>
int count;

void display_led(int count) {
  switch(count) {
    case 0 : PORTB = 0; PORTC = 0; break;
    case 1 : PORTB = 0x01; PORTC = 0; break;
    case 2 : PORTB = 0x03; PORTC = 0; break;
    case 3 : PORTB = 0x07; PORTC = 0; break;
    case 4 : PORTB = 0x0F; PORTC = 0; break;
    case 5 : PORTB = 0x1F; PORTC = 0; break;
    case 6 : PORTB = 0x3F; PORTC = 0; break;
    case 7 : PORTB = 0x3F; PORTC = 0b00000001; break;
    case 8 : PORTB = 0x3F; PORTC = 0b00000011; break;
  }
}

void wait_until_sw_pressed() {
  bool currentState;
  bool lastState = HIGH;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50; // Debounce delay time in milliseconds

  // Wait for the button to be pressed
  while (true) {
    currentState = digitalRead(2);
    if (currentState != lastState) {
      lastDebounceTime = millis(); // Reset the debounce timer
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (currentState == LOW) {
        delay(50); // Additional delay to ensure LED stability
        break; // Button is pressed, exit the loop
      }
    }

    lastState = currentState;
  }

  // Wait for the button to be released
  while (digitalRead(2) == LOW) {
    // Do nothing, just wait for release
    wdt_reset();
  }
}

void setup() {
  // put your setup code here, to run once:
  DDRB = 0xFF;
  DDRC = 0b00000011; // Set PC0 and PC1 as output
  count = 0;
  WDTCSR = (1<<WDE | 1<<WDCE);
  WDTCSR = (0<<WDP0 | 0<<WDP1 | 0<<WDP2 | 1<<WDP3 | 1<<WDE); // 4S
}

void loop() {
  // put your main code here, to run repeatedly:
  display_led(count);
  wait_until_sw_pressed();
  if(count < 8)
    count++;
  else
    count = 0;
}
