/*
   Author: Md. Masrur Saqib, CSE-18, RUET
   Organization: Freelancer.com
   Date of creation: 20-Mar-21
   Project Name: Beehive scale (telemetry) with GSM
   Last Modification: 31-Mar-21
   Modified by: Md. Masrur Saqib
*/

#include <SD.h>
#include <DHT.h>
#include <SPI.h>
#include "HX711.h"
#include "Display.h"
#include "sim800L.h"
#include "timeStamp.h"
#include <TinyGPS++.h>
#include <ArduinoJson.h>

#define   scale_calibration_factor  1200
#define   DHT_TYPE                  DHT22 // or DHT11

String  Name_of_Scale       = "Beehive19";
String  textSendingNumber   = "+8801743473012";


uint8_t pushButtonPin   = 2;
uint8_t chipSelect      = 53;
uint8_t scaleDataPin    = 6;
uint8_t scaleClockPin   = 7;
uint8_t dhtPin          = 5;
uint8_t pirPin          = 4;
uint8_t pirState        = 0;

long      scaleOffset     = 0;
float     hum_val         = 0;
float     temp_val        = 0;
float     weight = 0, lastWeight = 0, weightDiff = 0;
float     Latitude = 0, Longitude = 0, Altitude = 0;
uint32_t  prev, prev2, prev3;
String    smsCmd = "";
String    operatorName, strength, DT, TS, caliData;

DHT                 dht(dhtPin, DHT_TYPE);
HX711               scale;
Display             dis;
sim800L             sim;
timeStamp           cTime;
TinyGPSPlus         gps;
DynamicJsonDocument doc(64);
DynamicJsonDocument doc2(1024);
DynamicJsonDocument docu(64);


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // BLUETOOTH MODULE SERIAL
  Serial2.begin(9600); // GPS MODULE SERIAL
  Serial3.begin(9600); // SIM MODULE SERIAL


  attachInterrupt(digitalPinToInterrupt(pushButtonPin), generateOffsetData, CHANGE);

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
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  File myFile = SD.open("calibration.txt");
  if (myFile) {
    Serial.println("calibration.txt:");
    while (myFile.available()) {
      caliData = myFile.read();
      Serial.println(caliData);
    }
    myFile.close();
  }
  else {
    Serial.println("error opening calibration.txt");
  }

  //Scale calibration
  dis.scaleCalib();
  scale.begin(scaleDataPin, scaleClockPin);
  scale.set_scale(scale_calibration_factor);
  if (caliData.length()) {
    deserializeJson(docu, caliData);
    scaleOffset = docu["scaleOffset"];
    scale.set_offset(scaleOffset);
  }
  else {
    generateOffsetData();
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
}

void loop() {

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
    weight = scale.get_units();
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

  smsCmd = sim.incomingProcess();
  if(smsCmd == "WEIGHT"){
    String data = Name_of_Scale + "\nW " + weight + "kg, "
                + 167 + " " + weightDiff + "kg \nTemp "
                + temp_val + 248 + "C, Hum " + hum_val + "%\n"
                + "Signal " + strength + "\n";

    Serial.println(data);
    sim.sendMessage(data);
    smsCmd = "";
  }
  else if(smsCmd == "GPS"){
    TS = cTime.getTimeStamp();
    String data = Name_of_Scale + "\nTime Stamp: " + TS
                  + "\nLatitude: " + Latitude + "\nLongitude: " 
                  + Longitude + "\nAltitude: " + Altitude
                  + "\n";

    Serial.println(data);
    sim.sendMessage(data);
    smsCmd = "";
  }
  else{
    smsCmd = "";
  }

  if(pirSensor()){
    String data = Name_of_Scale + "\nAlarm: Motion Detected!!"
                  + "\n";

    Serial.println(data);
    sim.sendMessage(data);
  }

}

void generateOffsetData() {
  String sdData = "";
  scale.tare();
  scaleOffset = scale.get_offset();
  doc["scaleOffset"] = scaleOffset;
  serializeJson(doc, sdData);

  File myFile = SD.open("calibration.txt");
  if (myFile) {
    myFile.println(sdData);
    Serial.println(sdData);
    myFile.close();
  }
  else {
    Serial.println("error opening calibration.txt");
  }
  delay(500);

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
