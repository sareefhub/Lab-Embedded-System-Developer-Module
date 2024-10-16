//Lab11Ch5
const int strobe = 7;
const int clock = 9;
const int data = 8;
volatile unsigned int hour, minute, sec;
volatile bool state = false;  //สร้างตัวแปร stat สำหรับคุมขีด (ture/false)
volatile bool stateH, stateM, stateS; 
//stateH คุมสถานะการกระพริบของ LED hr
//stateM นาที, stateS วินาที
volatile static int digit;  //ทดสอบเพื่อให้ทราบว่าตำแหน่งไหนเกิดการเปลี่ยนแปลง
int count_sec, count_3sec, count_half;


ISR(TIMER1_COMPA_vect){
  
  if(count_sec == 10){ //10 ครั้ง = 1 วิ จะมีการเข้ามาทำ (100 ms) ทำงานครบ 1 รอบยัง
    state = !state;
    Serial.println(state);
    update_time();
    display_7seg();
    count_sec = 0;
  }
  uint8_t mode = readTM1638_Buttons();
  if(mode){
      count_half = 0;
      count_3sec = 1;
      switch(mode){
        case 0x01 : hour < 23 ? hour++ : hour = 0; digit = 1; break;
        case 0x02 : hour > 0 ? hour-- : hour = 23; digit = 1; break;
        case 0x08 : minute < 59 ? minute++ : minute = 0; digit = 2; break;
        case 0x10 : minute > 0 ? minute-- : minute = 59; digit = 2; break;
        case 0x40 : sec = 59; digit = 3; break;
        case 0x80 : sec = 0; digit = 3; break;
      }
      display_7seg();
   }
    
  if((count_3sec > 0) && (count_3sec <= 30)){

    Serial.println(count_3sec); //ทุกๆ 3 วิ
    count_sec = 0;
    count_3sec++;
    if((count_3sec % 10) == 0){
      state = !state;
      Serial.println("Blink");
    }
    if((count_3sec % 5) == 0){
      switch(digit){
        case 1: stateH = !stateH; break;
        case 2: stateM = !stateM; break;
        case 3: stateS = !stateS; break;
      }
    }
    display_7seg();
  }
  else{
    stateH = 1;
    stateM = 1;
    stateS = 1;
    count_3sec = 0;
    count_half = 0;
    count_sec++;
  }
}  
uint8_t readTM1638_Buttons(void)
{
  uint8_t buttons = 0;
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0x42);

  pinMode(data, INPUT);

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(data, clock, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(data, OUTPUT);
  digitalWrite(strobe, HIGH);
  return buttons;
}

void sendCommand(uint8_t value)
{
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}


void reset()
{
  sendCommand(0x40); // set auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
  count_sec = 0;
  count_3sec = 0;
  count_half = 0;
}

void setup()
{
  TCCR1A = 0;
  TCNT1 = 0;
  OCR1A = 6249;
  TCCR1B = (1<<CS12| 0<<CS11 | 0<<CS10 | 1<<WGM12);//prescaler 256
  TIMSK1 = (1 << OCIE1A);
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  Serial.begin(9600);
  hour = 0;
  minute = 0;
  sec = 0;
  sendCommand(0x8f);  // activate
  reset();
  display_7seg();
}

void update_time(){
  if(sec == 59){
    sec = 0;
    if(minute == 59){
      minute = 0;
      hour == 23 ? 0 : hour++;
    }
    else{
      minute++;
    }
  }
  else{
    sec++;
  }
}

void display_7seg()
{
   uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };
   sendCommand(0x44); // set single address
   //Hour
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc0);
   shiftOut(data, clock, LSBFIRST, stateH ? digits[hour/10] : 0);
   digitalWrite(strobe, HIGH);
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc2);
   shiftOut(data, clock, LSBFIRST, stateH ? digits[hour%10] : 0);
   digitalWrite(strobe, HIGH);
   //Seperate
    digitalWrite(strobe, LOW);
    shiftOut(data, clock, LSBFIRST, 0xc4);
    shiftOut(data, clock, LSBFIRST, state ? 0x40 : 0);
    digitalWrite(strobe, HIGH);
   //Min
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc6);
   shiftOut(data, clock, LSBFIRST, stateM ? digits[minute/10] : 0);
   digitalWrite(strobe, HIGH);
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xc8);
   shiftOut(data, clock, LSBFIRST, stateM ? digits[minute%10] : 0);
   digitalWrite(strobe, HIGH);
   //Seperate
     digitalWrite(strobe, LOW);
     shiftOut(data, clock, LSBFIRST, 0xca);
     shiftOut(data, clock, LSBFIRST, state ? 0x40 : 0);
     digitalWrite(strobe, HIGH);
   //Sec
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xcc);
   shiftOut(data, clock, LSBFIRST, stateS ? digits[sec/10] : 0);
   digitalWrite(strobe, HIGH);
   digitalWrite(strobe, LOW);
   shiftOut(data, clock, LSBFIRST, 0xce);
   shiftOut(data, clock, LSBFIRST, stateS ? digits[sec%10] : 0);
   digitalWrite(strobe, HIGH);
}

void loop()
{
}