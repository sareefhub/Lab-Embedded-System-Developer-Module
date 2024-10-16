#include <avr/io.h>
#include <avr/interrupt.h>
uint8_t count;
ISR(TIMER1_OVF_vect)
{
  TCNT1 = 3036;
  count++;
  if(count==10)
  {
    count=0;
    PORTB ^=0x20;
  }
}
int main(void)
{
  count = 0;
  DDRB = (1<<DDB5);
  PORTB = 0x20;
  TCCR1A = 0x00;
  TCCR1B = (1<<CS12);
  TCNT1 = 3036;
  TIMSK1 = (1<<TOIE1);
  sei();
  while(1)
  {

  }
}