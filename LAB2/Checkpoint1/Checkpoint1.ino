const int segmentA = 14;
const int segmentB = 15;
const int segmentC = 16;
const int segmentD = 17;
const int segmentE = 18;
const int segmentF = 19;
const int segmentG = 8;
const int led1 = 9;   
const int led2 = 10;  
const int led3 = 11;  
const int led4 = 12;  

const int sw0 = 0;
const int sw7 = 7;

void display_0()
{
  digitalWrite(segmentA, HIGH);   digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH);   digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH);   digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, LOW);
}

void display_1()
{
  digitalWrite(segmentA, LOW);    digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH);   digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, LOW);    digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, LOW);
}

void display_2()
{
  digitalWrite(segmentA, HIGH);   digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, LOW);    digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH);   digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, HIGH);
}

void display_3()
{
  digitalWrite(segmentA, HIGH);   digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH);   digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, LOW);    digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, HIGH);
}

void display_4()
{
  digitalWrite(segmentA, LOW);    digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH);   digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, LOW);    digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_5()
{
  digitalWrite(segmentA, HIGH);   digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, HIGH);   digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, LOW);    digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_6()
{
  digitalWrite(segmentA, HIGH);   digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, HIGH);   digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH);   digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_7()
{
  digitalWrite(segmentA, HIGH);   digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH);   digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, LOW);    digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, LOW);
}

void display_8()
{
  digitalWrite(segmentA, HIGH);   digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH);   digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH);   digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_9()
{
  digitalWrite(segmentA, HIGH);   digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH);   digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, LOW);    digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

int readSwitch1()
{
  int sw1_input;
  sw1_input = 0;
  for(int i=0; i<=4; i++)
  {
    switch(i)
    {
     case 0: sw1_input += digitalRead(i) * 1; break;
     case 1: sw1_input += digitalRead(i) * 2; break;
     case 2: sw1_input += digitalRead(i) * 4; break;
     case 3: sw1_input += digitalRead(i) * 8; break;
    }
  }
  return sw1_input;
}

int readSwitch2()
{
  int sw2_input;
  sw2_input = 0;
  for(int i=4; i<=8; i++)
  {
    switch(i - 4)
    {
      case 0: sw2_input += digitalRead(i) * 1; break;
      case 1: sw2_input += digitalRead(i) * 2; break;
      case 2: sw2_input += digitalRead(i) * 4; break;
      case 3: sw2_input += digitalRead(i) * 8; break;
    }
  }
  return sw2_input;
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
  }
}
void setup() {
  
  for (int i=sw0; i <= sw7; i++)
  {
    pinMode(i, INPUT);
  }
  for (int i=segmentA; i<=segmentF; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(segmentG, OUTPUT);

  for (int i=led1; i<=led4; i++) 
  {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  
  int sw1, sw2;   
  int count;
  sw1 = readSwitch1(); 
  sw2 = readSwitch2();
  count = sw1 * sw2; 
  digitalWrite(led1, HIGH);  
  display7Segment(count%10);
  delay(5);
  digitalWrite(led1, LOW); 
  if(count >= 10) 
  {
    digitalWrite(led2, HIGH); 
    display7Segment((count/10)%10);
    delay(5);
    digitalWrite(led2, LOW);
  }
  if(count >= 100)  
  {
    digitalWrite(led3, HIGH);  
    display7Segment((count/100));
    delay(5);
    digitalWrite(led3, LOW);
  }
  
}