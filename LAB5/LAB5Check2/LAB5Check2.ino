#include <avr/io.h>      // รวมไฟล์ header สำหรับการใช้งานอินพุต/เอาต์พุตของ AVR
#define F_CPU 16000000UL // กำหนดความถี่ของ CPU เป็น 16 MHz

int main(void) {
  uint16_t x;           // ประกาศตัวแปร x สำหรับเก็บค่าจาก ADC

  // Disable digital input on ADC0 to ADC5 pins (ลดการใช้พลังงาน)
  DIDR0 = 0x00111111;   
  
  DDRD = 0xFF;          // กำหนดพอร์ต D เป็นเอาต์พุตทั้งหมด
  DDRC = 0x00;          // กำหนดพอร์ต C เป็นอินพุตทั้งหมด

  // กำหนด ADMUX register: 
  // - ใช้ Vcc เป็นแรงดันอ้างอิง (REFS0=1, REFS1=0)
  // - อ่านค่า ADC เป็น 10-bit (ADLAR=0)
  ADMUX = (0<<REFS0)|(0<<REFS1)|(0<<ADLAR); 

  // กำหนด ADCSRA register:
  // - เปิดใช้งาน ADC (ADEN=1)
  // - ตั้ง prescaler เป็น 128 (ADPS2=1, ADPS1=1, ADPS0=1)
  ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 

  while(1) {
    ADCSRA |= (1<<ADSC);    // เริ่มการแปลง ADC
    while(!(ADCSRA & (1<<ADIF))); // รอจนกว่าการแปลงจะเสร็จสิ้น

    x = ADC;  // อ่านค่าจาก ADC
    
    // กำหนดเงื่อนไขสำหรับการแสดงผล
    if(x < 113)
      PORTD = 0xFF;  // ถ้าค่าน้อยกว่า 113, เปิด LED ทั้งหมด
    else if (x < 227)
      PORTD = 0xFE;  // ถ้าค่าน้อยกว่า 227, เปิด LED ตัวสุดท้าย
    else if (x < 342)
      PORTD = 0xFC;  // ถ้าค่าน้อยกว่า 342, เปิด LED สองตัวสุดท้าย
    else if (x < 455)
      PORTD = 0xF8;  // ถ้าค่าน้อยกว่า 455, เปิด LED สามตัวสุดท้าย
    else if (x < 569)
      PORTD = 0xF0;  // ถ้าค่าน้อยกว่า 569, เปิด LED สี่ตัวสุดท้าย
    else if (x < 684)
      PORTD = 0xE0;  // ถ้าค่าน้อยกว่า 684, เปิด LED ห้าตัวสุดท้าย
    else if (x < 799)
      PORTD = 0xC0;  // ถ้าค่าน้อยกว่า 799, เปิด LED หกตัวสุดท้าย
    else if (x < 911)
      PORTD = 0x80;  // ถ้าค่าน้อยกว่า 911, เปิด LED ตัวแรก
    else
      PORTD = 0x00;  // ถ้ามากกว่าหรือเท่ากับ 911, ปิด LED ทั้งหมด

    ADCSRA |= (1<<ADIF); // เคลียร์ flag ของ ADC interrupt เพื่อพร้อมสำหรับการแปลงครั้งถัดไป
  }
}
