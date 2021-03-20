#include <Servo.h>
#include <DHT.h>

#define DHTPIN      2
#define DHTTYPE     DHT11
#define Bulb_pin    A2
#define Motor_pin   3
#define initialTemp 30
#define initialHum  50
#define Buzzer_pin  A1
#define AlarmPause  2 //minutes
#define TTemp       67 //degree celcious at which the buzzer will be on
#define rotateAngle 45
#define rotateAfter 2 //minutes
#define temp_offset 25

DHT dht(DHTPIN, DHTTYPE);
Servo motor;

bool      notSetTemp  = true;
bool      notSetHum   = true;
int   temperature = initialTemp;
int   humidity    = initialHum;
int   curr_temp   = 0;
int   curr_hum    = 0;
uint16_t  Button_val  = 0;
uint32_t  prevTime, prevTime2, prevTime3;

int direction     = 1;
int   initPos     = 0;
int   finalPos    = rotateAngle;


void setup()
{
  Serial.begin(9600);
  pinMode(Bulb_pin, OUTPUT);
  pinMode(Buzzer_pin, OUTPUT);

  motor.attach(Motor_pin);
  motor.write(0); //initial position
  dht.begin();
  Serial.print("Sebastian &");
  Serial.print("Nazmirul's ");
  Serial.print("Arduino Project:");
  Serial.println("Egg Incubator");

  setTemp();
  setHum();

  digitalWrite(Bulb_pin, LOW);

  delay(500);
  prevTime = millis();
  prevTime2 = millis();
  prevTime3 = millis();
}

void loop()
{
  curr_temp = dht.readTemperature() + temp_offset;
  curr_hum  = dht.readHumidity();

  //Updating Display
  if (millis() - prevTime > 1000) {
    Serial.print("Current Temperature: ");
    Serial.print(dht.readTemperature() + temp_offset);
    Serial.print(" degree Celsius ");
    Serial.print("\tCurrent Humidity: ");
    Serial.println(dht.readHumidity());
    Serial.print(" %");
    prevTime = millis();
  }

  //Code for Alarm
  if ((curr_temp > TTemp || curr_temp > temperature) && (millis() - prevTime3 > AlarmPause * 1000 * 60)) {
    digitalWrite(Buzzer_pin, HIGH);
    bool buzz = true;
    Serial.println("Input OK in the serial monitor.\n");
    while (!Serial.available()) {
      curr_temp = dht.readTemperature() + temp_offset;
      curr_hum  = dht.readHumidity();
      if (millis() - prevTime > 1000) {
        Serial.print("Current Temperature: ");
        Serial.print(dht.readTemperature() + temp_offset);
        Serial.print("\tCurrent Humidity: ");
        Serial.println(dht.readHumidity());
        prevTime = millis();
        if (buzz) {
          digitalWrite(Buzzer_pin, LOW);
          buzz = !buzz;
        }
        else {
          digitalWrite(Buzzer_pin, HIGH);
          buzz = !buzz;
        }
        Serial.println("Input OK in the serial monitor.\n");
      }
    }
    String x = Serial.readString();
    digitalWrite(Buzzer_pin, LOW);
    prevTime3 = millis();
  }

  //Code for Bulb
  if (curr_temp >= temperature) {
    digitalWrite(Bulb_pin, HIGH);
  }
  else {
    digitalWrite(Bulb_pin, LOW);
  }

  //Code for Servo
  if (millis() - prevTime2 > rotateAfter * 60 * 1000) {
    if (initPos < finalPos)
      for (initPos; initPos <= finalPos; initPos += direction) {
        motor.write(initPos);
        delay(50);
      }
    else
      for (initPos; initPos >= finalPos; initPos += direction) {
        motor.write(initPos);
        delay(50);
      }

    finalPos = initPos + direction * rotateAngle;

    if (finalPos > 180) {
      direction = -1;
      initPos = 180;
      finalPos = 180 - rotateAngle;
    }
    else if (finalPos < 0) {
      direction = 1;
      initPos = 0;
      finalPos = rotateAngle;
    }
    prevTime2 = millis();
  }
  delay(100);
}

void setTemp()
{
  if (notSetTemp) {
    Serial.print("Please input the Temperature: ");
    while (!Serial.available());
    String x = Serial.readString();
    temperature = x.toInt();
    notSetTemp  = false;
  }
}

void setHum()
{
  if (notSetHum) {
    Serial.print("Please input the Humidity: ");
    while (!Serial.available());
    String x = Serial.readString();
    humidity = x.toInt();
    notSetHum  = false;
  }
}
