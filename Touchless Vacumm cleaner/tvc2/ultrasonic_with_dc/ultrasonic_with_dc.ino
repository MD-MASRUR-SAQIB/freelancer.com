int echoPin = 2;
int trigPin = 3;

int enA = 9;
int in1 = 8;
int in2 = 7;
int speed = 255;
uint64_t motorTime = 5000;

int swtch = 4;

long  duration;
int   distance;
int   targetDistance = 15;
int   sensorState = 0, lastState = 0;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(swtch, INPUT_PULLUP);

  Serial.begin(115200);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, speed);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance <= targetDistance && digitalRead(swtch) == LOW) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    delay(motorTime);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

}
