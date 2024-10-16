#include <avr/io.h>
#include <avr/interrupt.h>

const int COL = 4;
const int ROW = 4;
const int cols[4] = {7,6,5,4};
const int rows[4] = {11,10,9,8};
const unsigned char keyMap[4][4] = 
{{1, 2, 3, 0xA},
{4, 5, 6, 0xB},
{7, 8, 9, 0xC},
{0xE, 0, 0xF, 0xD}};

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
                          0b01110001}; // F

volatile unsigned char key = 0xFF; // ตัวแปรเก็บค่าจาก Keypad

void initial_read_sw_and_display(void)  
{
  DDRD &= ~(1 << PD3);  // ตั้งค่าให้ PD3 (INT1) เป็น input
  PORTD |= (1 << PD3);  // เปิดใช้งาน pull-up resistor ที่ PD3
  DDRC = 0xFF;          // ตั้งค่าให้ PORTC ทั้งหมดเป็น output
  PORTC = ~TB7SEG[0];   // แสดงผลเริ่มต้นเป็น 0
}


int main(void)
{
  // ตั้งค่า DDRD โดยให้ PD3 เป็น input และ PD0, PD1, PD2, PD4-PD7 เป็น output (0xF7)
  DDRD = 0xF7;

  // เปิดใช้งานการขัดจังหวะ INT1
  EICRA |= (1 << ISC10); // ตั้งค่าให้ INT1 เกิดการขัดจังหวะทั้งขาขึ้นและขาลง
  EIMSK |= (1 << INT1);  // เปิดใช้งานการขัดจังหวะ INT1

  sei(); // เปิดใช้งานการขัดจังหวะทั่วระบบ

  initial_read_sw_and_display();

  while(1)
  {
    // รอการขัดจังหวะ
  }
}

ISR(INT1_vect)
{
  // อ่านค่าจาก Keypad
  for (int r = 0; r < ROW; r++)
  {
    // ตั้งค่าบิตที่ตรงกับแถวเป็น 0 และบิตที่เหลือเป็น 1
    PORTD = ~(1 << rows[r]);
    _delay_ms(1); // รอให้สัญญาณคงที่
    for (int c = 0; c < COL; c++)
    {
      if (!(PIND & (1 << cols[c])))
      {
        key = keyMap[r][c]; // เก็บค่าจาก Keypad
        PORTC = ~TB7SEG[key]; // แสดงผลค่าบน 7-segment
        return;
      }
    }
  }
}
