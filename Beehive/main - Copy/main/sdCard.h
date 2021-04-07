/*
   Author: Md. Masrur Saqib, CSE-18, RUET
   Organization: Gizan-Tech
   Date of creation: 05-Nov-20
   Project Name: Low Cost Air Purifier
   Last Modification: 13-Dec-20
   Modified by: Md. Masrur Saqib
*/

#ifndef __SDCARD_H__
#define __SDCARD_H__

#include<Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "Display.h"
#include <ArduinoJson.h>

Display             disp;
DynamicJsonDocument doc(64);

class sdCard {

    uint8_t   chipSelect      = 53;
    long      scaleOffset     = 0;
    String    caliData        = "";
    float     scaleData      = 1.0;

  public:

    sdCard(uint8_t chipSel) {
      chipSelect = chipSel;
    }

    void begin(){
      disp.begin2();
      readOffset();
      readScale();
    }

    long getOffset(){
      return scaleOffset;
    }

    float getScale(){
      return scaleData;
    }

    void readOffset() {
      if (!SD.begin(chipSelect)){
        Serial.println("Card failed, or not present");
        disp.showMid(0, "SD Card failed");
        disp.showMid(1, "Please Restart!!");
        delay(1000);
        while (1);
      }
      if (SD.exists("offset.txt")) {
        File myFile = SD.open("offset.txt");
        if (myFile) {
          caliData = "";
          while (myFile.available()) {
            caliData = myFile.read();
            Serial.println(caliData);
            if (caliData.length() > 0) {
              deserializeJson(doc, caliData);
              scaleOffset = doc["scaleOffset"];
              disp.showFront(0, "Scale offset = ");
              disp.showFront(1, (String)scaleOffset);
              delay(1000);
            }
            else {
              disp.showFront(0, "Scale offset");
              disp.showFront(1, "not found!!");
              delay(2000);
              disp.showFront(0, "Please press the");
              disp.showFront(1, "offset button!");
              delay(2000);
              return;
            }
          }
          myFile.close();
        }
        else {
          disp.showFront(0, "Error opening");
          disp.showFront(1, "the File!!");
          delay(2000);
          disp.showFront(0, "Please check");
          disp.showFront(1, "the SD card!");
          delay(2000);
          return;
        }
        myFile.close();
      }
      else {
        disp.showFront(0, "Offset file");
        disp.showFront(1, "not found!!");
        delay(2000);
        disp.showFront(0, "Please press the");
        disp.showFront(1, "offset button!");
        delay(2000);
        return;
      }
    }

    void readScale() {
      if (!SD.begin(chipSelect)){
        Serial.println("Card failed, or not present");
        disp.showMid(0, "SD Card failed");
        disp.showMid(1, "Please Restart!!");
        delay(1000);
        while (1);
      }
      if (SD.exists("scale.txt")) {
        File myFile = SD.open("scale.txt");
        if (myFile) {
          while (myFile.available()) {
            caliData = "";
            caliData = myFile.read();
            Serial.println(caliData);
            if (caliData.length() > 0) {
              deserializeJson(doc, caliData);
              scaleData = doc["scaleData"];
              disp.showFront(0, "Scale data = ");
              disp.showFront(1, (String)scaleData);
              delay(1000);
            }
            else {
              disp.showFront(0, "Scale Data");
              disp.showFront(1, "not found!!");
              delay(2000);
              disp.showFront(0, "Please press the");
              disp.showFront(1, "Scale button!");
              delay(2000);
              return;
            }
          }
          myFile.close();
        }
        else {
          disp.showFront(0, "Error opening");
          disp.showFront(1, "the File!!");
          delay(2000);
          disp.showFront(0, "Please check");
          disp.showFront(1, "the SD card!");
          delay(2000);
          return;
        }
        myFile.close();
      }
      else {
        disp.showFront(0, "Scale file");
        disp.showFront(1, "not found!!");
        delay(2000);
        disp.showFront(0, "Please press the");
        disp.showFront(1, "Scale button!");
        delay(2000);
        return;
      }
    }

    void generateScaleData(float sclData) {
      if (!SD.begin(chipSelect)){
        Serial.println("Card failed, or not present");
        disp.showMid(0, "SD Card failed");
        disp.showMid(1, "Please Restart!!");
        delay(1000);
        while (1);
      }
      String sdData = "";
      doc["scaleData"] = sclData;
      serializeJson(doc, sdData);
      if (SD.exists("scale.txt")) {
        SD.remove("scale.txt");
      }
      File myFile = SD.open("scale.txt");
      if (myFile) {
        disp.showFront(0, "Scale Data set to ");
        disp.showFront(1, (String)sclData);
        myFile.println(sdData);
        Serial.println(sdData);
        myFile.close();
        delay(500);
      }
      else {
          disp.showFront(0, "Error opening");
          disp.showFront(1, "the File!!");
          delay(2000);
          disp.showFront(0, "Please check");
          disp.showFront(1, "the SD card!");
          delay(2000);
          return;
        }
        myFile.close();
    }

    void generateOffsetData(long offsetDt) {
      if (!SD.begin(chipSelect)){
        Serial.println("Card failed, or not present");
        disp.showMid(0, "SD Card failed");
        disp.showMid(1, "Please Restart!!");
        delay(1000);
        while (1);
      }
      String sdData = "";
      doc["scaleOffset"] = offsetDt;
      serializeJson(doc, sdData);
      if (SD.exists("offset.txt")) {
        SD.remove("offset.txt");
      }
      
      File myFile = SD.open("offset.txt");
      if (myFile) {
        disp.showFront(0, "Scale offset set to ");
        disp.showFront(1, (String)offsetDt);
        myFile.println(sdData);
        Serial.println(sdData);
        myFile.close();
        delay(500);
      }
      else {
          disp.showFront(0, "Error opening");
          disp.showFront(1, "the File!!");
          delay(2000);
          disp.showFront(0, "Please check");
          disp.showFront(1, "the SD card!");
          delay(2000);
          return;
        }
      myFile.close();
    }

};

#endif
