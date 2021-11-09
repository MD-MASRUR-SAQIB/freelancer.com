int irPin = 3; //connect the ir output pin to Arduino pin 3

int enA = 9;
int in1 = 8;
int in2 = 7;
int speed = 255;
uint64_t motorTime = 5000;

int swtch = 4; //connect the switch between pin 4 and ground

long  duration;
int   distance;
int   targetDistance = 15;
int   sensorState = 0, lastState = 0;


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(irPin, INPUT);
  pinMode(swtch, INPUT_PULLUP);

  Serial.begin(115200);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, speed);
}
void loop() {

  if (digitalRead(irPin) == LOW && digitalRead(swtch)) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    delay(motorTime);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

}
