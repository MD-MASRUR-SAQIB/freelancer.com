#define HALL_SENSOR 2
#define RELAY1 3
#define RELAY2 4
#define RELAY3 5
#define RELAY4 6
#define RELAY5 7
#define RELAY6 8
#define RELAY7 9
#define RELAY8 10


#define rotations 100

uint8_t prevCon = 0;
uint8_t currCon = 0;

uint8_t relays[] = {RELAY1, RELAY2, RELAY3, RELAY4, RELAY5, RELAY6, RELAY7, RELAY8};
int count = 0;

void setup() {
  pinMode(HALL_SENSOR, INPUT);

  for (int i = 0; i < sizeof(relays); i++) {
    pinMode(relays[i], OUTPUT);
  }
  for (int i = 0; i < sizeof(relays); i++) {

  }

}

void loop() {

  if (digitalRead(HALL_SENSOR) == HIGH) {
    count++;
    while (digitalRead(HALL_SENSOR) == HIGH) {

    }
    delay(1);
  }

  if (count == rotations * 1.5) {
    digitalWrite(relays[0], HIGH);
    currCon = 1;
    if (prevCon != currCon) {
      digitalWrite(relays[7], HIGH); //sets the relay 8 on (siren)
      delay(100);            // waits for a second
      digitalWrite(relays[7], LOW); //sets the relay 8 off (siren)
      delay(100);
      prevCon = currCon;
    }
  }

  if (count == (rotations * 1.5) + (rotations * 1.5)) {
    digitalWrite(relays[1], HIGH);
    currCon = 2;
    if (prevCon != currCon) {
      digitalWrite(relays[7], HIGH); //sets the relay 8 on (siren)
      delay(100);            // waits for a second
      digitalWrite(relays[7], LOW); //sets the relay 8 off (siren)
      delay(100);
      prevCon = currCon;
    }
  }

  if (count == (rotations * 1.5) + (rotations * 1.5) + (rotations * 1.5)) {
    digitalWrite(relays[2], HIGH);
    currCon = 3;
    if (prevCon != currCon) {
      digitalWrite(relays[7], HIGH); //sets the relay 8 on (siren)
      delay(100);            // waits for a second
      digitalWrite(relays[7], LOW); //sets the relay 8 off (siren)
      delay(100);
      prevCon = currCon;
    }
  }
}
