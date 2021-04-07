/*
   Author: Md. Masrur Saqib, CSE-18, RUET
   Organization: Freelancer.com
   Date of creation: 20-Mar-21
   Project Name: Beehive scale (telemetry) with GSM
   Last Modification: 3-Mar-21
   Modified by: Md. Masrur Saqib
*/


#include <DHT.h>
#include <SPI.h>
#include "HX711.h"
#include "sdCard.h"
#include "Display.h"
#include "sim800L.h"
#include "timeStamp.h"
#include <TinyGPS++.h>
#include <ArduinoJson.h>

//
#define   DHT_TYPE                  DHT11 // or DHT22

String  Name_of_Scale       = "Beehive19";
String  textSendingNumber   = "01743473012";

//Regular message sending hour:minute
int hh = 13, mm = 30; // This line will send message everyday at 01:30 PM


uint8_t pushButtonPin   = 2;
uint8_t pushButton2Pin  = 3;
uint8_t chipSelect      = 53;
uint8_t scaleDataPin    = 6;
uint8_t scaleClockPin   = 7;
uint8_t dhtPin          = 5;
uint8_t pirPin          = 4;


//
long      scaleOffset     = 0;
float     scaleVal        = 1.0;

int       prevDate = 0, newDate = 0;
uint8_t   pirState        = 0;
float     hum_val         = 0;
float     temp_val        = 0;
float     weight = 0, lastWeight = 0, weightDiff = 0;
float     Latitude = 65.9, Longitude = 23.6, Altitude = 0;
int       disShowing = 0;
uint32_t  prev, prev2, prev3, prev4;
String    smsCmd = "";
String    operatorName, strength, DT, TS, caliData;

DHT                 dht(dhtPin, DHT_TYPE);
HX711               scale;
sdCard              memory(chipSelect);
Display             dis;
sim800L             sim;
timeStamp           cTime;
TinyGPSPlus         gps;
DynamicJsonDocument doc2(1024);


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // BLUETOOTH MODULE SERIAL
  Serial2.begin(9600); // GPS MODULE SERIAL
  Serial3.begin(9600); // SIM MODULE SERIAL

  dis.begin(Name_of_Scale);

  //Sim Module initialization
  sim.begin(textSendingNumber);
  operatorName = sim.getOperator();
  strength = sim.getStrength();
  dis.simStat(operatorName, strength);

  //RTC Module initialization and callibrating with sim module time
  DT = sim.getDT();
  cTime.setRTC(DT);
  TS = cTime.getTimeStamp();
  dis.rtcStat(TS);

  //SD card module initialization
  memory.begin();
  scaleOffset = memory.getOffset();
  scaleVal    = memory.getScale();

  //Scale Calibration
  scale.begin(scaleDataPin, scaleClockPin);
  if (scale.is_ready()) {
    scale.set_scale(scaleVal);
    scale.set_offset(scaleOffset);
  }
  else {
    dis.showMid(0, "Scale not found!");
    dis.showMid(1, " ");
    delay(2000);
  }


  //DHT 22 and IR sensor
  dis.initDht();
  dht.begin();
  temp_val  = trunk(dht.readTemperature());
  hum_val   = trunk(dht.readHumidity());

  if (isnan(temp_val) || isnan(hum_val)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    dis.dhtInitError();
  }

  pinMode(pirPin, INPUT);
  pirSensor();

  delay(2000);
  prev = millis();
  prev2 = millis();
  prev3 = millis();
  prev4 = millis();
}

void loop() {
  if(digitalRead(pushButtonPin) == HIGH){
    changeScale();
  }
  if(digitalRead(pushButton2Pin) == HIGH){
    makeScaleZero();
  }
  if (millis() - prev4 > 5000) {
    prev4 = millis();
    if (disShowing == 0) {
      String lat = "Latitude=" + (String)Latitude;
      String lon = "Longitude=" + (String)Longitude;
      dis.showFront(0, lat);
      dis.showFront(1, lon);
      disShowing++;
    }
    else if (disShowing == 1) {
      String lat = "Weight=" + (String)weight + " kg";
      String lon = "Diff=" + (String)weightDiff + " kg";
      dis.showFront(0, lat);
      dis.showFront(1, lon);
      disShowing++;
    }
    else if (disShowing == 2) {
      String lat = "Temp=" + (String)temp_val + " C";
      String lon = "Humi=" + (String)hum_val + " %";
      dis.showFront(0, lat);
      dis.showFront(1, lon);
      disShowing++;
    }
    else if (disShowing == 3) {
      String lat = "Signal=" + (String)strength;
      String lon = cTime.getTimeStamp();
      dis.showFront(0, lat);
      dis.showFront(1, lon);
      disShowing = 0;
    }
  }

  while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read()))
      if (gps.location.isValid()) {

        Latitude = gps.location.lat();
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);

        Longitude = gps.location.lng();
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);

        Altitude = gps.altitude.meters();
        Serial.print("Altitude: ");
        Serial.println(gps.altitude.meters());
      }
      else {
        Serial.println("Location: Not Available");
      }
  }

  while (millis() - prev > 5000) {
    prev = millis();
    weight = 5000;//scale.get_units();
    weightDiff = weight - lastWeight;
    temp_val  = trunk(dht.readTemperature());
    hum_val   = trunk(dht.readHumidity());
  }

  while (millis() - prev2 > 600000) {
    prev2 = millis();

    String sdData = "";
    strength = sim.getStrength();
    operatorName = sim.getOperator();
    TS = cTime.getTimeStamp();
    doc2["Time_Stamp"] = TS;
    doc2["Provider"] = operatorName;
    doc2["Signal_Strength"] = strength;
    doc2["Weight"] = weight;
    doc2["Weight_Difference"] = weightDiff;
    doc2["Temperature"] = temp_val;
    doc2["Humidity"] = hum_val;

    serializeJson(doc2, sdData);

    sdData += "\n";

    File myFile = SD.open("data.txt");
    if (myFile) {
      myFile.println(sdData);
      Serial.println(sdData);
      myFile.close();
    }
    else {
      Serial.println("error opening data.txt");
    }
    delay(500);
  }

  if (millis() - prev3 > 120000) {
    prev3 = millis();
    
    if (pirSensor()) {
      String data = Name_of_Scale + "\nAlarm: Motion Detected!!"
                    + "\n";

      Serial.println(data);
      sim.sendMessage(data);
    }
  }
  
  //SMS check

  smsCmd = sim.incomingProcess();
  if (smsCmd == "WEIGHT") {
    String data = Name_of_Scale + "\nW " + weight + "kg, "
                  + 167 + " " + weightDiff + "kg \nTemp "
                  + temp_val + 248 + "C, Hum " + hum_val + "%\n"
                  + "Signal " + strength + "\n";

    Serial.println(data);
    sim.sendMessage(data);
    smsCmd = "";
  }
  else if (smsCmd == "GPS") {
    TS = cTime.getTimeStamp();
    String data = Name_of_Scale + "\nTime Stamp: " + TS
                  + "\nLatitude: " + Latitude + "\nLongitude: "
                  + Longitude + "\nAltitude: " + Altitude
                  + "\n";

    Serial.println(data);
    sim.sendMessage(data);
    smsCmd = "";
  }
  else {
    smsCmd = "";
  }

  // Motion check


}


void changeScale() {
  Serial.println("1");
  delay(100);
  makeScaleZero();
  
  dis.showMid(0, "Please put 1 kg");
  dis.showMid(1, "weight on scale");
  delay(3000);
  dis.showFront(0, "Wait for");
  dis.showFront(1, "5 seconds!");
  delay(5000);
  dis.showFront(0, "Calebrating");
  dis.showFront(1, "the Scale...");
  delay(2000);

  //scaleVal = scale.get_value(10);
  scaleVal = 56;
  //scale.set_scale(scaleVal);
  memory.generateScaleData(scaleVal);

  dis.showFront(0, "Calebration");
  dis.showFront(1, "complete...");
  delay(2000);
}

void makeScaleZero() {
  Serial.println("2");

  dis.showMid(0, "Please Make sure");
  dis.showMid(1, "Scale is empty!");
  delay(3000);
  dis.showFront(0, "Wait for");
  dis.showFront(1, "5 seconds!");
  delay(5000);

  //scale.tare();
  //scaleOffset = scale.get_offset();
  scaleOffset = 5987;
  memory.generateOffsetData(scaleOffset);
}

void dailyMessageCheck() {
  newDate = cTime.getDateOnly();
  if (prevDate != newDate && newDate != -1) {
    if (cTime.getHourOnly() >= hh && cTime.getHourOnly() != -1) {
      if (cTime.getMinutesOnly() >= mm && cTime.getMinutesOnly() != -1) {
        String data = Name_of_Scale + "\nW " + weight + "kg, "
                      + 167 + " " + weightDiff + "kg \nTemp "
                      + temp_val + 248 + "C, Hum " + hum_val + "%\n"
                      + "Signal " + strength + "\n";

        Serial.println(data);
        lastWeight = weight;
        sim.sendMessage(data);
        prevDate = newDate;
      }
    }
  }
}

float trunk(float num) {
  uint64_t x = (num * 100);
  return x / 100.0;
}

bool pirSensor() {
  if (digitalRead(pirPin) == HIGH) {
    Serial.println("Motion detected.");
    dis.pirMotion();
    delay(50);
    return true;
  }
  return false;
}
