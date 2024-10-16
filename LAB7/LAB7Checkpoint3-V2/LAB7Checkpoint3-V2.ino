#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define USART0_BAUDRATE 19200
#define UBRR0_VALUE (((F_CPU/(USART0_BAUDRATE * 16UL)))-1)
#define ADC0 0
#define ADC2 2

uint8_t adc_ip;

void init_USART0_module(void) {
    UBRR0H = (uint8_t)(UBRR0_VALUE >> 8);
    UBRR0L = (uint8_t) UBRR0_VALUE;
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
    UCSR0B |= (1 << RXCIE0);
}

ISR(USART_RX_vect) {
    switch (UDR0) {
        case '1': adc_ip = 1; break;
        case '2': adc_ip = 2; break;
        case 't':
        case 'T': adc_ip = (adc_ip == 1) ? 2 : 1; break;
    }
}

uint16_t ADC_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void USART0_Send_1_Byte(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void USART0_print_newline(void) {
    USART0_Send_1_Byte('\r');
    USART0_Send_1_Byte('\n');
}

int main(void) { 
    adc_ip = 1;
    float volt;
    uint16_t Vadc;
    char msg[32];
    DDRC = 0x00;
    ADMUX = 0b01000101;
    ADCSRA = 0x87;
    init_USART0_module();
    sei();
    while (1) {
        if (adc_ip == 2) {
            Vadc = ADC_read(ADC2);
            volt = Vadc / 1023.00 * 5.00;
            sprintf(msg, "Vin2 = %f", volt);
        } else {
            Vadc = ADC_read(ADC0);
            volt = Vadc / 1023.00 * 5.00;
            sprintf(msg, "Vin1 = %f", volt);
        }
        for (uint8_t i = 0; i < strlen(msg); i++) {
            USART0_Send_1_Byte(msg[i]);
        }
        USART0_print_newline();
        _delay_ms(1000);
    }
}
