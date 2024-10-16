#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h> // ใช้ <util/delay.h> สำหรับการหน่วงเวลา
#include <stdio.h> // ใช้ <stdio.h> สำหรับ sprintf และ strlen

#define USART0_BAUDRATE 9600
#define UBRR0_VALUE (((F_CPU / (USART0_BAUDRATE * 16UL))) - 1)
#define ADC0 0

// ฟังก์ชันอ่านค่าจาก ADC จากช่องที่กำหนด
unsigned int ADC_read(unsigned char a)
{
    ADMUX = (ADMUX & 0xF0) | (a & 0x0F); // ตั้งค่าช่อง ADC
    ADCSRA |= (1<<ADSC); // เริ่มการแปลง
    while (!(ADCSRA & (1<<ADIF))); // รอจนกว่าการแปลงจะเสร็จสิ้น
    ADCSRA |= (1<<ADIF); // ล้าง flag การแปลง
    return ADC; // ส่งคืนค่า ADC
}

// ฟังก์ชันเริ่มต้นโมดูล USART0
void init_USART0_module(void)
{
    UBRR0H = (uint8_t)(UBRR0_VALUE >> 8); // ตั้งค่า baud rate
    UBRR0L = (uint8_t)UBRR0_VALUE;
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // ข้อมูลขนาด 8 บิต
    UCSR0B = (1<<TXEN0); // เปิดใช้งานตัวส่ง
}

// ส่งข้อมูล 1 byte ผ่าน USART0
void USART0_Send_1_Byte(uint8_t data) 
{
    while (!(UCSR0A & (1 << UDRE0))); // รอให้บัฟเฟอร์ส่งว่าง
    UDR0 = data; // ส่งข้อมูล
}

// ส่งอักขระ newline ผ่าน USART0
void USART0_print_newline(void) 
{
    USART0_Send_1_Byte('\r');
    USART0_Send_1_Byte('\n');
}

int main(void)
{
    float volt; // ตัวแปรเก็บค่าแรงดันไฟฟ้า
    uint16_t Vadc0; // ตัวแปรเก็บค่า ADC ที่อ่านได้
    char msg[32]; // ตัวแปรเก็บข้อความ

    DDRC = 0x00; // ตั้งค่า PORTC เป็น input (ขา ADC)
    ADMUX = (1<<REFS0); // ใช้ AVcc เป็นแหล่งอ้างอิง
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1); // เปิดใช้งาน ADC และตั้งค่า prescaler เป็น 64
    init_USART0_module(); // เรียกฟังก์ชันเริ่มต้นโมดูล USART0

    while(1)
    {
        Vadc0 = ADC_read(ADC0); // อ่านค่าจาก ADC ช่อง 0
        volt = Vadc0 * (5.0 / 1023.0); // แปลงค่าจาก ADC เป็นแรงดันไฟฟ้า

        // จัดรูปแบบค่า float ด้วยมือ
        int volt_int = (int)volt; // ส่วนจำนวนเต็มของแรงดันไฟฟ้า
        int volt_frac = (int)((volt - volt_int) * 100); // ส่วนทศนิยมสองตำแหน่งของแรงดันไฟฟ้า

        sprintf(msg, "Voltage = %d.%02d V.", volt_int, volt_frac); // จัดรูปแบบข้อความแรงดันไฟฟ้า
        for (int i = 0; msg[i] != '\0'; i++)
            USART0_Send_1_Byte(msg[i]); // ส่งข้อความผ่าน USART
        USART0_print_newline(); // ส่ง newline
        _delay_ms(100); // หน่วงเวลา 1 วินาที
    }
}
