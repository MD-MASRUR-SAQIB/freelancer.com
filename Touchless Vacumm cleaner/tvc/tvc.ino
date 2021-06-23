#define echoPin   2
#define trigPin   3
#define SENSORPIN 4
#define led1      5
#define led2      6


long  duration;
int   distance;
int   targetDistance = 10;
int   sensorState = 0, lastState=0;


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(SENSORPIN, INPUT_PULLUP);
  
  
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
  
  if(distance <= targetDistance){
    digitalWrite(led1, HIGH);
  }
  else{
    digitalWrite(led1, LOW);
  }

  sensorState = digitalRead(SENSORPIN);
  
  if(sensorState == LOW){
    digitalWrite(led2, HIGH);
  }
  else{
    digitalWrite(led2, LOW);
  }
  
}
