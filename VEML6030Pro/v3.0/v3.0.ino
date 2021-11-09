#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#include "ClosedCube_TCA9546A.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Wire.h>




#define   I2C_Address         0x77    //Change this address for different sensors
#define   dataDelay           15      //second. (Data sending delay) Data will be sent after every 15 seconds


#define   connected_sensors   4       //Change this to change the number of connected sensors
volatile long sensor1, sensor2, sensor3, sensor4;

ClosedCube_TCA9546A tca9546a;
void veml6030_begin(float g, int t);
void ring();

//check function::
void check() {
  //conditions will be here:
  //demo conditions:
  if (Serial.available()) {
    while (Serial.available()) {
      Serial.read();
    }
    ring();
  }

  if (sensor3 < sensor4) {
    ring();
  }
}
/*
   Change the config below to change your esp-8266 wifi name
   and password. Or, Use the default to connect with Arduino
*/
#define   ssid                "Poco F1"
#define   password            "e1234567"


SparkFun_Ambient_Light light(I2C_Address);
DynamicJsonDocument doc(256);

WiFiServer server(80);
String header;

String        output;
float         gain = .125;     // Possible values: .125, .25, 1, 2
int           timer = 100;          // Possible integration timers in milliseconds: 800, 400, 200, 100, 50, 25
uint32_t      prev, prev2, prev3;
int           responseTime = 2000;//milli seconds

void setup() {
  ESP.eraseConfig();
  Serial.begin(115200);
  Wire.begin();
  delay(500);

  tca9546a.begin(I2C_Address);
  delay(100);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  prev = millis();
  prev2 = millis();
  ring();
}



void loop()
{
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    Serial.println("New Client.");
    prev3 = millis();
    while (client.connected() && millis() - prev3 < responseTime) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/plain");
            client.println("Connection: close");
            client.println();
            client.println(output.c_str());
            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    //client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  if (millis() - prev > (dataDelay * 1000)) {
    prev = millis();

    //tca9546a.selectChannel(0);
    //sensor1 = light.readLight();

    Serial.print("Sensor no: ");
    Serial.print(1);
    Serial.print("\tSensor value: ");
    Serial.print(sensor1);
    Serial.println(" Lux");
    delay(100);

    //tca9546a.selectChannel(1);
    //sensor2 = light.readLight();

    Serial.print("Sensor no: ");
    Serial.print(2);
    Serial.print("\tSensor value: ");
    Serial.print(sensor2);
    Serial.println(" Lux");
    delay(100);

    //tca9546a.selectChannel(2);
    //sensor3 = light.readLight();

    Serial.print("Sensor no: ");
    Serial.print(3);
    Serial.print("\tSensor value: ");
    Serial.print(sensor3);
    Serial.println(" Lux");
    delay(100);

    tca9546a.selectChannel(3);
    sensor4 = light.readLight();

    Serial.print("Sensor no: ");
    Serial.print(4);
    Serial.print("\tSensor value: ");
    Serial.print(sensor4);
    Serial.println(" Lux");
    delay(100);

    check();
  }
}

void ring() {
  doc["S"] = true;
  doc["T"] = prev;

  output = "";
  serializeJson(doc, Serial);
  serializeJson(doc, output);
}

void veml6030_begin(float g, int t)
{
  light.begin();
  light.setGain(g);
  light.setIntegTime(t);
}
