#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char TEXT[38] = "My name is Sareef. My ID is 6510110115"; // ข้อความที่ต้องการส่ง
unsigned char i;

void USART_Transmit(unsigned char data)
{
  while (!(UCSR0A & (1<<UDRE0))); // รอจนกว่าบัฟเฟอร์ส่งจะว่าง
  UDR0 = data; // ส่งข้อมูล
}

void do_nothing(void)
{
  // ฟังก์ชั่นนี้ตั้งใจให้ว่างเปล่า
}

int main(void)
{
  UCSR0A = 0x00; // รีเซ็ตการตั้งค่า UCSR0A
  UCSR0B = 0x08; // เปิดใช้งานการส่ง
  UCSR0C = 0x06; // ตั้งค่ารูปแบบเฟรม: 8 บิตข้อมูล, 1 บิตหยุด
  UBRR0H = 0; // ตั้งค่าบิตสูงของ Baud rate เป็น 0
  UBRR0L = 103; // ตั้งค่า Baud rate เป็น 9600

  for (i = 0; i < 38; i++) 
  {
    USART_Transmit(TEXT[i]); // ส่งข้อมูลทีละตัวอักษร
  }

  while (1)
  {
    do_nothing(); // ทำงานฟังก์ชั่น do_nothing ในลูปหลัก
  }
}
