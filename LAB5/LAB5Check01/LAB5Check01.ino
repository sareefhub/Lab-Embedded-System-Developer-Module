#include <avr/io.h>
#include <avr/interrupt.h>

void do_nothing(void) {}

int main(void)
{
  DDRD = 0xFF;
  DDRB = 0xFF;

  ADMUX = 0b00100000;
  while(1)
  {
    ADCSRA = 0b11000111;
    while (!(ADCSRA & (1 << ADIF))) //set ADIF = 1 For check
      do_nothing();
      
    PORTD = ADCH; // Set
    PORTB = (ADCL>>6); // Set Right 6 bit
  }
}
