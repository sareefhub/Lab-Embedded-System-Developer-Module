#include <avr/io.h>
#include <Arduino.h>

const int segmentPins[7] = {14, 15, 16, 17, 18, 19, 12}; // Segment pins: A, B, C, D, E, F, G
const int dipSwitchPins[4] = {8, 9, 10, 11}; // Dip-Switch input pins

void setup() {
    // Set Dip-Switch pins as inputs with pull-up resistors
    for (int i = 0; i < 4; i++) {
        pinMode(dipSwitchPins[i], INPUT_PULLUP);
    }
    
    // Set segment pins as outputs
    for (int i = 0; i < 7; i++) {
        pinMode(segmentPins[i], OUTPUT);
    }
}

void loop() {
    int value = 0; // Default value is 0

    // Check if any of the Dip-Switches are pressed
    for (int i = 0; i < 4; i++) {
        if (digitalRead(dipSwitchPins[i]) == LOW) {
            value |= (1 << i);
        }
    }

    display7Segment(value);
    delay(100); // Add a small delay to debounce the switches
}

void display7Segment(int value) {
    static const byte segmentMap[16] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111, // 9
        0b01110111, // A (10)
        0b01111100, // b (11)
        0b00111001, // C (12)
        0b01011110, // d (13)
        0b01111001, // E (14)
        0b01110001  // F (15)
    };
    
    byte segments = segmentMap[value & 0x0F]; // Mask to ensure value is within 0-15 range
    
    for (int i = 0; i < 7; i++) {
        digitalWrite(segmentPins[i], (segments >> i) & 0x01);
    }
}
