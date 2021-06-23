#include <Keyboard.h>
#define tPushButtons 7

const int  dBTK = 500; //minimum delay Between Two Keystrokes in second, Here 0.5s
char       sKey[tPushButtons] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
int        sPin[tPushButtons] = { A0,  A1,  A2,  A3,  A4,  A5,  3 };
uint64_t   lastTime[tPushButtons];
int        nreading[tPushButtons];

void setup() {
  Serial.begin(9600);
  
  for(int i = 0 ; i < tPushButtons ; i++){
    pinMode( sPin[i], INPUT_PULLUP);
    lastTime[i] = millis();
    delay(50);
  }
  delay(1000);
  Keyboard.begin();
}

void loop() {
  for (int i = 0 ; i < tPushButtons ; i++) {
    nreading[i] = digitalRead(sPin[i]);
    if(nreading[i] == LOW){
      keySend(i);
    }
  }
}

void keySend(int x){
  if(millis() - lastTime[x] > dBTK){
    lastTime[x] = millis();
    Keyboard.print(sKey[x]);
    Serial.println(sKey[x]);
  }
}
