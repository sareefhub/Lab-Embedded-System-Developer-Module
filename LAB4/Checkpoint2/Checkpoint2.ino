#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_COMPA_vect)
{
  PORTD ^= (1<<6);  // Toggle PORTD6
}

int main(void)
{
  DDRD |= (1<<6);  // Set PORTD6 as an output
  TIMSK0 = (1<<OCIE0A);
  TCCR0A = (1<<WGM01);
  TCCR0B = (1<<CS02 | 0<<CS01 | 1<<CS00);
  OCR0A = 77;
  sei();
  while (1) 
  {

  }
}
