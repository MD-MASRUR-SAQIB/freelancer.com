#include "heltec.h"
#include "HX711.h"

// HX711: Initialization of classes and Defining Macros
uint8_t   scaleDataPin    = 12;
uint8_t   scaleClockPin   = 13;
long      scaleOffset     = 0; //************Input Scale Calibrated Data here!!
float     scaleData       = 1.0; //************Input Scale Calibrated Data here!!

HX711     scale;

void setup()
{
  Heltec.begin(true, false, true);
  delay(5000);
  pinMode(25, OUTPUT);    //Your Code
  digitalWrite(25, HIGH); //Your Code

  Serial.begin(9600);

  //Setup: Scale Calibration
  Serial.println("Initializing Scale! \n\n***Remove all the weights from the scale.");
  scale.begin(scaleDataPin, scaleClockPin);
  delay(2000);
  if (scale.is_ready()) {
    Serial.println("Getting Scale Offset!");
    scale.tare();
    scaleOffset = scale.get_offset();
    scale.set_offset(scaleOffset);
    Serial.print("Scale Offset = ");
    Serial.println(scaleOffset);
    
    Serial.println("Getting Scale Data!");
    Serial.println("Put any known weight on the scale (example: 100 gram)!!");
    for (int i = 0 ; i < 10 ; i++) {
      Serial.print(10 - i);
      Serial.print(" ");
      delay(1000);
    }
    Serial.println("\nMeasuring Scale Data.");
    scaleData = scale.get_value(10);
    scale.set_scale(scaleData);
    Serial.print("Scale Data = ");
    Serial.print(scaleData);
    Serial.print("/your known weight.");
  }
  else {
    Serial.println("Scale connection error!!");
  }
}
void loop(){
  
}
