/*
   Author: Md. Masrur Saqib, CSE-18, RUET
   Organization: Freelancer.com
   Date of creation: 20-Mar-21
   Project Name: Beehive scale (telemetry) with GSM
   Last Modification: 3-Mar-21
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


//Change DHT_TYPE if needed
#define   DHT_TYPE          DHT22 // or DHT22

//Change scale name & the number where the text will be sent!
String  Name_of_Scale       = "NAME_OF_SCALE";
String  textSendingNumber   = "NUM1";
String  textSendingNumber2  = "NUM2";

//Change Regular message sending hour:minute
int hh = 00, mm = 05; // This line will send message everyday at 12:05 AM
int hh2 = 13, mm2 = 30; // This line will send message everyday at 01:30 PM


//Change SD card data logging time
uint64_t sdDataLog      = 30;//minutes

//Change pir motion sensing stating time from switching the power on
uint64_t pirSenseAfter  = 30;//minutes


uint8_t pushButtonPin   = 2;
uint8_t pushButton2Pin  = 3;
uint8_t pushButton3Pin  = 8;
uint8_t pushButton4Pin  = 9;
uint8_t chipSelect      = 53;
uint8_t scaleDataPin    = 6;
uint8_t scaleClockPin   = 7;
uint8_t dhtPin          = 5;
uint8_t pirPin          = 4;

long      scaleOffset    = 0;
float     scaleData      = 1.0;

int       prevDate = 0, newDate = 0;
uint8_t   pirState        = 0;
float     hum_val         = 0;
float     temp_val        = 0;
float     weight = 0, lastWeight = 0, weightDiff = 0;
float     Latitude = 0.0, Longitude = 0.0, Altitude = 0;
int       disShowing = 0;
uint32_t  prev, prev2, prev3, prev4, prev5, prev6;
String    smsCmd = "";
String    operatorName, strength, DT, TS, caliData;
bool      disState = true;

DHT                 dht(dhtPin, DHT_TYPE);
HX711               scale;
Display             dis;
sim800L             sim;
timeStamp           cTime;
TinyGPSPlus         gps;
DynamicJsonDocument docsd(64);
DynamicJsonDocument docgo(64);
DynamicJsonDocument doc2(1024);


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // BLUETOOTH MODULE SERIAL
  Serial2.begin(9600); // GPS MODULE SERIAL
  Serial3.begin(9600); // SIM MODULE SERIAL

  dis.begin(Name_of_Scale);

  //Sim Module initialization
  sim.begin();
  operatorName = sim.getOperator();
  strength = sim.getStrength();
  dis.simStat(operatorName, strength);

  //RTC Module initialization and callibrating with sim module time
  DT = sim.getDT();
  cTime.setRTC(DT);
  TS = cTime.getTimeStamp() + " " + cTime.getFullDate();
  dis.rtcStat(TS);

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


  //SD card module initialization
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    dis.showMid(0, "SD Card failed");
    dis.showMid(1, "Please Restart!!");
    delay(1000);
    while (1);
  }
  readOffset();
  readScale();

  //Scale Calibration
  scale.begin(scaleDataPin, scaleClockPin);
  if (scale.is_ready()) {
    scale.set_scale(scaleData);
    scale.set_offset(scaleOffset);
  }
  else {
    dis.showMid(0, "Scale not");
    dis.showMid(1, "connected!!");
    delay(2000);
  }
  delay(1000);

  pinMode(pushButtonPin, INPUT);
  pinMode(pushButton2Pin, INPUT);
  pinMode(pushButton3Pin, INPUT);
  pinMode(pushButton4Pin, INPUT);

  prev = millis();
  prev2 = millis();
  prev3 = millis();
  prev4 = millis();
  prev5 = millis();
  prev6 = millis();
}

void loop() {
  if (digitalRead(pushButtonPin) == HIGH) {
    changeScale();
  }
  if (digitalRead(pushButton2Pin) == HIGH) {
    makeScaleZero();
  }
  if (digitalRead(pushButton3Pin) == HIGH) {
    lcdLed();
  }

  if (millis() - prev4 > 5000) {
    prev4 = millis();
    if (disShowing == 0) {
      String lat = "Latitude :" + (String)Latitude;
      String lon = "Longitude:" + (String)Longitude;
      dis.showFront(0, lat);
      dis.showFront(1, lon);
      disShowing++;
    }
    else if (disShowing == 1) {
      String lat = "Weight:" + (String)weight + " kg";
      String lon = "Differ:" + (String)weightDiff + " kg";
      dis.showFront(0, lat);
      dis.showFront(1, lon);
      disShowing++;
    }
    else if (disShowing == 2) {
      String lat = "Temp:" + (String)temp_val + " C";
      String lon = "Humi:" + (String)hum_val + " %";
      dis.showFront(0, lat);
      dis.showFront(1, lon);
      disShowing++;
    }
    else if (disShowing == 3) {
      String lat = "Signal:" + (String)strength;
      String lon = cTime.getTimeStamp() + " " + cTime.getFullDate();
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
        //Serial.println("Location: Not Available");
      }
  }

  while (millis() - prev > 5000) {
    prev = millis();
    weight = scale.get_units();
    weightDiff = weight - lastWeight;
    temp_val  = trunk(dht.readTemperature());
    hum_val   = trunk(dht.readHumidity());
  }

  while (millis() - prev2 > sdDataLog * 1000 * 60) {
    prev2 = millis();
    String sdData = "";
    strength = sim.getStrength();
    operatorName = sim.getOperator();
    TS = "Date: " + cTime.getFullDate() +
         ", Time: " + cTime.getTimeStamp();
    doc2["Time_Stamp"] = TS;

    doc2["Weight"] = weight;
    doc2["Weight_Difference"] = weightDiff;

    doc2["Temperature"] = temp_val;
    doc2["Humidity"] = hum_val;

    doc2["Latitude"] = Latitude;
    doc2["Longitude"] = Longitude;

    doc2["Signal_Strength"] = strength;


    serializeJson(doc2, sdData);

    sdData += "\n";

    String fileName = cTime.getFullDateName() + ".txt";

    Serial.println(fileName);

    File myFile = SD.open(fileName, FILE_WRITE);
    if (myFile) {
      myFile.println(sdData);
      Serial.println(sdData);
      myFile.close();
    }
    else {
      Serial.println("error opening .txt file");
    }
    delay(500);

  }

  dailyMessageCheck(hh, mm);
  dailyMessageCheck(hh2, mm2);

  if ( millis() - prev6 > pirSenseAfter * 60 * 1000) {
    if (pirSensor()) {
      if (millis() - prev3 > 120000) {
        prev3 = millis();
        String data = Name_of_Scale + "\nAlarm: PIR Motion Detected!!"
                      + "\nLatitude: " + Latitude + "\nLongitude: "
                      + Longitude + "\nAltitude: " + Altitude
                      + "\n";
        Serial.println(data);

        sim.sendMessage(data, textSendingNumber);
        delay(5000);
        sim.sendMessage(data, textSendingNumber2);


        data = "Sending alert to " + textSendingNumber + " "
               + textSendingNumber2 + "\n" + data + "\n";

        File myFile = SD.open("smslog.txt", FILE_WRITE);
        if (myFile) {
          myFile.println(data);
          Serial.println(data);
          myFile.close();
        }
        else {
          Serial.println("error opening smslog.txt");
        }
        delay(1000);

      }
    }
  }

  //SMS check

  smsCmd = sim.incomingProcess();
  if (smsCmd == "WEIGHT") {

    dis.showFront(0, "Incoming Command");
    dis.showFront(1, "?WEIGHT?");

    TS = "Date: " + cTime.getFullDate() +
         ", Time: " + cTime.getTimeStamp()
         ;

    String data = Name_of_Scale + "\n" + TS + "\nW " + weight + "kg, "
                  + "Diff " + weightDiff + "kg \nTemp "
                  + temp_val + " C, Hum " + hum_val + "%\n"
                  + "Signal " + strength + "\n";

    Serial.println(data);
    sim.sendMessage(data, textSendingNumber2);
    delay(5000);
    sim.sendMessage(data, textSendingNumber);
    data = "Incoming message info: " + sim.saveMessage()
           + "\nSending reply to " + textSendingNumber + " "
           + textSendingNumber2 + "\n" + data + "\n";

    File myFile = SD.open("smslog.txt", FILE_WRITE);
    if (myFile) {
      myFile.println(data);
      Serial.println(data);
      myFile.close();
    }
    else {
      Serial.println("error opening smslog.txt");
    }
    delay(1000);

    smsCmd = "";
  }
  else if (smsCmd == "GPS") {
    dis.showFront(0, "Incoming Command");
    dis.showFront(1, "?GPS?");

    TS = "Date: " + cTime.getFullDate() +
         ", Time: " + cTime.getTimeStamp()
         ;
    String data = Name_of_Scale + "\n" + TS
                  + "\nLatitude: " + Latitude + "\nLongitude: "
                  + Longitude + "\nAltitude: " + Altitude
                  + "\n";

    Serial.println(data);

    sim.sendMessage(data, textSendingNumber);
    delay(5000);
    sim.sendMessage(data, textSendingNumber2);


    data = "Incoming message info: " + sim.saveMessage()
           + "\nSending reply to " + textSendingNumber + " "
           + textSendingNumber2 + "\n" + data + "\n";

    File myFile = SD.open("smslog.txt", FILE_WRITE);
    if (myFile) {
      myFile.println(data);
      Serial.println(data);
      myFile.close();
    }
    else {
      Serial.println("error opening smslog.txt");
    }
    delay(500);

    smsCmd = "";
  }

  else if (smsCmd == "VALIDATE") {
    dis.showFront(0, "Incoming Command");
    dis.showFront(1, "?VALIDATE?");


    String data = Name_of_Scale + "\n"
                  + sim.getVali()
                  + "\n";

    Serial.println(data);
    delay(5000);
    sim.sendMessage(data, textSendingNumber);
    delay(5000);
    sim.sendMessage(data, textSendingNumber2);


    data = "Incoming message info: " + sim.saveMessage()
           + "\nSending reply to " + textSendingNumber + " "
           + textSendingNumber2 + "\n" + data + "\n";

    File myFile = SD.open("smslog.txt", FILE_WRITE);
    if (myFile) {
      myFile.println(data);
      Serial.println(data);
      myFile.close();
    }
    else {
      Serial.println("error opening smslog.txt");
    }
    delay(500);

    smsCmd = "";
  }

  else {
    smsCmd = "";
  }
}


void lcdLed() {
  if (millis() - prev5 > 1500) {
    prev5 = millis();
    if (disState) {
      disState = !disState;
      dis.off();
    }
    else {
      disState = !disState;
      dis.on();
    }
  }
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

  scaleData = scale.get_value(10);
  scale.set_scale(scaleData);
  generateScaleData(scaleData);

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
  scale.tare();
  scaleOffset = scale.get_offset();
  generateOffsetData(scaleOffset);
}

void dailyMessageCheck(int h1, int m1) {
  newDate = cTime.getDateOnly();
  if (prevDate != newDate && newDate != -1) {
    if (cTime.getHourOnly() == h1 && cTime.getHourOnly() != -1) {
      if (cTime.getMinutesOnly() >= m1 && cTime.getMinutesOnly() != -1) {
        String data = "Daily Update:\n" + Name_of_Scale + "\nW " + weight + "kg, "
                      + "Weight Difference " + weightDiff + "kg \nTemp "
                      + temp_val + " C, Hum " + hum_val + "%\n"
                      + "Signal " + strength + "\n";

        Serial.println(data);
        lastWeight = weight;

        sim.sendMessage(data, textSendingNumber);
        delay(5000);
        sim.sendMessage(data, textSendingNumber2);

        prevDate = newDate;

        data = "Sending daily update to " + textSendingNumber + " "
               + textSendingNumber2 + "\n" + data + "\n";

        File myFile = SD.open("smslog.txt", FILE_WRITE);
        if (myFile) {
          myFile.println(data);
          Serial.println(data);
          myFile.close();
        }
        else {
          Serial.println("error opening smslog.txt");
        }
        delay(1000);

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

void readOffset() {
  char next;
  if (SD.exists("offset.txt")) {
    File myFile = SD.open("offset.txt");
    if (myFile) {
      caliData = "";
      while (myFile.available()) {
        next = myFile.read();
        caliData += (String)next;
        //
        if (caliData.length() > 0) {
          deserializeJson(docgo, caliData);
          scaleOffset = docgo["scaleOffset"];
          dis.showFront(0, "Scale offset = ");
          dis.showFront(1, (String)scaleOffset);
          delay(1000);
        }
        else {
          dis.showFront(0, "Scale offset");
          dis.showFront(1, "not found!!");
          delay(2000);
          dis.showFront(0, "Please press the");
          dis.showFront(1, "offset button!");
          delay(2000);
        }
      }
      Serial.println(caliData);
      myFile.close();
    }
    else {
      dis.showFront(0, "Error opening");
      dis.showFront(1, "the File!!");
      delay(2000);
      dis.showFront(0, "Please check");
      dis.showFront(1, "the SD card!");
      delay(2000);
    }
  }
  else {
    dis.showFront(0, "Offset file");
    dis.showFront(1, "not found!!");
    delay(2000);
    dis.showFront(0, "Please press the");
    dis.showFront(1, "offset button!");
    delay(2000);
  }
}

void readScale() {
  char next;
  if (SD.exists("scale.txt")) {
    File myFile = SD.open("scale.txt");
    if (myFile) {
      caliData = "";
      while (myFile.available()) {
        next = myFile.read();
        caliData += (String)next;

        if (caliData.length() > 0) {
          deserializeJson(docsd, caliData);
          scaleData = docsd["scaleData"];
          dis.showFront(0, "Scale data = ");
          dis.showFront(1, (String)scaleData);
          delay(1000);
        }
        else {
          dis.showFront(0, "Scale Data");
          dis.showFront(1, "not found!!");
          delay(2000);
          dis.showFront(0, "Please press the");
          dis.showFront(1, "Scale button!");
          delay(2000);
        }
      }
      myFile.close();
    }
    else {
      dis.showFront(0, "Error opening");
      dis.showFront(1, "the File!!");
      delay(2000);
      dis.showFront(0, "Please check");
      dis.showFront(1, "the SD card!");
      delay(2000);
    }
    Serial.println(caliData);
  }
  else {
    dis.showFront(0, "Scale file");
    dis.showFront(1, "not found!!");
    delay(2000);
    dis.showFront(0, "Please press the");
    dis.showFront(1, "Scale button!");
    delay(2000);
  }
}

void generateScaleData(float sclData) {
  String sdData = "";
  docsd["scaleData"] = sclData;
  serializeJson(docsd, sdData);
  if (SD.exists("scale.txt")) {
    SD.remove("scale.txt");
  }
  File myFile = SD.open("scale.txt", FILE_WRITE);
  if (myFile) {
    dis.showFront(0, "Scale Data set to ");
    dis.showFront(1, (String)sclData);
    myFile.println(sdData);
    Serial.println(sdData);
    myFile.close();
    delay(500);
  }
  else {
    dis.showFront(0, "Error opening");
    dis.showFront(1, "the File!!");
    delay(2000);
    dis.showFront(0, "Please check");
    dis.showFront(1, "the SD card!");
    delay(2000);
  }
}

void generateOffsetData(long offsetDt) {
  String sdData = "";
  docgo["scaleOffset"] = offsetDt;
  serializeJson(docgo, sdData);
  if (SD.exists("offset.txt")) {
    SD.remove("offset.txt");
  }

  File myFile = SD.open("offset.txt", FILE_WRITE);
  if (myFile) {
    dis.showFront(0, "Scale offset set to ");
    dis.showFront(1, (String)offsetDt);
    myFile.println(sdData);
    Serial.println(sdData);
    myFile.close();
    delay(500);
  }
  else {
    dis.showFront(0, "Error opening");
    dis.showFront(1, "the File!!");
    delay(2000);
    dis.showFront(0, "Please check");
    dis.showFront(1, "the SD card!");
    delay(2000);
  }
}
