#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

#define S0 8
#define S1 7
#define S2 2
#define S3 4
#define sensorOut 13

int frequency = 0;
int b = 750;
int x = 2;
int f = 0;
typedef enum { BLAU, ORANGE, KA } farbe_t;

void setup () {
  Serial.begin(9600);
  Braccio.begin();
  nichts();
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0,LOW);
  digitalWrite(S1,HIGH);
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);

}


void loop(){
  delay(10);
  static farbe_t farbe = KA;
  static uint8_t farbe_zaehler = 0;
  frequency = pulseIn(sensorOut, LOW);
  //Serial.println("");
 // Serial.print("fre");
  Serial.println(frequency);
  //Serial.print("  ");
  
  if(frequency >=b){
    f = 1;
  }
  else if(frequency >= x && frequency <= 50+x){
    f = 2;
  }
  else{
    f = 0;
  }

    switch (f) {
      case 1:
        if (farbe == BLAU) {
          farbe_zaehler++;
        } else {
          farbe = BLAU;
          farbe_zaehler = 1;
        }
        break;
      case 2:
        if (farbe == ORANGE) {
          farbe_zaehler++;
        } else {
          farbe = ORANGE;
          farbe_zaehler = 1;
        }
        break;
      default:
      farbe_zaehler = 0;
      break;
    }

   // Serial.print(farbe);
    //Serial.print(", ");
   // Serial.print(farbe_zaehler);

    if (farbe_zaehler > 3) {
      switch (farbe) {
        case BLAU:
          Serial.println("blau entdeckt");
                farbe = KA;
                farbe_zaehler = 0;
                futter();
                
          break;
        case ORANGE:
          Serial.println("orange entdeckt");
                farbe = KA;
                farbe_zaehler = 0;
                keinfutter();
              
          break;
      }
    }
}
    
void futter(){
                    //(step delay M1,  M2,  M3,  M4,  M5,  M6);
  Greifen();
  Braccio.ServoMovement(20,        5,  95,   5,   3,  70,  70);//aufheben
  Braccio.ServoMovement(20,        5, 130,   5,   3,  70,  70);
  Braccio.ServoMovement(20,        5, 130,  90,   3,  180, 70);
  Braccio.ServoMovement(20,        5,  95,  90,   3,  180, 70);
  Braccio.ServoMovement(20,        5,  95,  90,  90,  180, 70);//Grundposition
  Braccio.ServoMovement(20,        5,  95,  90, 180,  180, 70);//andere richtung Runter
  Braccio.ServoMovement(20,        5, 130,  90, 180,  180, 70);
  Braccio.ServoMovement(20,        5, 130,  90, 180,  180, 10);
  
  delay(1000);
   
  Braccio.ServoMovement(20,        5,  95,  90, 180,  180,   0);
  Braccio.ServoMovement(20,        5,  95,  90,  90,  180,   0);//andere Richtung rauf
  nichts();
  }
 
  void keinfutter(){
                    //(step delay M1,  M2,  M3,  M4,  M5,  M6);
  Greifen();
  Braccio.ServoMovement(20,        5,  95,   5,   3,  70,   70);//aufheben
  Braccio.ServoMovement(20,        5, 130,   5,   3,  70,   70);
  Braccio.ServoMovement(20,        5, 130,  90,   3, 180,  70);
  Braccio.ServoMovement(20,        5,  95,  90,   3, 180,  70);
  Braccio.ServoMovement(20,        5,  95,  90,  90, 100,  70);//Grundposition
  Braccio.ServoMovement(20,      100,  95,  90,  90, 180,  70);//drehen
  delay(100);
  Braccio.ServoMovement(20,      100,  95,   0,   3, 170,  70);
  Braccio.ServoMovement(20,      100,  95,   0,   3, 170,   0);//runter
  delay(1000);
  Braccio.ServoMovement(20,      100, 130,   0,   3, 170,   0);
  Braccio.ServoMovement(20,      100, 130,  90,   3, 180,   0);
  Braccio.ServoMovement(20,      100,  95,  90,   3, 180,   0);
 nichts();
  } 

  void Greifen() {
                    //(step delay M1,  M2,  M3,  M4,  M5,  M6);
  Braccio.ServoMovement(20,        5,  95,  90,  90, 20,   0);//holen
  Braccio.ServoMovement(20,        5,  95,  90,   3, 20,   0);
  Braccio.ServoMovement(20,        5, 130,  90,   3, 50,   0);
  Braccio.ServoMovement(20,       13, 130,   0,   3, 70,   0);
  Braccio.ServoMovement(20,       13,  95,   0,   3, 70,   0);
  delay(100); 
  Braccio.ServoMovement(20,       13,  95,   5,   3, 70,  70);
  delay(100); 
  }
  void nichts() {
  Braccio.ServoMovement(20,        5,  95,  90,  90,150,   0);
  delay(100);
  }
