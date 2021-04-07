#include "HX711.h"

#define DOUT  4
#define CLK  5

HX711 scale(DOUT, CLK);

float calibration_factor = 0;
long zero_factor = 0;
float units;

void setup() {
  Serial.begin(115200);
  Serial.println("Remove all weight from scale");
  Serial.println("Calibrating zero weight in: ");
  int i = 11;
  while(i--){
    Serial.print(i);
    Serial.println(" seconds");
    delay(1000);
  }
  scale.set_scale(calibration_factor);
  scale.tare();
  zero_factor = scale.read_average();
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
