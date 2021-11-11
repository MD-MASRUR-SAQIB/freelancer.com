#define STEPPER_PIN_1 14
#define STEPPER_PIN_2 12
#define STEPPER_PIN_3 13
#define STEPPER_PIN_4 21

int revolution  = 320; //degree. A complete circular rotation. You can reduce it to any value upto 360 you need.
int perStepTime = 15; //millisecond. As the LiDAR sensor needs atleast 10ms to get a new value.

int   actualSteps  = 2038; //According to its gear ratio, motor needs 2038 steps to rotate 360 degree.
int   step_number = 0, currStep = 0;
int   stepsAcctoRev;
bool  rotDir = false; //false means anti-clockwise first.

void setup() {
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);

  stepsAcctoRev = (actualSteps/360.0) * min(revolution,360); //min function is just for safety. incase if anyone increases the revolution value more than 360
  compRot();
}

void loop() {

}

void compRot(){
  for(currStep = 0; currStep <= stepsAcctoRev; currStep++){
    OneStep(rotDir);
    delay(perStepTime);
  }
  
  rotDir = !rotDir;
  
  for(currStep = 0; currStep <= stepsAcctoRev; currStep++){
    OneStep(rotDir);
    delay(perStepTime);
  }
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
