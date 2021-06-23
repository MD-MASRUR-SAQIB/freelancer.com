#include <Keyboard.h>
#define tPushButtons 7

const int  dBTK = 500; //minimum delay Between Two Keystrokes in second, Here 0.5s
const int  ledTime = 300; //Change the led off time in millisecond

char       sKey[tPushButtons]    = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

// Connect one head of each push buttons to these pins and another head to GND
int        sPin[tPushButtons] = { A0,  A1,  A2,  A3,  A4,  A5,  3 };

// Connect led to these pins with 150 ohm resistors
int        ledPin[tPushButtons]  = { 4 ,  5 ,  6 ,  7 ,  8 ,  9 , 10 };

uint64_t   lastTime[tPushButtons];
uint64_t   ledLastTime;
int        nreading[tPushButtons];

void setup() {
  //Serial.begin(9600);
  
  ledLastTime = millis();
  
  for (int i = 0 ; i < tPushButtons ; i++) {
    pinMode( sPin[i], INPUT_PULLUP);
    lastTime[i] = millis();
    pinMode( ledPin[i], OUTPUT);
    delay(50);
  }
  
  for (int i = 0 ; i < tPushButtons; i++) {
    digitalWrite(ledPin[i], HIGH);
  }
  
  delay(1000);
  Keyboard.begin();
}

void loop() {
  for (int i = 0 ; i < tPushButtons ; i++) {
    nreading[i] = digitalRead(sPin[i]);
    if (nreading[i] == LOW) {
      keySend(i);
    }
    if (millis() - ledLastTime > ledTime) {
      for (int i = 0 ; i < tPushButtons; i++) {
        digitalWrite(ledPin[i], HIGH);
      }
    }
  }
}

void keySend(int x) {
  if (millis() - lastTime[x] > dBTK) {
    ledLastTime = millis();
    lastTime[x] = millis();
    Keyboard.print(sKey[x]);
    //Serial.println(sKey[x]);
    for (int i = 0 ; i < tPushButtons; i++) {
      digitalWrite(ledPin[i], LOW);
    }
    digitalWrite(ledPin[x], HIGH);
  }
}
