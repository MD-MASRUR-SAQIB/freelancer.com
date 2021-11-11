#include "TFLidar.h"

#define USETFLUNA

#define SERIAL  Serial
#define uart    Serial2 //can be used while not using the display. Recommended to use this.

TFLuna SeeedTFLuna;
TFLidar SeeedTFLidar(&SeeedTFLuna);

void setup() {
  SERIAL.begin(9600);
  while (!Serial);
  SeeedTFLidar.begin(&uart, 115200);
}

void loop() {
  while (!SeeedTFLidar.get_frame_data()) {
    delay(1);
  }
  SERIAL.print("dist = ");
  SERIAL.print(SeeedTFLidar.get_distance()); //output measure distance value of LiDAR
  SERIAL.print('\t');
}
