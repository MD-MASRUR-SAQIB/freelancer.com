#include "TFLidar.h"

#define USETFLUNA

#define SERIAL  Serial
#define uart    Serial2 //can be used while not using the display. Recommended to use this.

TFLuna SeeedTFLuna;
TFLidar SeeedTFLidar(&SeeedTFLuna);

void setup() {
    // put your setup code here, to run once:
    SERIAL.begin(9600);
    while(!Serial);
    int buff[3] = {0};
    SeeedTFLidar.begin(&uart,9600);
    SeeedTFLidar.set_frame_rate(SAMPLERATE_100HZ);
    if(SeeedTFLidar.save_config()){
        SERIAL.println("config successful");
    }
    else{
        SERIAL.println("config failed");
    }

}
void loop() {
}
