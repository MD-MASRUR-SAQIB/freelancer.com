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
int   perStepTime = 12; //millisecond. As the LiDAR sensor needs atleast 10ms to get a new value.
float infintyRange = 6; //meters. If the range is more than 6 meters, it'll consider as infinity range, case2 happens
float minimumRange = 0.65; //If the distance is less than or equal to (0.5 * previous distance) case3 happens
float maximumRange = 0.5; //If the distance is less than or equal to (0.5 * previous distance) case3 happens
float rotationFactor = 5; //Increase the factor to increase the speed and decrease the steps
int   comFact;
float   restofTheAngle;

int   actualSteps  = 2038; //According to its gear ratio, motor needs 2038 steps to rotate 360 degree.
int   step_number = 0, currStep = 0;
int   stepsAcctoRev;
bool  rotDir = false; //false means anti-clockwise first.

float area1, area2, average, perStepAngle, temp, smArea, bigArea, firstDis;
float prevDis, newDis, tempAngle, smBase, bigBase, smHypot, bigHypot, smHeight, bigHeight;

uint64_t prevTime;

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

  Serial.println("Starting Area Calculation.");
  prevTime = millis();
  compRot();
  Serial.print("Total Time: ");
  prevTime = millis() - prevTime;
  Serial.println((int)prevTime / 1000);
}

void loop() {

}

void compRot() {
  float max1 = -1, max2 = -1, max3 = -1, max4 = -1;
  int   steps1, steps2, steps3, steps4;
  int   finalSteps = stepsAcctoRev / rotationFactor;
  perStepAngle  = (2.0 * acos(-1) * rotationFactor) / 2038.0;
  
  for (currStep = 0; currStep <= finalSteps; currStep++) {
    for (comFact = 0; comFact < rotationFactor; comFact++) {
      OneStep(rotDir);
      delay(3);
    }
    if (currStep <= finalSteps / 4) {
      while (!SeeedTFLidar.get_frame_data()) {
        delay(1);
      }
      newDis = SeeedTFLidar.get_distance() / 100.0;
      if(max1 > newDis){
        steps1 = currStep;
        max1   = newDis;
      }  
    }
    else if (currStep > finalSteps / 4 && currStep <= finalSteps / 2) {
      while (!SeeedTFLidar.get_frame_data()) {
        delay(1);
      }
      newDis = SeeedTFLidar.get_distance() / 100.0;
      if(max2 > newDis){
        steps2 = currStep;
        max2   = newDis;
      }  
    }
    else if (currStep > finalSteps / 2 && currStep <= 3 * finalSteps / 4) {
      while (!SeeedTFLidar.get_frame_data()) {
        delay(1);
      }
      newDis = SeeedTFLidar.get_distance() / 100.0;
      if(max3 > newDis){
        steps3 = currStep;
        max3   = newDis;
      }  
    }
    else if (currStep > 3 * finalSteps / 4 && currStep <= finalSteps) {
      while (!SeeedTFLidar.get_frame_data()) {
        delay(1);
      }
      newDis = SeeedTFLidar.get_distance() / 100.0;
      if(max4 > newDis){
        steps4 = currStep;
        max4   = newDis;
      }  
    }
    delay(perStepTime);
  }
  float area1 = 0.5 * max1 * max2 * sin(steps2-steps1*perStepAngle);
  float area2 = 0.5 * max2 * max3 * sin(steps2-steps3*perStepAngle);
  float area3 = 0.5 * max3 * max4 * sin(steps3-steps4*perStepAngle);
  float area4 = 0.5 * max4 * max1 * sin((finalSteps - steps4 + steps1)*perStepAngle);

  Serial.print("Area1: ");
  Serial.println(area1, 8);
  Serial.print("Area2: ");
  Serial.println(area2, 8);
  Serial.print("Area3: ");
  Serial.println(area3, 8);
  Serial.print("Area4: ");
  Serial.println(area4, 8);
  Serial.print("Total Area: ");
  Serial.println(area1+area2+area3+area4, 8);
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
