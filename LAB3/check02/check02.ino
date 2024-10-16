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

volatile unsigned char count = 0;

void initial_read_sw_and_display(void)  
{
  DDRD = 0xFB;
  DDRB = 0x01;
  PORTD |= (1 << PD2);  // เปิดใช้งาน pull-up resistor ที่ PD2
  DDRC = 0xFF;
  PORTC = (TB7SEG[count]);
    if (TB7SEG[count] & 0x40) { // Check if segment g should be on
      PORTB |= (1 << PB0); // Turn on segment g
  } else {
      PORTB &= ~(1 << PB0); // Turn off segment g
  }
}

int main(void)
{
  sei();
  PCICR = 0x04;
  PCMSK2 = 0x04;

  initial_read_sw_and_display();

  while(1)
  {
    // Main loop
  }
}

ISR(PCINT2_vect)
{
  if(!(PIND & (1 << PD2))) {
    count++;
    if (count > 9){
      count = 0;
    }
    PORTC = TB7SEG[count];
    if (TB7SEG[count] & 0x40) { // Check if segment g should be on
      PORTB |= (1 << PB0); // Turn on segment g
  } else {
      PORTB &= ~(1 << PB0); // Turn off segment g
  }
  }
}