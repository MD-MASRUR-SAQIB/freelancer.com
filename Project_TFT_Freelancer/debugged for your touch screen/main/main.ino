#include "Screen.h"

#define   OPEN    true
#define   CLOSE   false

Screen screen;

uint64_t prev;
int LoRaSleepTime; //minutes
bool LoRaTransmit;


String startScreenBackground = "BLACK"; //VIOLET, AQUA, BLUE, DEEP_GREEN, YELLOW, ORANGE, RED, BLACK, WHITE
String startScreenFont = "RED"; //VIOLET, AQUA, BLUE, DEEP_GREEN, YELLOW, ORANGE, RED, BLACK, WHITE

float InsideTemp  =   -5.6;   //SENSOR VALUE
float OutsideTemp =   2.9;    //SENSOR VALUE
float Humidity    =   66;     //SENSOR VALUE
bool  doorState   =   OPEN;   // OPEN/CLOSED ////SENSOR VALUE


void setup()
{
  Serial.begin(9600);
  screen.setup(millis());                                  //Setting the LCD driver
  screen.sbegin(startScreenBackground, startScreenFont);    // Start Screen
}

void loop()
{
  screen.update(InsideTemp, OutsideTemp, Humidity, doorState, millis());//function "millis()" returns current time to the screen functions
  
  LoRaSleepTime = screen.getLoRaSleepTime();
  LoRaTransmit  = screen.getTransmitStat();
}
