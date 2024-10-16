#include <avr/io.h>
#include <avr/interrupt.h>
ISR(TIMER0_OVF_vect)
{
  TCNT0 = 178;
  PORTB ^= 0x01;
}

int main(void)
{
  DDRB |= 0x01;
  TIMSK0 = (1<<TOIE0);
  TCCR0B = (1<<CS02 | 0<<CS01 | 1<<CS00);
  TCCR0A = 0x00;
  TCNT0 = 178;
  sei();
  while (1)
  {
    
  }
}