#include <Servo.h>

Servo ESC;

int echoPin = 2;
int trigPin = 3;

int motorPin = 9;
int speed = 100;
long motorTime = 10000;

int swtch = 4;

long  duration;
int   distance;
int   targetDistance = 15;
int   sensorState = 0, lastState = 0;


void setup() {
  
  ESC.attach(motorPin,1000,2000);
  delay(500);
  ESC.writeMicroseconds(2000);
  delay(3000);
  ESC.writeMicroseconds(1000);
  delay(3000);  

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(swtch, INPUT_PULLUP);

  Serial.begin(115200);
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance <= targetDistance && !digitalRead(swtch)) {
    ESC.write(speed);
    delay(motorTime);
    ESC.write(0);
  }

}
