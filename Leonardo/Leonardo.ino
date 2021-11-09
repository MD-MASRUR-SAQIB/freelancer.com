#include <Keyboard.h>

char  keyStroke[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};

int         pushPin = 0;
const int   totalRelay = 12;
uint64_t    prev;
bool        relayOff = true;
bool        keySend = true;
bool        sendX = false;

void setup() {
  for(int i = 1 ; i <= totalRelay ; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  delay(5000);
  
  Keyboard.begin();
  
  pinMode(pushPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pushPin), changeMode, LOW);
  prev = millis();
}

void loop(){
  if(relayOff){
    if(!keySend){
      Keyboard.print(keyStroke[generateRealRandom(0, 11)]);
      keySend = true;
      sendX = false;
    }
  }
  else{
    if(!sendX){
      Keyboard.print('x');
      sendX = true;
    }
    keySend = false;
    int x = generateRealRandom(1, 12);
    
    digitalWrite(x, LOW);
    delay(1000);
    digitalWrite(x, HIGH);
  }
}

void changeMode(){
  if(millis() - prev > 500){
      prev = millis();
      if(relayOff){
        relayOff = false;
      }
      else{
        relayOff = true;
      }
  }
}

unsigned long generateRealRandom(int min, int max){
  randomSeed(analogRead(A5));
  uint64_t realRandom = random(1, 10) + random(1, 100) + random(1, 1000) + random(1, 10000);
  realRandom %= (max - min);
  realRandom += (min + 1);
  return realRandom;
}
