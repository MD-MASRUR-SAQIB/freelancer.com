#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>
#include <CayenneMQTTESP8266.h>

#define CAYENNE_PRINT Serial
#define ss 15
#define rst 16
#define dio0 2

// WiFi network info.
char ssid[] = "Masrur Saqib"; //WiFi name
char wifiPassword[] = "fatema03"; // WiFi password

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
// Register to Cayenne dashboard. Then at the dashboard select "Bring your own thing" and copy the credentials below.
char username[] = "f06af690-cd58-11eb-b767-3f1a8f1211ba";
char password[] = "68a8a8b01075c898d16b73620ca2a5eecec2204e";
char clientID[] = "e25f1540-cdbc-11eb-a2e4-b32ea624e442";

String incoming;

DynamicJsonDocument doc(128);

void setup() {
  Serial.begin(9600);
  Serial.println("LoRa Receiver");
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1){
      yield();
    }
    
  }
  Serial.println("LoRa Started!");

  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  Serial.println("Cayenne Connected!!");
}

void loop() {
  Cayenne.loop();
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    incoming = "";
    while (LoRa.available()) {
      incoming += LoRa.read();
    }
    Serial.println(incoming);
    deserializeJson(doc, incoming);
    
    float performance = doc["per"];
    float availability = doc["ava"];
    float quality = doc["qty"];

    Cayenne.virtualWrite(0, performance, "Performance", "null");
    Cayenne.virtualWrite(0, availability, "Availability", "null");
    Cayenne.virtualWrite(0, quality, "Quality", "null");
  }
  
}
