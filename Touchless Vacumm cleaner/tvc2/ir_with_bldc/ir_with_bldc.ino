#include <Servo.h>

Servo ESC;

int irPin = 3; //connect the ir output pin to Arduino pin 3

int motorPin = 9;
int speed = 100;
long motorTime = 10000;

int swtch = 4; //connect the switch between pin 4 and ground

void setup() {
  
  ESC.attach(motorPin,1000,2000);
  delay(500);
  ESC.writeMicroseconds(2000);
  delay(5000);
  ESC.writeMicroseconds(1000);
  delay(3000);  

  pinMode(irPin, INPUT);

  pinMode(swtch, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {

  if (digitalRead(irPin) == LOW && digitalRead(swtch) == LOW) {
    ESC.write(speed);
    delay(motorTime);
    ESC.write(0);
  }

}
