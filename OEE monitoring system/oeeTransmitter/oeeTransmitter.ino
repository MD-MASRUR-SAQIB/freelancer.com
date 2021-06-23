#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

#define current_sensor_pin  A0
#define ir_sensor_pin       4
#define push_button_pin     3

#define idea_cycle_time     50 //seconds, which means after every 50 seconds there will be a product count
uint64_t planned_production_time = 36000; //seconds which is equal to 10 Hours

uint64_t  cst, irt, pbt, lrt, onTime = 0;
int totalCount = 0, defectCount = 0;


float current_sensor_offset = 2500.0; //offset
float current_sensor_sensitivity = 100.0; // use 100 for 20A Module and 66 for 30A Module
float current_sensor_adcvalue = 0.0;
float current_sensor_voltage = 0.0;
float current_sensor_current = 0.0;
float offCurrent = 0.5;

bool lastState, currState;

float performance = 0;
float availability = 0;
float quality = 0;
String datapacket;

DynamicJsonDocument doc(64);

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Started transmission.");

  pinMode(current_sensor_pin, INPUT);
  pinMode(ir_sensor_pin, INPUT);
  pinMode(push_button_pin, INPUT_PULLUP);

  cst = millis();
  irt = millis();
  pbt = millis();
  lrt = millis();
}

void loop()
{
  if (millis() - cst > 1000) {
    cst = millis();
    Serial.print("Current value: ");
    Serial.println(get_current());
    if (get_current() > offCurrent) {
      onTime += 1;
    }
  }

  if (millis() - irt > 1000) {
    irt = millis();
    get_ir();
  }

  if (digitalRead(push_button_pin) == LOW) {
    if (millis() - pbt > 500) {
      pbt = millis();
      defectCount++;
      Serial.print("Bad count: ");
      Serial.println((int)defectCount);
    }
  }

  performance = (1.0 * idea_cycle_time * totalCount ) / onTime;
  availability = (1.0 * onTime) / planned_production_time;
  quality = 1.0 - (1.0 * defectCount / totalCount);

  if (millis() - lrt > 5000) {
    datapacket = "";
    lrt = millis();
    doc["per"] = performance;
    doc["ava"] = availability;
    doc["qty"] = quality;
    serializeJson(doc, datapacket);
    Serial.println(datapacket);
    LoRa.beginPacket();
    LoRa.print(datapacket);
    LoRa.endPacket();
    Serial.println("Data Send by LoRa!!");
  }
}

double get_current() {
  double average = 0;
  for (int i = 1 ; i <= 25 ; i++) {
    current_sensor_adcvalue = analogRead(current_sensor_pin);
    current_sensor_voltage = ((current_sensor_adcvalue * 5000.0) / 1024.0);
    current_sensor_current = ((current_sensor_voltage - current_sensor_offset) / current_sensor_sensitivity);
    average += current_sensor_current;
  }
  return average / 25.0;
}

void get_ir() {
  currState = digitalRead(ir_sensor_pin);
  if (lastState != currState && currState == LOW) {
    totalCount++;
    Serial.print("Total count: ");
    Serial.println((int)totalCount);
  }
  lastState = currState;
}
