#define Y_LED 0   //หลอดแอลอีดีสีเหลือง ต่อกับ GPIO0
#define R_LED 15  //หลอดแอลอีดีสีแดง ต่อกับ GPIO15
#define G_LED 2   //หลอดแอลอีดีสีเขียว ต่อกับ GPIO2
#define DIP12 12  //ดิปสวิตช์ บิตที่ 5 ต่อกับ GPI12
#define DIP13 13  //ดิปสวิตช์ บิตที่ 6 ต่อกับ GPI13
#define DIP5  5   //ดิปสวิตช์ บิตที่ 7 ต่อกับ GPIO5
#define DIP16 16  //ดิปสวิตช์ บิตที่ 8 ต่อกับ GPI16

void setup() {
  pinMode(Y_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(DIP12, INPUT);
  pinMode(DIP13, INPUT);
  pinMode(DIP5, INPUT);
  pinMode(DIP16, INPUT);
}

unsigned long prevTime;
bool led = false;

void loop() {
  uint8_t mode = 0;
  mode = (digitalRead(DIP12) * 1) + (digitalRead(DIP13) * 2) + (digitalRead(DIP5) * 4);

  if(digitalRead(DIP16)) {
    if((millis() - prevTime) > 1000){
      prevTime = millis();
      led = !led;
    }
    digitalWrite(R_LED, led);
    digitalWrite(Y_LED, LOW);
    digitalWrite(G_LED, LOW);
  } else if(mode == 0) {
    digitalWrite(Y_LED, LOW);
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, LOW);
  } else if(mode == 1) {
    digitalWrite(Y_LED, HIGH);
    digitalWrite(G_LED, LOW);
    digitalWrite(R_LED, HIGH);
  } else if(mode == 2) {
    digitalWrite(Y_LED, LOW);
    digitalWrite(G_LED, HIGH);
    digitalWrite(R_LED, HIGH);
  } else if(mode == 3) {
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, HIGH);
    digitalWrite(Y_LED, HIGH);
  } else if(mode == 4) {
    if((millis() - prevTime) > 1000){
      prevTime = millis();
      led = !led;
    }
    digitalWrite(Y_LED, led);
    digitalWrite(R_LED, led);
    digitalWrite(G_LED, led);
  }
}
