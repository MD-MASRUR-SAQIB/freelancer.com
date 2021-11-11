#include "TFLidar.h"

#define USETFLUNA

#define SERIAL  Serial
#define uart    Serial2 //can be used while not using the display. Recommended to use this.

TFLuna SeeedTFLuna;
TFLidar SeeedTFLidar(&SeeedTFLuna);

#define STEPPER_PIN_1 14
#define STEPPER_PIN_2 12
#define STEPPER_PIN_3 13
#define STEPPER_PIN_4 21

int   revolution  = 320; //degree. A complete circular rotation. You can reduce it to any value upto 360 you need.
int   perStepTime = 15; //millisecond. As the LiDAR sensor needs atleast 10ms to get a new value.
float infintyRange = 6; //meters. If the range is more than 6 meters, it'll consider as infinity range, case2 happens
float minimumRange = 0.5; //If the distance is less than or equal to (0.5 * previous distance) case3 happens

int   actualSteps  = 2038; //According to its gear ratio, motor needs 2038 steps to rotate 360 degree.
int   step_number = 0, currStep = 0;
int   stepsAcctoRev;
bool  rotDir = false; //false means anti-clockwise first.

float area1, area2, average, perStepAngle, temp;
float prevDis, newDis, tempAngle;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  delay(2000); //2s delay
  Serial.println("Initializing LiDAR module.");
  SeeedTFLidar.begin(&uart, 115200);

  Serial.println("Initializing Motor Pins.");
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);

  stepsAcctoRev = (actualSteps / 360.0) * min(revolution, 360); //min function is just for safety. incase if anyone increases the revolution value more than 360
  perStepAngle  = 360.0 / 2038.0;
  tempAngle     = perStepAngle;
  Serial.println("Starting Area Calculation.");
  compRot();
}

void loop() {

}

void compRot() {
  area1 = 0;
  area2 = 0;
  while (!SeeedTFLidar.get_frame_data()) {
    delay(1);
  }
  prevDis = SeeedTFLidar.get_distance() / 100.0; //in meter. Setting previous distance variable for the first time.

  for (currStep = 0; currStep <= stepsAcctoRev; currStep++) {
    OneStep(rotDir);
    while (!SeeedTFLidar.get_frame_data()) {
      delay(1);
    }
    newDis = SeeedTFLidar.get_distance() / 100.0;
    area1 += area();
    delay(perStepTime);
    
    Serial.print("Step = ");
    Serial.print(currStep+1);
    Serial.print("Angle = ");
    Serial.print(tempAngle);
    Serial.print("Current Area = ");
    Serial.println(area1);
  }
  Serial.print("\n\n\nArea1 = ");
  Serial.println(area1);
  prevDis = SeeedTFLidar.get_distance() / 100.0;
  rotDir = !rotDir;

  for (currStep = 0; currStep <= stepsAcctoRev; currStep++) {
    OneStep(rotDir);
    while (!SeeedTFLidar.get_frame_data()) {
      delay(1);
    }
    newDis = SeeedTFLidar.get_distance() / 100.0;
    area2 += area();
    delay(perStepTime);

    Serial.print("Step = ");
    Serial.print(currStep+1);
    Serial.print("Angle = ");
    Serial.print(tempAngle);
    Serial.print("Current Area = ");
    Serial.println(area2);
  }
  Serial.print("\n\n\nArea2 = ");
  Serial.println(area2);
  Serial.print("Average Area = ");
  average = (area1 + area2) / 2.0;
  Serial.println(average);
}

float area() {
  if (prevDis * minimumRange >= newDis) { // If there is an object.
    tempAngle += perStepAngle;    //Increase the angle but don't calculate Area
    return 0;
  }
  if (infintyRange < newDis) { // If the range is larger than the infinity range
    temp = 0.5 * prevDis * prevDis * cos(tempAngle); // use the previous values only
    tempAngle = perStepAngle;
    return temp;
  }
  temp = 0.5 * prevDis * newDis * cos(tempAngle);
  tempAngle = perStepAngle;
  prevDis = newDis;
  return temp;
}

void OneStep(bool dir) {
  if (dir) {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
    }
  } else {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);


    }
  }
  step_number++;
  if (step_number > 3) {
    step_number = 0;
  }
}
