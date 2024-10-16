const int segmentA = 14;
const int segmentB = 15;
const int segmentC = 16;
const int segmentD = 17;
const int segmentE = 18;
const int segmentF = 19;
const int segmentG = 8;
const int sw0 = 0;
const int sw7 = 7;
const int LED1 = 9;
const int LED2 = 10;
const int LED3 = 11;
int up;
int down;
int diff;

void display_0()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,LOW);
}

void display_1()
{
  digitalWrite(segmentA,LOW);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,LOW);
  digitalWrite(segmentE,LOW);  digitalWrite(segmentF,LOW);
  digitalWrite(segmentG,LOW);
}

void display_2()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,LOW);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,LOW);
  digitalWrite(segmentG,HIGH);
}

void display_3()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,LOW);  digitalWrite(segmentF,LOW);
  digitalWrite(segmentG,HIGH);
}

void display_4()
{
  digitalWrite(segmentA,LOW);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,LOW);
  digitalWrite(segmentE,LOW);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,HIGH);
}

void display_5()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,LOW);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,LOW);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,HIGH);
}

void display_6()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,LOW);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,HIGH);
}

void display_7()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,LOW);
  digitalWrite(segmentE,LOW);  digitalWrite(segmentF,LOW);
  digitalWrite(segmentG,LOW);
}

void display_8()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,HIGH);
}

void display_9()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,LOW);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,HIGH);
}

void display_a()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,LOW);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,HIGH);
}

void display_b()
{
  digitalWrite(segmentA,LOW);  digitalWrite(segmentB,LOW);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,HIGH);
}

void display_c()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,LOW);
  digitalWrite(segmentC,LOW);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,LOW);
}

void display_d()
{
  digitalWrite(segmentA,LOW);  digitalWrite(segmentB,HIGH);
  digitalWrite(segmentC,HIGH);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,LOW);
  digitalWrite(segmentG,HIGH);
}

void display_e()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,LOW);
  digitalWrite(segmentC,LOW);  digitalWrite(segmentD,HIGH);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,HIGH);
}

void display_f()
{
  digitalWrite(segmentA,HIGH);  digitalWrite(segmentB,LOW);
  digitalWrite(segmentC,LOW);  digitalWrite(segmentD,LOW);
  digitalWrite(segmentE,HIGH);  digitalWrite(segmentF,HIGH);
  digitalWrite(segmentG,HIGH);
}

int readSwitch()
{
  int count;
  count = 0;
  for(int i = sw0; i<= sw7;i++)
  {
    if(digitalRead(i))
      count++;
  }
  return count;
}

void display7Segment(int value)
{
  switch(value)
  {
    case 0: display_0(); break;
    case 1: display_1(); break;
    case 2: display_2(); break;
    case 3: display_3(); break;
    case 4: display_4(); break;
    case 5: display_5(); break;
    case 6: display_6(); break;
    case 7: display_7(); break;
    case 8: display_8(); break;
    case 9: display_9(); break;
    case 10: display_a(); break;
    case 11: display_b(); break;
    case 12: display_c(); break;
    case 13: display_d(); break;
    case 14: display_e(); break;
    case 15: display_f(); break;
  }
}

void setup() {
  for ( int i = sw0; i<=sw7; i ++)
  {
    pinMode(i,INPUT);
  }
  
  for ( int i = segmentA; i<= segmentF; i++)
  {
    pinMode(i,OUTPUT);
  }
  pinMode(segmentG,OUTPUT);
}
//---------------------------------------------------------------------

void loop() {
  down = PIND;
  up = down >> 4;
  down &= 0x0F;
  up &= 0x0F;
  if(down == up){
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,LOW);
    display7Segment(up);
  }
  else if(up < down){
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    diff = down - up;
    display7Segment(diff);
  }
  else{
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,HIGH);
    diff = up - down;
    display7Segment(diff);
  }
}

