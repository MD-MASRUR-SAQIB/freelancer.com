#include <CayenneMQTTESP32.h>

#define CAYENNE_PRINT Serial
#define current_sensor_pin  32
#define ir_sensor_pin       34
#define push_button_pin     19

// WiFi network info.
char ssid[] = "Masrur Saqib";
char wifiPassword[] = "fatema03";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "f06af690-cd58-11eb-b767-3f1a8f1211ba";
char password[] = "68a8a8b01075c898d16b73620ca2a5eecec2204e";
char clientID[] = "1feb3e70-cdfe-11eb-b767-3f1a8f1211ba";

int idea_cycle_time = 50; //seconds, which means after every 50 seconds there will be a product count
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
float oee = 0;

void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);

  pinMode(current_sensor_pin, INPUT);
  pinMode(ir_sensor_pin, INPUT);
  pinMode(push_button_pin, INPUT_PULLUP);

  cst = millis();
  irt = millis();
  pbt = millis();
  lrt = millis();
}

void loop() {
  Cayenne.loop();

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

  if (millis() - lrt > 10000) {
    lrt = millis();
    oee = performance * availability * quality;
    Cayenne.virtualWrite(0, performance, "Performance", "null");
    Cayenne.virtualWrite(1, availability, "Availability", "null");
    Cayenne.virtualWrite(2, quality, "Quality", "null");
    Cayenne.virtualWrite(3, oee, "OEE", "null");
    Serial.println("Data Send to Cayenne Dashboard!!");
  }
}

double get_current() {
  double average = 0;
  for (int i = 1 ; i <= 25 ; i++) {
    current_sensor_adcvalue = analogRead(current_sensor_pin);
    current_sensor_voltage = ((current_sensor_adcvalue * 5000.0) / 4095.0);
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
