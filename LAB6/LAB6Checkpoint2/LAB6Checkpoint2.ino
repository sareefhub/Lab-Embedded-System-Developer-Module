const unsigned int ADC5 = 5;

unsigned int ADC_read(unsigned char a){
  ADCSRA |= (1<<ADSC);
  while (!(ADCSRA & (1<<ADIF)));
  ADCSRA |= 0<<ADIF;
  return ADC;
}

unsigned char TABLE7SEG[]= {
  0b00111111, 0b00000110,
  0b01011011, 0b01001111,
  0b01100110, 0b01101101,
  0b01111101, 0b00000111,
  0b01111111, 0b01101111,
  0b01110111, 0b01111100,
  0b00111001, 0b01011110,
  0b01111001, 0b01110001
};

int counter = 0;
int adcVal,prevADC,DECODED;

void DISPLAY7SEG(signed char a){
  PORTD = TABLE7SEG[a];
};

void setup() {
  DDRD = 0xFF;
  DDRC = 0x00;
  DISPLAY7SEG(0);
  ADMUX = 0b01000101;
  ADCSRA = 0b10000111;
  DECODED = 0;
  //Serial.begin(9600);
};

void loop() {
  unsigned int x = 0;
  counter = 0;
  prevADC = 0;
  do
  {
    x = analogRead(19);
    Serial.println(x);
    adcVal = ADC_read(ADC5);
    Serial.println(adcVal);
    if((abs(adcVal-prevADC)<6)&&(adcVal>33))
      counter ++;
    else
      counter = 0;
    delay(3);
    prevADC = adcVal;
  }while (counter <20 );
  if((adcVal>33)&&(adcVal<66)) DECODED =1;
  else if ((adcVal>71)&&(adcVal<104)) DECODED = 2;
  else if ((adcVal>113)&&(adcVal<146)) DECODED = 3;
  else if ((adcVal>152)&&(adcVal<185)) DECODED = 10;
  else if ((adcVal>180)&&(adcVal<213)) DECODED = 4;
  else if ((adcVal>293)&&(adcVal<326)) DECODED = 5;
  else if ((adcVal>395)&&(adcVal<428)) DECODED = 6;
  else if ((adcVal>472)&&(adcVal<510)) DECODED = 11;
  else if ((adcVal>520)&&(adcVal<565)) DECODED = 7;
  else if ((adcVal>650)&&(adcVal<710)) DECODED = 8;
  else if ((adcVal>766)&&(adcVal<780)) DECODED = 9;
  else if ((adcVal>820)&&(adcVal<830)) DECODED = 12;
  else if ((adcVal>830)&&(adcVal<850)) DECODED = 14;
  else if ((adcVal>860)&&(adcVal<944)) DECODED = 0;
  else if ((adcVal>944)&&(adcVal<960)) DECODED = 15;
  else if ((adcVal>965)&&(adcVal<1023)) DECODED = 13;
  do
  {
    adcVal = ADC_read(ADC5);
  }while(adcVal > 33);
  DISPLAY7SEG(DECODED);
}