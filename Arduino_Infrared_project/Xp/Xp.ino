#include <Keyboard.h>

int sPin[7];
const int dBTK = 1; //minimum delay Between Two Keystrokes in second, Here 0.5s
char  r  = 'n';
char  sKey[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

int   sreading[7], nreading[7];

void setup() {
  Serial.begin(9600);
  sPin[0] = A0;
  sPin[1] = A1;
  sPin[2] = A2;
  sPin[3] = A3;
  sPin[4] = A4;
  sPin[5] = A5;
  sPin[6] = A6; //Digital pin 4
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  Serial.println("Please, Keep all the objects near the infrared sensors within 5 seconds.");
  delay(2000);

  for (int i = 0 ; i < 7 ; i++) {
    sreading[i] = analogRead(sPin[i]);
  }
  delay(5000);
  Keyboard.begin();
}

void loop() {
  for (int i = 0 ; i < 7 ; i++) {
    nreading[i] = analogRead(sPin[i]);

    if (abs(nreading[i] - sreading[i]) > 100) {
      if (sreading[i] > 1000) {
        delay(dBTK * 1000);
        sreading[i] = analogRead(sPin[i]);
      }
      else {
        Keyboard.print(sKey[i]);
        delay(dBTK * 1000);
        sreading[i] = 1023;
      }
    }
  }
}
