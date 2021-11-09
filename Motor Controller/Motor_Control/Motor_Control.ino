/*
   Motor Driver Connection:
   VCC ->   +5 volt (Arduino)
   GND ->   GND     (Arduino)
   R_EN ->  Pin 8   (Arduino)
   L_EN ->  Pin 9   (Arduino)
   RPWM ->  Pin 5   (Arduino)
   LPWM ->  Pin 6   (Arduino)

   M+ ->  Motor Positive
   M- ->  Motor Negative
   B+ ->  Power supply Positive
   B- ->  Power supply Negative
*/

#define RPWM    5
#define LPWM    6
#define REN     8
#define LEN     9
#define speed   180 //limit: 0-255. gain maximum speed at 255

#define pushPin 3   //connect the push button between pin 3 and GND

void setup() {
  Serial.begin(9600);
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(LEN, OUTPUT);
  pinMode(REN, OUTPUT);
  pinMode(pushPin, INPUT_PULLUP);
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
}

void loop() {

  if (digitalRead(pushPin) == LOW) {
    analogWrite(RPWM, speed);
    analogWrite(LPWM, 0);
    delay(3000);
    analogWrite(LPWM, speed);
    analogWrite(RPWM, 0);
    delay(3000);
  }
}
