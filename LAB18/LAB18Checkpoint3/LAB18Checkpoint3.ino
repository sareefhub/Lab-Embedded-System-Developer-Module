#define Y_LED 0   // หลอดแอลอีดีสีเหลืองต่อกับ GPIO0
#define R_LED 15  // หลอดแอลอีดีสีแดงต่อกับ GPIO15
#define G_LED 2   // หลอดแอลอีดีสีเขียวต่อกับ GPIO2
#define SW1 16    // สวิตช์ต่อกับ GPIO16
#define SW2 5     // สวิตช์ต่อกับ GPIO5
#define SW3 13    // สวิตช์ต่อกับ GPIO13
#define SW4 12    // สวิตช์ต่อกับ GPIO12

#define BLYNK_TEMPLATE_ID "TMPL6AjOdrj81"
#define BLYNK_TEMPLATE_NAME "Checkpoint3"
#define BLYNK_AUTH_TOKEN "21XK0ISBQK4kzlResIvzmOBK7GwQ4wkt"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "iPhone ของ sola";
char pass[] = "////////";

BlynkTimer timer;

BLYNK_CONNECTED() {
  Blynk.setProperty(V1, "offImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V1, "onImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V100, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstartdevice-was-made");
}

void setup() {
  pinMode(Y_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(100L, ReadSW);
}

void loop() {
  Blynk.run();
  timer.run();

  bool s1 = digitalRead(SW1);
  bool s2 = digitalRead(SW2);
  bool s3 = digitalRead(SW3);
  bool s4 = digitalRead(SW4);

  if (s1 == HIGH && s2 == LOW && s3 == LOW && s4 == LOW) {
    digitalWrite(R_LED, HIGH);
    digitalWrite(G_LED, HIGH);
    digitalWrite(Y_LED, HIGH);
    delay(1000);
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, LOW);
    digitalWrite(Y_LED, LOW);
    delay(1000);
  }
  else if (s1 == HIGH && s2 == LOW && s3 == LOW && s4 == HIGH) {
    // สั่งให้ LED สีแดงควบคุมผ่านแอป Blynk
    digitalWrite(G_LED, LOW);
    digitalWrite(Y_LED, LOW);
  }
  else if (s1 == HIGH && s2 == LOW && s3 == HIGH && s4 == LOW) {
    // สั่งให้ LED สีเขียวควบคุมผ่านแอป Blynk
    digitalWrite(R_LED, LOW);
    digitalWrite(Y_LED, LOW);
  }
  else if (s1 == HIGH && s2 == LOW && s3 == HIGH && s4 == HIGH) {
    // สั่งให้ LED สีเหลืองควบคุมผ่านแอป Blynk
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, LOW);
  }
  else if (s1 == LOW && s2 == HIGH && s3 == LOW && s4 == LOW) {
    // สั่งให้ LED ควบคุมผ่านแอป Blynk ทั้งหมด
  }
  else if (s1 == LOW && s2 == HIGH && s3 == LOW && s4 == HIGH) {
    analogWrite(R_LED, 205);
    delay(1000);
    digitalWrite(R_LED, LOW);
    delay(1000);
  }
  else if (s1 == LOW && s2 == HIGH && s3 == HIGH && s4 == LOW) {
    analogWrite(G_LED, 512);
    delay(1000);
    digitalWrite(G_LED, LOW);
    delay(1000);
  }
  else if (s1 == LOW && s2 == HIGH && s3 == HIGH && s4 == HIGH) {
    analogWrite(Y_LED, 716);
    delay(1000);
    digitalWrite(Y_LED, LOW);
    delay(1000);
  }
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  if (digitalRead(SW1) == HIGH && digitalRead(SW2) == LOW && digitalRead(SW3) == LOW && digitalRead(SW4) == HIGH) {
    if (value == 1) {
      digitalWrite(R_LED, HIGH);
    } else {
      digitalWrite(R_LED, LOW);
    }
  }
  else if (digitalRead(SW1) == LOW && digitalRead(SW2) == HIGH && digitalRead(SW3) == LOW && digitalRead(SW4) == LOW) {
    if (value == 1) {
      digitalWrite(R_LED, HIGH);
    } else {
      digitalWrite(R_LED, LOW);
    }
  }
}

BLYNK_WRITE(V1) {
  int value = param.asInt();
  if (digitalRead(SW1) == HIGH && digitalRead(SW2) == LOW && digitalRead(SW3) == HIGH && digitalRead(SW4) == LOW) {
    if (value == 1) {
      digitalWrite(G_LED, HIGH);
    } else {
      digitalWrite(G_LED, LOW);
    }
  }
  else if (digitalRead(SW1) == LOW && digitalRead(SW2) == HIGH && digitalRead(SW3) == LOW && digitalRead(SW4) == LOW) {
    if (value == 1) {
      digitalWrite(G_LED, HIGH);
    } else {
      digitalWrite(G_LED, LOW);
    }
  }
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  if (digitalRead(SW1) == HIGH && digitalRead(SW2) == LOW && digitalRead(SW3) == HIGH && digitalRead(SW4) == HIGH) {
    if (value == 1) {
      digitalWrite(Y_LED, HIGH);
    } else {
      digitalWrite(Y_LED, LOW);
    }
  }
  else if (digitalRead(SW1) == LOW && digitalRead(SW2) == HIGH && digitalRead(SW3) == LOW && digitalRead(SW4) == LOW) {
    if (value == 1) {
      digitalWrite(Y_LED, HIGH);
    } else {
      digitalWrite(Y_LED, LOW);
    }
  }
}

void ReadSW() {
  uint8_t bl1 = digitalRead(SW3);
  uint8_t bl2 = digitalRead(SW4);
  Blynk.virtualWrite(V3, bl1);
  Blynk.virtualWrite(V4, bl2);
}
