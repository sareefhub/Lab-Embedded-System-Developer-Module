#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char TB7SEG[] = {0b00111111, // 0
                          0b00000110, // 1
                          0b01011011, // 2
                          0b01001111, // 3
                          0b01100110, // 4
                          0b01101101, // 5
                          0b01111101, // 6
                          0b00000111, // 7
                          0b01111111, // 8
                          0b01101111, // 9
                          0b01110111, // A
                          0b01111100, // b
                          0b00111001, // C
                          0b01011110, // d
                          0b01111001, // E
                          0b01110001  // F
};

void initial_read_sw_and_display(void) 
{
    unsigned char a;
    a = PINB & 0x0F; // Read PB0-PB3
    PORTC = (TB7SEG[a] & 0x3F); // Send a-f to PC0-PC5
    if (TB7SEG[a] & 0x40) { // Check if segment g should be on
        PORTB |= (1 << PB4); // Turn on segment g
    } else {
        PORTB &= ~(1 << PB4); // Turn off segment g
    }
}

int main(void)
{
    sei(); // Enable global interrupts
    DDRB = 0x10; // Set PB4 as output, PB0-PB3 as input
    DDRC = 0x3F; // Set PC0-PC5 as output

    // Enable pin change interrupt on PB0-PB3
    PCICR = (1 << PCIE0);
    PCMSK0 = (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);
    
    initial_read_sw_and_display();

    while(1)
    {
        // Main loop
    }
}

ISR(PCINT0_vect)
{
    unsigned char a;
    a = PINB & 0x0F; // Read PB0-PB3
    PORTC = (TB7SEG[a] & 0x3F); // Send a-f to PC0-PC5
    if (TB7SEG[a] & 0x40) { // Check if segment g should be on
        PORTB |= (1 << PB4); // Turn on segment g
    } else {
        PORTB &= ~(1 << PB4); // Turn off segment g
    }
}
