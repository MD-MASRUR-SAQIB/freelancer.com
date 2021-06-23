/*
   Author: Md. Masrur Saqib, CSE-18, RUET
   Organization: Gizan-Tech
   Date of creation: 05-Nov-20
   Project Name: Low Cost Air Purifier
   Last Modification: 13-Dec-20
   Modified by: Md. Masrur Saqib
*/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include<Arduino.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

class Display {

  public:
    Display()
    {

    }
    void begin2(){
      lcd.begin();
    }

    void off(){
      lcd.noDisplay();
      lcd.noBacklight();
    }

    void on(){
      lcd.display();
      lcd.backlight();
    }

    void begin(String Name_of_Scale) {
      lcd.begin();
      lcd.setCursor(0, 0);
      lcd.print("     Beehive    ");
      lcd.setCursor(0, 1);
      lcd.print("    Telemetry   ");
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("  Initializing  ");
      lcd.setCursor(0, 1);
      lcd.print(" Beehive V.0.1  ");
      delay(1000);
      showMid(0, Name_of_Scale);
    }

    void simStat(String op, String str) {
      lcd.setCursor(0, 0);
      lcd.print("  Initializing  ");
      lcd.setCursor(0, 1);
      lcd.print("  Sim Module...  ");
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("Opr:             ");
      lcd.setCursor(5, 0);
      lcd.print(op);
      lcd.setCursor(0, 1);
      lcd.print("Str:             ");
      lcd.setCursor(5, 1);
      lcd.print(str);
      delay(1500);
    }

    void rtcStat(String DT) {
      Serial.println(DT);
      lcd.setCursor(0, 0);
      lcd.print("  Initializing  ");
      lcd.setCursor(0, 1);
      lcd.print("  RTC Module... ");
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("Time Stamp:     ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(DT);
      delay(1500);
    }

    void scaleCalib() {
      lcd.setCursor(0, 0);
      lcd.print("  Calibrating   ");
      lcd.setCursor(0, 1);
      lcd.print(" Weight Scale...");
      delay(1500);
    }

    void initDht() {
      lcd.setCursor(0, 0);
      lcd.print("   Initiating   ");
      lcd.setCursor(0, 1);
      lcd.print("  DHT Sensor... ");
      delay(1500);
    }

    void dhtInitError() {
      lcd.setCursor(0, 0);
      lcd.print("   DHT Failed   ");
      lcd.setCursor(0, 1);
      lcd.print(" to initiate... ");
      delay(1500);
    }

    void pirMotion() {
      lcd.setCursor(0, 0);
      lcd.print("   PIR sensor   ");
      lcd.setCursor(0, 1);
      lcd.print("motion detected.");
      delay(1000);
    }

    void showMid(int y, String text) {
      lcd.setCursor(0, y);
      lcd.print("                ");
      int size = text.length();
      size = 16 - size;
      if (size > 0) {
        int x = ceil(size / 2.0) - 1;
        lcd.setCursor(x, y);
        lcd.print(text);
      }
      else {
        lcd.setCursor(0, y);
        lcd.print(text);
      }
    }

    void showFront(int y, String text) {
      lcd.setCursor(0, y);
      lcd.print("                ");
      lcd.setCursor(0, y);
      lcd.print(text);
    }

};

#endif
