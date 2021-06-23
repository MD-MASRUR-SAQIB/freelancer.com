#include <Keyboard.h>

int sPin[7];
const int dBTK = 0.5; //minimum delay Between Two Keystrokes in second, Here 0.5s
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
  Keyboard.begin();
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  delay(10000);
  Serial.println("Please, Keep all the objects near the infrared sensors.");
  delay(2000);
  Serial.println("If you have placed all the objects near the sensors,\ninput \"Y\" in the Serial monitor. Then press the send button.");
  while (r != 'Y') {
    if (Serial.available()) {
      r = Serial.read();
      if (r == 'Y') {
        Serial.println("Reading Data.");
        break;
      }
      else {
        Serial.println("Please enter \"Y\" without the quotation!");
      }
    }
  }
  for (int i = 0 ; i < 7 ; i++) {
    sreading[i] = analogRead(sPin[i]);
  }

  Serial.print("Sensor0 reading: ");
  Serial.println(sreading[0]);
  Serial.print("Sensor1 reading: ");
  Serial.println(sreading[1]);
  Serial.print("Sensor2 reading: ");
  Serial.println(sreading[2]);
  Serial.print("Sensor3 reading: ");
  Serial.println(sreading[3]);
  Serial.print("Sensor4 reading: ");
  Serial.println(sreading[4]);
  Serial.print("Sensor5 reading: ");
  Serial.println(sreading[5]);
  Serial.print("Sensor6 reading: ");
  Serial.println(sreading[6]);

  r = "z";
  delay(2000);
  Serial.println("\nDefault Keystroks - ");
  Serial.println("Sensor num\tKeystroke");
  Serial.println("Sensor 0\tA");
  Serial.println("Sensor 1\tB");
  Serial.println("Sensor 2\tC");
  Serial.println("Sensor 3\tD");
  Serial.println("Sensor 4\tE");
  Serial.println("Sensor 5\tF");
  Serial.println("Sensor 6\tG");
  Serial.println("\nDo you want to change the default keystroks?\n Enter \"Y\" to change or \"n\" to go with default.");
  Serial.readString();
  while (1) {
    if (Serial.available()) {
      r = Serial.read();
      Serial.readString();
      if (r == 'Y') {
        for (int i = 0 ; i < 7; i++) {
          Serial.readString();
          Serial.print("\nEnter a Key for sensor ");
          Serial.print(i);
          Serial.print(": ");
          while (!Serial.available());
          sKey[i] = Serial.read();
          Serial.print("The sensor ");
          Serial.print(i);
          Serial.print(" has been configured to send the keystroke: ");
          Serial.println(sKey[i]);
        }
        break;
      }
      else if (r == 'n') {
        Serial.println("Using default!!");
        break;
      }
      else {
        Serial.print("\n Enter \"Y\" to change or \"n\" to go with default.");
      }
    }
  }
  Serial.println("\nInitiating Keyboard Mode...");
}

void loop() {
  for (int i = 0 ; i < 7 ; i++) {
    nreading[i] = analogRead(sPin[i]);
    if (abs(nreading[i] - sreading[i]) > 5) {
      Keyboard.print(sKey[i]);
      sreading[i] = nreading[i];
      delay(dBTK * 1000);
    }
  }
}
