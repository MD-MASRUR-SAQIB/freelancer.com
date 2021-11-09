#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#include "ClosedCube_TCA9546A.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

#define   I2C_Address         0x77    //Change this address for different sensors
#define   dataDelay           10      //second. (Data sending delay) Data will be sent after every 15 seconds


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
    ring();
    Serial.println("Sending..");
    Serial.read();
  }
  if (sensor3 < sensor4) {
    ring();
  }
}
/*
   Change the config below to change your esp-8266 wifi name
   and password. Or, Use the default to connect with Arduino
*/
#define   ssid                "Greenhouse1"
#define   password            "123456789"


SparkFun_Ambient_Light light(I2C_Address);
DynamicJsonDocument doc(256);

WiFiServer server(80);
String header;

String        output;
float         gain = .125;     // Possible values: .125, .25, 1, 2
int           timer = 100;          // Possible integration timers in milliseconds: 800, 400, 200, 100, 50, 25
uint32_t      prev, prev2, prev3, prev4;
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
  Serial.println("Server OK");
  prev = millis();
  prev2 = millis();
  prev4 = millis() - 15000;
  ring();
  Serial.println("Ring function OK");
}

void loop()
{

  WiFiClient client = server.available();

  if (client) {
    Serial.println("if (client) true");

    String currentLine = "";
    Serial.println("New Client.");
    prev3 = millis();
    while (client.connected() && millis() - prev3 < responseTime) {
      //Serial.println("while (client.connected() && millis() - prev3 < responseTime) true");

      if (client.available()) {
        //Serial.println("client.available() true");
        char c = client.read();
        //Serial.write(c);
        header += c;
        if (c == '\n') {
          //Serial.println("if (c == '\n') true");
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/plain");
            client.println("Connection: close");
            client.println();
            client.println(output.c_str());
            break;
          }
          else {
           // Serial.println("if (c == '\n') false");
            currentLine = "";
          }
        }
        else if (c != '\r') {
         // Serial.println("else if (c == '\r') true");
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
   // Serial.println("");
  }
/*
  tca9546a.selectChannel(0);
  sensor1 = light.readLight();


  Serial.print("Sensor no: ");
  Serial.print(1);
  Serial.print("\tSensor value: ");
  Serial.print(sensor1);
  Serial.println(" Lux");


  delay(100);

  tca9546a.selectChannel(1);
  sensor2 = light.readLight();


  Serial.print("Sensor no: ");
  Serial.print(2);
  Serial.print("\tSensor value: ");
  Serial.print(sensor2);
  Serial.println(" Lux");


  delay(100);

  tca9546a.selectChannel(2);
  sensor3 = light.readLight();


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
*/

  delay(100);

  check();
}

void ring() {
  if (millis() - prev4 > 15000) {
    prev4 = millis();
    doc["S"] = true;
    doc["T"] = prev4;

    output = "";
    serializeJson(doc, output);
    serializeJson(doc, Serial);
  }
}

void veml6030_begin(float g, int t)
{
  light.begin();
  light.setGain(g);
  light.setIntegTime(t);
}
