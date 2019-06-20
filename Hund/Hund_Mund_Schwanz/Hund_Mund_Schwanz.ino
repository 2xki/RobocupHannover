#include <SoftwareSerial.h>
#include <Servo.h>

Servo mouth;
Servo tail;
SoftwareSerial UnoSerial(D2, D3);   // RX, TX
int input;

void setup() {

  mouth.attach(D5);       // 60 - 130
  tail.attach(D6);        // 50 - 110 - 150
  Serial.begin(115200);
  UnoSerial.begin(115200);
  mouth.write(130);
  tail.write(115);
}

void loop() {
  if (UnoSerial.available() > 0) {
    if (input == 0 || input == 3) {
      input = UnoSerial.parseInt();
    }
    Serial.println(input);
    if (input == 1) {
      input = 0;
      mouth.write(60);
      delay(500);
      mouth.write(130);
    }
    else if (input == 2) {
      input = 0;
      while (true) {
        tail.write(150);
        delay(300);
        tail.write(50);
        delay(500);
        tail.write(115);
        input = UnoSerial.parseInt();
        if (input == 1 || input == 3) {
          tail.write(115);
          break;
        }
      }
    }
  }
  /*if (Serial.available() > 0) {
    input = Serial.parseInt();
    if (input == 1) {
      mouth.write(60);
      delay(500);
      mouth.write(130);
    }
    else if (input == 2) {
      while (true) {
        tail.write(150);
        delay(300);
        tail.write(50);
        delay(500);
        tail.write(115);
        if (Serial.parseInt() == 3) {
          tail.write(115);
          break;
        }
      }
    }
  }*/
}
