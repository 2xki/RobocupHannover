#include <SoftwareSerial.h>
SoftwareSerial WemosSerial(3, 2); // RX, TX

#define mL_power 5   // Motor left power
#define mL_dirF 6    // Motor left direction forward
#define mL_dirR 7    // Motor left direction reverse
#define mR_dirF 8    // Motor right direction forward
#define mR_dirR 9    // Motor right direction reverse
#define mR_power 10   // Motor right power

int l = 0, r = 0;
int inputL, inputR;
String rawIn;
long input;
char incomingByte;
void setup() {

  Serial.begin(9600);
  WemosSerial.begin(115200);
  Serial.println("Hi");
  WemosSerial.println("Hi");

  pinMode(mL_power, OUTPUT);
  pinMode(mL_dirF, OUTPUT);
  pinMode(mL_dirR, OUTPUT);
  pinMode(mR_dirF, OUTPUT);
  pinMode(mR_dirR, OUTPUT);
  pinMode(mR_power, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    input = 0;
    while (1) {           // force into a loop until 'n' is received
      incomingByte = Serial.read();
      if (incomingByte == '\n') break;   // exit the while(1), we're done receiving
      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
      input *= 10;  // shift left 1 decimal place
      // convert ASCII to integer, add, and shift left 1 decimal place
      input = ((incomingByte - 48) + input);
    }
    //input = Serial.parseInt();
    Serial.println(input);

    if (input != 1 && input != 2 && input != 3) {   // 1: Bellen, 2: Schwanzwedeln, 3: Schwanzwedeln aus
      if (input == 0) {
        input = 100100;
      }
      if (input != 100100){
        Serial.println("send to Wemos: 1");
        WemosSerial.println(1); // Bellen bei Bewegung
      }
      inputL = input / 1000;
      inputR = input - inputL * 1000;
      l = map(inputL, 0, 200, -255, 255);
      r = map(inputR, 0, 200, -255, 255);


      if (l > 0) {
        digitalWrite(mL_dirF, 1);
        digitalWrite(mL_dirR, 0);
      } else if (l < 0) {
        digitalWrite(mL_dirF, 0);
        digitalWrite(mL_dirR, 1);
      } else {
        digitalWrite(mL_dirF, 0);
        digitalWrite(mL_dirR, 0);
      }
      if (r > 0) {
        digitalWrite(mR_dirF, 1);
        digitalWrite(mR_dirR, 0);
      } else if (l < 0) {
        digitalWrite(mR_dirF, 0);
        digitalWrite(mR_dirR, 1);
      } else {
        digitalWrite(mR_dirF, 0);
        digitalWrite(mR_dirR, 0);
      }
      analogWrite(mL_power, abs(l));
      analogWrite(mR_power, abs(r));

      Serial.println("L in: " + String(inputL) + "\tR in: " + String(inputR) + "\tL: " + String(l) + "\tR: " + String(r) + "\tmL_dirF: " + String(digitalRead(mL_dirF)) + "\tmL_dirR: " + String(digitalRead(mL_dirR)) + "\n");
      //WemosSerial.println("L in: " + String(inputL) + "\tR in: " + String(inputR) + "\tL: " + String(l) + "\tR: " + String(r) + "\tmL_dirF: " + String(digitalRead(mL_dirF)) + "\tmL_dirR: " + String(digitalRead(mL_dirR)) + "\n");
    }
    else {
      Serial.println("send to Wemos: " + String(input));
      WemosSerial.println(input);   // 1: Bellen, 2: Schwanzwedeln, 3: Schwanzwedeln aus
    }
    
  }
}
