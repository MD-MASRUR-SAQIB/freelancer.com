#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>


#define   I2C_Address         0x48    //Change this address for different sensors
#define   dataDelay           15      //second. (Data sending delay) Data will be sent after every 15 seconds


#define   connected_sensors   4       //Change this to change the number of connected sensors
volatile long sensor1, sensor2, sensor3, sensor4;


void TCA9548A(uint8_t bus);
void veml6030_begin(float g, int t);
void ring();

//check function::
void check() {
  //conditions will be here:
  //demo conditions:
  if (sensor1 > sensor2) {
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
#define   ssid                "Greenhouse1"
#define   password            "123456789"



IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
SparkFun_Ambient_Light light(I2C_Address);
DynamicJsonDocument doc(256);
ESP8266WebServer server(80);


String        output;
float         gain = .125;     // Possible values: .125, .25, 1, 2
int           timer = 100;          // Possible integration timers in milliseconds: 800, 400, 200, 100, 50, 25
uint32_t      prev, prev2;

void setup() {
  ESP.eraseConfig();
  Serial.begin(115200);
  Wire.begin();
  delay(500);

  for (uint8_t i = 0 ; i < connected_sensors ; i++) {
    TCA9548A(i);
    veml6030_begin(gain, timer);
    delay(100);
  }
  Serial.println();
  Serial.println("Setting AP (Access Point)…");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", output.c_str());
  });
  server.begin();
  prev = millis();
  prev2 = millis();
}

void loop()
{
  
  if (millis() - prev > (dataDelay * 1000)) {
    prev = millis();

    TCA9548A(0);
    sensor1 = light.readLight();

    Serial.print("Sensor no: ");
    Serial.print(1);
    Serial.print("\tSensor value: ");
    Serial.print(sensor1);
    Serial.println(" Lux");
    delay(100);

    TCA9548A(1);
    sensor2 = light.readLight();

    Serial.print("Sensor no: ");
    Serial.print(2);
    Serial.print("\tSensor value: ");
    Serial.print(sensor2);
    Serial.println(" Lux");
    delay(100);

    TCA9548A(2);
    sensor3 = light.readLight();

    Serial.print("Sensor no: ");
    Serial.print(3);
    Serial.print("\tSensor value: ");
    Serial.print(sensor3);
    Serial.println(" Lux");
    delay(100);

    TCA9548A(3);
    sensor4 = light.readLight();

    Serial.print("Sensor no: ");
    Serial.print(4);
    Serial.print("\tSensor value: ");
    Serial.print(sensor4);
    Serial.println(" Lux");
    delay(100);

    check();
  }
  server.handleClient();
}

void ring() {
  doc["S"] = true;
  doc["T"] = prev;

  output = "";
  serializeJson(doc, output);
  Serial.println(output);
}

void veml6030_begin(float g, int t)
{
  light.begin();
  light.setGain(g);
  light.setIntegTime(t);
}

void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}
