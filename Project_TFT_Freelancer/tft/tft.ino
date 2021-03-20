#include "Screen.h"
#define OPEN  true
#define CLOSE false

String startScreenBackground = "BLACK"; //VIOLET, AQUA, BLUE, DEEP_GREEN, YELLOW, ORANGE, RED, BLACK, WHITE
String startScreenFont = "RED"; //VIOLET, AQUA, BLUE, DEEP_GREEN, YELLOW, ORANGE, RED, BLACK, WHITE


Screen screen;

bool doorStat = true;
bool prevStat = true;
bool doorAlarm = false; //True if open, False if closed
uint64_t prev;

void setup()
{
  Serial.begin(9600);
  screen.setup(millis());
  screen.sbegin(startScreenBackground, startScreenFont);
  prev = millis();
}

void loop()
{
  doorStat = OPEN;
  screen.updateTemp(-1.14, -4.96, random(100), doorAlarm, millis());
  checkDoor();
}

void checkDoor()
{
  if (doorStat && prevStat) {
    Serial.println(1);
    prev = millis();
    prevStat = false;
    screen.doorOpened();
  }

  if (millis() - prev > ( screen.getDoorAlarmTime() * 60 * 1000 ) && doorStat) {
    Serial.println(2);
    doorAlarm = true;
    prevStat = true;
  }
  else if (!doorStat) {
    Serial.println(3);
    doorAlarm = false;
    prevStat = true;
    screen.doorClosed();
  }
}
