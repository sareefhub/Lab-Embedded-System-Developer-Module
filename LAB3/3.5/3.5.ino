#include_next <avr/io.h>
#include_next <avr/interrupt.h>
#define F_CPU 16000000UL
#include_next <avr/delay.h>

signed char PressedKey;
signed char n;

int main(void)
{
  DDRC = 0b111;
  DDRB = 0b0000;
  DDRD = 0b11110111;
  EICRA = 0b1000;
  EIMSK = 0x02;
  sei()
  PORTB = 0x0F;
  PORTC = 0b000;
  PORTD |= (1<<3);
  Switch_GREEN_LED_off();
  Switch_RED_LED_off();
  while(1)
  {}
}

void Switch_RED_LED_on(void)
{
  PORTD &= 0b11011111;
}

void  Switch_GREEN_LED_on(void)
{
  PORTD &= 0b11101111;
}

void Switch_RED_LED_off(void)
{
  PORTD |= 0b00100000;
}

void Switch_GREEN_LED_off(void)
{
  PORTD |= 0b00010000;
}

void Blink_RED_LED(void)
{
  Switch_RED_LED_on();
  _delay_ms(500);
  Switch_RED_LED_off();
  _delay_ms(500);
}

void Blink_GREEN_LED(void)
{
  Switch_GREEN_LED_on();
  _delay_ms(500);
  Switch_GREEN_LED_off();
  _delay_ms(500);
}

ISR(INT1_vect)
{
  PressedKey = -1;
  n = 1;
  do
  {
    if (n==1)
    {
      PORTC = 0b110;
      _delay_ms(10);
      switch (PINB & 0x0F) 
      {
      case 0b0111:
        PressedKey = 1;break;
      case 0b1011:
        PressedKey = 4;break;
      case 0b1101:
        PressedKey = 7;break;
      case 0b1110:
        PressedKey = 10;break;
      }
    }
    else if (n==2) 
    {
      PORTC = 0b101;
      _delay_ms(10);
      switch (PINB & 0x0F)
      {
      case 0b0111:
        PressedKey = 2;break;
      case 0b1011:
        PressedKey = 5;break;
      case 0b1101:
        PressedKey = 8;break;
      case 0b1110:
        PressedKey = 0;break;
      }
    }
    else if (n==3) 
    {
      PORTC = 0b011;
      _delay_ms(10);
      switch (PINB & 0x0F)
      {
      case 0b0111:
        PressedKey = 3;break;
      case 0b1011:
        PressedKey = 6;break;
      case 0b1101:
        PressedKey = 9;break;
      case 0b1110:
        PressedKey = 11;break;
      }
    }
    n++;
} while (n <= 3);

  if (PressedKey==0)
  {
    for (int i=0;i<5;i++)
      Blink_RED_LED();
  }

  else if (PressedKey != -1)
  {
    for (int i=0; i<PressedKey; i++)
      Blink_GREEN_LED();
    }
    PORTC = 0b000;
  }