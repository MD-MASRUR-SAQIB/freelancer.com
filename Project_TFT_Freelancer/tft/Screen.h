#ifndef __SCREEN_H__
#define __SCREEN_H__

#include<Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <Arduino.h>

#include<Fonts/FreeSerif12pt7b.h>
#include<Fonts/FreeSerif24pt7b.h>
#include<Fonts/FreeSerifBold12pt7b.h>
#include<Fonts/FreeSerifBold24pt7b.h>
#include<Fonts/FreeSerifBold18pt7b.h>
#include<Fonts/FreeSerifBold9pt7b.h>
#include<Fonts/FreeSerif9pt7b.h>


#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

//const int TS_LEFT=907,TS_RT=136,TS_TOP=942,TS_BOT=139;


#define startScreenDelay  2//seconds

//Defined Colors
int BLACK = 0x0000;
int WHITE = 0xFFFF;
int GREEN = 0x07E0;
int LIME = 0x87E0;
int BLUE = 0x001F;
int RED = 0xF800;
int AQUA = 0x5D1C;
int YELLOW = 0xFFE0;
int CYAN = 0x07FF;
int ORANGE = 0xFCA0;
int VIOLET = 0x9199;
int DEEP_GREEN = 1263010;


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
MCUFRIEND_kbv tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

class Screen {
    float LowTemp = - 20, HighTemp = 30;
    float InsideTemp, OutsideTemp;
    bool ecolor = true;
    bool transmit = true;
    int sleepTime = 5;
    int doorAlarmTime = 1;
    uint64_t prev, prev2, lastDoorOpenedTime;
    int xcolor = 0;
    String input = "";
    int settingsVal = 0;
    bool emergency = false;
    bool doorEmergency = false;
    int touchPage = 0;
    bool once = true;

  public:
    Screen() {}
    void setup(uint64_t now) {
      prev = now, prev2 = now, lastDoorOpenedTime = 0;
      tft.reset();
      uint16_t ID = tft.readID();
      Serial.println(ID);
      tft.begin(ID);
      tft.setRotation(3);
      pinMode(13, OUTPUT);

    }

    void sbegin(String backColor, String fontColor) {

      if (backColor == "VIOLET") {
        tft.fillScreen(VIOLET);
      }
      else if (backColor == "BLACK") {
        tft.fillScreen(BLACK);
      }
      else if (backColor == "WHITE") {
        tft.fillScreen(WHITE);
      }
      else if (backColor == "AQUA") {
        tft.fillScreen(AQUA);
      }
      else if (backColor == "BLUE") {
        tft.fillScreen(BLUE);
      }
      else if (backColor == "DEEP_GREEN") {
        tft.fillScreen(DEEP_GREEN);
      }
      else if (backColor == "YELLOW") {
        tft.fillScreen(YELLOW);
      }
      else if (backColor == "ORANGE") {
        tft.fillScreen(ORANGE);
      }
      else if (backColor == "RED") {
        tft.fillScreen(RED);
      }

      //VIOLET, AQUA, BLUE, DEEP_GREEN, YELLOW, ORANGE, RED

      if (fontColor == "VIOLET") {
        tft.setTextColor(VIOLET);
      }
      else if (fontColor == "AQUA") {
        tft.setTextColor(AQUA);
      }
      else if (fontColor == "BLUE") {
        tft.setTextColor(BLUE);
      }
      else if (fontColor == "DEEP_GREEN") {
        tft.setTextColor(DEEP_GREEN);
      }
      else if (fontColor == "YELLOW") {
        tft.setTextColor(YELLOW);
      }
      else if (fontColor == "ORANGE") {
        tft.setTextColor(ORANGE);
      }
      else if (fontColor == "RED") {
        tft.setTextColor(RED);
      }
      else if (fontColor == "BLACK") {
        tft.setTextColor(BLACK);
      }
      else if (fontColor == "WHITE") {
        tft.setTextColor(WHITE);
      }

      tft.setFont(&FreeSerifBold24pt7b);
      tft.setCursor(30, 120);
      tft.println("ONESENSE");
      tft.setFont(&FreeSerifBold9pt7b);
      tft.setCursor(20, 220);
      tft.println(fontColor);
      delay(startScreenDelay * 1000);
      normalScreen();
    }

    void normalScreen() {

      tft.fillScreen(BLACK);
      tft.setCursor(10, 15);
      tft.setTextColor(YELLOW);
      tft.setFont(&FreeSerifBold9pt7b);
      tft.println("Inside Temperature");
      tft.setCursor(105, 38);
      tft.setFont(&FreeSerif9pt7b);
      tft.print(" o");

      tft.setCursor(105, 60);
      tft.setFont(&FreeSerif24pt7b);
      tft.print(" C");

      tft.setCursor(10, 100);
      tft.setTextColor(CYAN);
      tft.setFont(&FreeSerifBold9pt7b);
      tft.println("Outside Temperature");
      tft.setCursor(67, 124);
      tft.setFont(&FreeSerif9pt7b);
      tft.print("o");

      tft.setCursor(73, 130);
      tft.setFont(&FreeSerifBold9pt7b);
      tft.print(" C /        %RH");

      tft.setCursor(10, 160);
      tft.setTextColor(GREEN);
      tft.println("Operating temperature");
      tft.setTextColor(RED);
      tft.setCursor(10, 190);
      tft.print("Low: ");
      tft.println(LowTemp);
      tft.setCursor(10, 220);
      tft.print("High: ");
      tft.println(HighTemp);

      //tft.fillCircle(272, 210, 20, BLUE);
      //tft.fillCircle(272, 210, 12, BLACK);

      tft.fillRoundRect(210, 190, 90, 40, 40, BLUE);
      tft.fillRoundRect(215, 195, 80, 30, 40, BLACK);

      tft.setTextColor(CYAN);
      tft.setFont(&FreeSerifBold9pt7b);
      tft.setCursor(225, 213);
      tft.print("Settings");
      doorClosed();
    }

    void emergencyDoorOpen(bool emergency, uint64_t now) {
      if (emergency) {
        if (now - prev > 1000) {
          prev = now;

          if (ecolor) {
            tft.fillScreen(BLACK);
            doorOpened();
            ecolor = false;
            tft.setTextColor(RED);
            tft.setCursor(10, 190);
            tft.print("Low: ");
            tft.println(LowTemp);
            tft.setCursor(10, 220);
            tft.print("High: ");
            tft.println(HighTemp);
          }
          else {
            ecolor = true;
            tft.fillScreen(RED);
            tft.fillRect(215, 15, 80, 150, YELLOW);
            tft.fillCircle(225, 85, 4, BLACK);

            tft.setCursor(235, 45);
            tft.setTextColor(BLACK);
            tft.setFont(&FreeSerifBold9pt7b);
            tft.print("Door");
            tft.setCursor(225, 70);
            tft.print("Opened");

            tft.setTextColor(BLACK);
            tft.setCursor(10, 190);
            tft.print("Low: ");
            tft.println(LowTemp);
            tft.setCursor(10, 220);
            tft.print("High: ");
            tft.println(HighTemp);
          }
          tft.setCursor(10, 15);
          tft.setTextColor(YELLOW);
          tft.setFont(&FreeSerifBold9pt7b);
          tft.println("Inside Temperature");
          tft.setCursor(105, 38);
          tft.setFont(&FreeSerif9pt7b);
          tft.print(" o");

          tft.setCursor(105, 60);
          tft.setFont(&FreeSerif24pt7b);
          tft.print(" C");

          tft.setCursor(10, 100);
          tft.setTextColor(CYAN);
          tft.setFont(&FreeSerifBold9pt7b);
          tft.println("Outside Temperature");
          tft.setCursor(67, 124);
          tft.setFont(&FreeSerif9pt7b);
          tft.print("o");

          tft.setCursor(73, 130);
          tft.setFont(&FreeSerifBold9pt7b);
          tft.print(" C /        %RH");

          tft.setCursor(10, 160);
          tft.setTextColor(GREEN);
          tft.println("Operating temperature");


          //tft.fillCircle(272, 210, 20, BLUE);
          //tft.fillCircle(272, 210, 12, BLACK);

          tft.fillRoundRect(210, 190, 90, 40, 40, BLUE);
          tft.fillRoundRect(215, 195, 80, 30, 40, BLACK);

          tft.setTextColor(CYAN);
          tft.setFont(&FreeSerifBold9pt7b);
          tft.setCursor(225, 213);
          tft.print("Settings");
        }
      }
    }

    void emergencyDoorOpen2(bool emergency, uint64_t now) {
      if (emergency) {
        if (touchPage == 0) {
          if (now - prev > 1000) {
            prev = now;
            if (ecolor) {
              doorOpened();
              ecolor = false;
            }
            else {
              ecolor = true;
              tft.fillRect(215, 15, 80, 150, YELLOW);
              tft.fillCircle(225, 85, 4, BLACK);

              tft.setCursor(235, 45);
              tft.setTextColor(BLACK);
              tft.setFont(&FreeSerifBold9pt7b);
              tft.print("Door");
              tft.setCursor(225, 70);
              tft.print("Opened");
            }
          }
        }
      }
    }

    int getTouchPage() {
      return touchPage;
    }

    int getDoorAlarmTime() {
      return doorAlarmTime;
    }

    void updateTemp(float insideTemp, float outsideTemp, bool door,  int humidity, uint64_t now) {

      touch();

      emergencyDoorOpen2(door, now);

      if (now - prev2 > 1100 && touchPage == 0) {
        prev2 = now;


        if (insideTemp > HighTemp || insideTemp < LowTemp) {
          emergency = true;
        }
        else {
          emergency = false;
        }

        if (emergency && xcolor == 0) {
          tft.setTextColor(YELLOW);
          tft.setCursor(105, 38);
          tft.setFont(&FreeSerif9pt7b);
          tft.print(" o");

          tft.setCursor(105, 60);
          tft.setFont(&FreeSerif24pt7b);
          tft.print(" C");
          xcolor = 1;
        }

        else if (emergency && xcolor == 1) {
          tft.setTextColor(RED);
          tft.setCursor(105, 38);
          tft.setFont(&FreeSerif9pt7b);
          tft.print(" o");

          tft.setCursor(105, 60);
          tft.setFont(&FreeSerif24pt7b);
          tft.print(" C");
          xcolor = 2;
        }

        else if (emergency && xcolor == 2) {
          tft.setTextColor(BLUE);
          tft.setCursor(105, 38);
          tft.setFont(&FreeSerif9pt7b);
          tft.print(" o");

          tft.setCursor(105, 60);
          tft.setFont(&FreeSerif24pt7b);
          tft.print(" C");
          xcolor = 0;
        }

        else {
          tft.setTextColor(YELLOW);
          tft.setCursor(105, 38);
          tft.setFont(&FreeSerif9pt7b);
          tft.print(" o");

          tft.setCursor(105, 60);
          tft.setFont(&FreeSerif24pt7b);
          tft.print(" C");
          xcolor = 1;
        }

        tft.fillRect(0, 25, 110, 45, BLACK);
        tft.fillRect(0, 105, 60, 30, BLACK);
        tft.fillRect(100, 105, 40, 30, BLACK);

        tft.setCursor(10, 60);
        tft.setFont(&FreeSerif24pt7b);
        tft.print((int)insideTemp);
        tft.setFont(&FreeSerif12pt7b);
        tft.print(".");
        int afterfloat = abs(100.0 * (insideTemp - (int)insideTemp));
        tft.print(afterfloat);

        tft.setCursor(10, 130);
        tft.setTextColor(CYAN);
        tft.setFont(&FreeSerif9pt7b);
        tft.print((int)outsideTemp);
        tft.print(".");
        afterfloat = abs(100.0 * (outsideTemp - (int)outsideTemp));
        tft.print(afterfloat);

        tft.setCursor(115, 130);
        tft.print(humidity);
      }
    }

    void doorClosed() {
      tft.fillRect(215, 15, 80, 150, LIME);
      tft.fillCircle(225, 85, 4, BLACK);

      tft.setCursor(235, 45);
      tft.setTextColor(BLACK);
      tft.setFont(&FreeSerifBold9pt7b);
      tft.print("Door");
      tft.setCursor(230, 70);
      tft.print("Closed");
    }

    void doorOpened() {
      tft.fillRect(215, 15, 80, 150, RED);
      tft.fillCircle(225, 85, 4, BLACK);

      tft.setCursor(235, 45);
      tft.setTextColor(BLACK);
      tft.setFont(&FreeSerifBold9pt7b);
      tft.print("Door");
      tft.setCursor(225, 70);
      tft.print("Opened");
    }

    void settingsButton() {
      tft.fillScreen(BLACK);
      tft.setCursor(0, 23);
      tft.setFont(&FreeSerif9pt7b);
      tft.setTextColor(RED);
      tft.println("   Temperature High");
      tft.setCursor(0, 58);
      tft.setTextColor(YELLOW);
      tft.println("   Temperature Low");
      tft.setCursor(0, 93);
      tft.setTextColor(LIME);
      tft.println("   LoRa Transmit");
      tft.setCursor(0, 128);
      tft.setTextColor(CYAN);
      tft.println("   LoRa Sleep Time");
      tft.setCursor(0, 163);
      tft.setTextColor(RED);
      tft.println("   Door Alarm Time");

      tft.fillRect(165, 0, 100, 175, CYAN);

      tft.drawFastHLine(165, 35, 100, BLACK);
      tft.drawFastHLine(165, 70, 100, BLACK);
      tft.drawFastHLine(165, 105, 100, BLACK);
      tft.drawFastHLine(165, 140, 100, BLACK);

      tft.setCursor(275, 23);
      tft.setFont(&FreeSerif9pt7b);
      tft.setTextColor(RED);
      tft.println(" C");
      tft.setCursor(275, 58);
      tft.setTextColor(YELLOW);
      tft.println(" C");
      tft.setCursor(275, 93);
      tft.setTextColor(LIME);
      tft.println("");
      tft.setCursor(275, 128);
      tft.setTextColor(CYAN);
      tft.println(" min");
      tft.setCursor(275, 163);
      tft.setTextColor(RED);
      tft.println(" min");

      tft.setCursor(200, 23);
      tft.setFont(&FreeSerifBold9pt7b);
      tft.setTextColor(BLACK);
      tft.print(HighTemp);
      tft.setCursor(200, 58);
      tft.print(LowTemp);
      tft.setCursor(200, 93);
      tft.print((transmit) ? "ON" : "OFF");
      tft.setCursor(200, 128);
      tft.print(sleepTime);
      tft.setCursor(200, 163);
      tft.print(doorAlarmTime);

      tft.fillRoundRect(210, 190, 90, 40, 40, BLUE);
      tft.fillRoundRect(215, 195, 80, 30, 40, BLACK);

      tft.setTextColor(CYAN);
      tft.setFont(&FreeSerifBold9pt7b);
      tft.setCursor(240, 213);
      tft.print("OK");
    }

    void keyboard(int SV) {
      settingsVal = SV;
      input = "";
      if (settingsVal == 1) {
        tft.fillScreen(BLACK);
        tft.setCursor(0, 23);
        tft.setFont(&FreeSerif9pt7b);
        tft.setTextColor(RED);
        tft.println("   Temperature High");

        tft.fillRect(165, 0, 100, 35, CYAN);

        tft.setCursor(275, 23);
        tft.println(" C");
      }
      else if (settingsVal == 2) {
        tft.fillScreen(BLACK);
        tft.setCursor(0, 23);
        tft.setFont(&FreeSerif9pt7b);
        tft.setTextColor(YELLOW);
        tft.println("   Temperature Low");

        tft.fillRect(165, 0, 100, 35, CYAN);

        tft.setCursor(275, 23);
        tft.println(" C");
      }
      else if (settingsVal == 3) {
        tft.fillRect(165, 72, 99, 30, CYAN);
        tft.setFont(&FreeSerifBold9pt7b);
        tft.setTextColor(BLACK);
        transmit = !transmit;
        tft.setCursor(200, 93);
        tft.print((transmit) ? "ON" : "OFF");
        return;
      }
      else if (settingsVal == 4) {
        tft.fillScreen(BLACK);
        tft.setCursor(0, 23);
        tft.setFont(&FreeSerif9pt7b);
        tft.setTextColor(CYAN);
        tft.println("   LoRa Sleep Time");

        tft.fillRect(165, 0, 100, 35, CYAN);

        tft.setCursor(275, 23);
        tft.println(" min");
      }
      else if (settingsVal == 5) {
        tft.fillScreen(BLACK);
        tft.setCursor(0, 23);
        tft.setFont(&FreeSerif9pt7b);
        tft.setTextColor(RED);
        tft.println("   Door Alarm Time");

        tft.fillRect(165, 0, 100, 35, CYAN);

        tft.setCursor(275, 23);
        tft.println(" min");
      }
      tft.fillRect(0, 40, 320, 5, CYAN);
      tft.fillRect(0, 89, 320, 5, CYAN);
      tft.fillRect(0, 138, 320, 5, CYAN);
      tft.fillRect(0, 187, 320, 5, CYAN);
      tft.fillRect(0, 236, 320, 5, CYAN);

      tft.fillRect(80, 40, 5, 200, CYAN);
      tft.fillRect(160, 40, 5, 200, CYAN);
      tft.fillRect(240, 40, 5, 200, CYAN);
      tft.fillRect(0, 40, 5, 200, CYAN);
      tft.fillRect(315, 40, 5, 200, CYAN);

      tft.setFont(&FreeSerifBold12pt7b);
      tft.setTextColor(CYAN);
      tft.setCursor(40, 70);
      tft.println("1");
      tft.setCursor(120, 70);
      tft.println("2");
      tft.setCursor(200, 70);
      tft.println("3");
      tft.setCursor(280, 70);
      tft.println("4");

      tft.setCursor(40, 119);
      tft.println("5");
      tft.setCursor(120, 119);
      tft.println("6");
      tft.setCursor(200, 119);
      tft.println("7");
      tft.setCursor(280, 119);
      tft.println("8");

      tft.setCursor(40, 168);
      tft.println("9");
      tft.setCursor(120, 168);
      tft.println("0");
      tft.setCursor(190, 168);
      tft.println("(-)");
      tft.setCursor(280, 168);
      tft.println(".");

      //tft.setCursor(40, 217);
      //tft.println("9");
      //tft.setCursor(120, 217);
      //tft.println("");
      tft.setCursor(170, 217);
      tft.println("Clear");
      tft.setCursor(260, 217);
      tft.println("OK");
    }

    void keys(int num) {
      if (num == 1) {
        input += "1";
      }
      else if (num == 2) {
        input += "2";
      }
      else if (num == 3) {
        input += "3";
      }
      else if (num == 4) {
        input += "4";
      }
      else if (num == 5) {
        input += "5";
      }
      else if (num == 6) {
        input += "6";
      }
      else if (num == 7) {
        input += "7";
      }
      else if (num == 8) {
        input += "8";
      }
      else if (num == 9) {
        input += "9";
      }
      else if (num == 10) {
        input += "0";
      }
      else if (num == 11) {
        input = "-" + input;
      }
      else if (num == 12) {
        input += ".";
      }
      else if (num == 13) {
        input = "";
      }
      else if (num == 14) {
        save();
        return;
      }

      tft.fillRect(165, 0, 100, 35, CYAN);
      tft.setFont(&FreeSerifBold9pt7b);
      tft.setTextColor(BLACK);
      tft.setCursor(200, 23);
      tft.print(input);
    }

    void save() {
      if (settingsVal == 1) {
        float x = input.toFloat();
        if (x <= 30 && x >= -20) {
          HighTemp = x;
        }
      }
      else if (settingsVal == 2) {
        float x = input.toFloat();
        if (x <= 30 && x >= -20) {
          LowTemp = x;
        }
      }
      else if (settingsVal == 4) {
        int x = input.toInt();
        if (x <= 20 && x >= 1) {
          sleepTime = x;
        }
      }
      else if (settingsVal == 5) {
        int x = input.toInt();
        if (x <= 5 && x >= 1) {
          doorAlarmTime = x;
        }
      }
      settingsButton();
    }

    void touched(int x, int y) {

      if (touchPage == 0) {
        if (x > 20 && x < 55 && y > 175 && y < 240) {
          settingsButton();
          touchPage = 1;
          delay(100);
        }
      }

      else if (touchPage == 1) {
        if (x > 20 && x < 55 && y > 175 && y < 240) {
          normalScreen();
          touchPage = 0;
          delay(100);
        }
        else if (x > 290 && x < 340 && y > 140 && y < 220) {
          keyboard(1);
          touchPage = 2;
          delay(100);
        }
        else if (x > 240 && x < 290 && y > 140 && y < 220) {
          keyboard(2);
          touchPage = 2;
          delay(100);
        }
        else if (x > 190 && x < 240 && y > 140 && y < 220) {
          keyboard(3);
          delay(100);
          touchPage = 1;
        }
        else if (x > 140 && x < 190 && y > 140 && y < 220) {
          keyboard(4);
          touchPage = 2;
          delay(100);
        }
        else if (x > 90 && x < 140 && y > 140 && y < 220) {
          keyboard(5);
          touchPage = 2;
          delay(100);
        }
      }

      else if (touchPage == 2) {
        if (x > 215 && x < 280 && y > 15 && y < 75) {
          keys(1);
          touchPage = 2;
          delay(100);
        }
        else if (x > 215 && x < 280 && y > 75 && y < 135) {
          keys(2);
          touchPage = 2;
          delay(100);
        }
        else if (x > 215 && x < 280 && y > 135 && y < 195) {
          keys(3);
          touchPage = 2;
          delay(100);
        }
        else if (x > 215 && x < 280 && y > 195 && y < 275) {
          keys(4);
          touchPage = 2;
          delay(100);
        }
        else if (x > 140 && x < 215 && y > 15 && y < 75) {
          keys(5);
          touchPage = 2;
          delay(100);
        }
        else if (x > 140 && x < 215 && y > 75 && y < 135) {
          keys(6);
          touchPage = 2;
          delay(100);
        }
        else if (x > 140 && x < 215 && y > 135 && y < 195) {
          keys(7);
          touchPage = 2;
          delay(100);
        }
        else if (x > 140 && x < 215 && y > 195 && y < 275) {
          keys(8);
          touchPage = 2;
          delay(100);
        }
        else if (x > 70 && x < 140 && y > 15 && y < 75) {
          keys(9);
          touchPage = 2;
          delay(100);
        }
        else if (x > 70 && x < 140 && y > 75 && y < 135) {
          keys(10);
          touchPage = 2;
          delay(100);
        }
        else if (x > 70 && x < 140 && y > 135 && y < 195) {
          keys(11);
          touchPage = 2;
          delay(100);
        }
        else if (x > 70 && x < 140 && y > 195 && y < 275) {
          keys(12);
          touchPage = 2;
          delay(100);
        }
        else if (x > 0 && x < 70 && y > 135 && y < 195) {
          keys(13);
          touchPage = 2;
          delay(100);
        }
        else if (x > 0 && x < 70 && y > 195 && y < 275) {
          keys(14);
          delay(100);
          touchPage = 1;
        }
      }
    }

    void touch() {
      digitalWrite(13, HIGH);
      TSPoint p;
      p = ts.getPoint();
      digitalWrite(13, LOW);
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);


      digitalWrite(YP, HIGH);
      digitalWrite(XM, HIGH);

      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
        p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
        touched(p.x, p.y);
      }
    }
};

#endif
