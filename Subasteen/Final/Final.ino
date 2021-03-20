#include <LiquidCrystal.h>
#include <Servo.h>
#include <DHT.h>

#define DHTPIN      2
#define DHTTYPE     DHT11
#define LCD_rs      8
#define LCD_en      9
#define LCD_d4      4
#define LCD_d5      5
#define LCD_d6      6
#define LCD_d7      7
#define Bulb_pin    A2
#define Button_pin  A0
#define Motor_pin   3
#define initialTemp 30
#define initialHum  50
#define Buzzer_pin  A1
#define AlarmPause  1 //minutes

#define rotateAngle 45
#define rotateAfter 2 //minutes

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(LCD_rs, LCD_en, LCD_d4, LCD_d5, LCD_d6, LCD_d7);
Servo motor;

bool      notSetTemp  = true;
bool      notSetHum   = true;
uint8_t   temperature = initialTemp;
uint8_t   humidity    = initialHum;
uint8_t   curr_temp   = 0;
uint8_t   curr_hum    = 0;
uint16_t  Button_val  = 0;
uint32_t  prevTime, prevTime2, prevTime3;

int direction     = 1;
int   initPos     = 0;
int   finalPos    = rotateAngle;


void setup()
{
  Serial.begin(9600);
  pinMode(Button_pin, INPUT);
  pinMode(Bulb_pin, OUTPUT);
  pinMode(Buzzer_pin, OUTPUT);

  motor.attach(Motor_pin);
  motor.write(0); //initial position
  dht.begin();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sebastian &");
  lcd.setCursor(0, 1);
  lcd.print("Nazmirul's ");
  delay (3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Arduino Project:");
  lcd.setCursor(0, 1);
  lcd.print("Egg Incubator");
  delay (3000);
  lcd.clear();
  setTemp();
  setHum();
  digitalWrite(Bulb_pin, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature    C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity       %");
  delay(500);
  prevTime = millis();
  prevTime2 = millis();
  prevTime3 = millis();
}

void loop()
{
  curr_temp = dht.readTemperature();
  curr_hum  = dht.readHumidity();

  //Updating Display
  if (millis() - prevTime > 1000) {
    lcd.setCursor(0, 12);
    lcd.print(curr_temp);
    lcd.setCursor(1, 12);
    lcd.print(curr_hum);
    prevTime = millis();
  }

  //Code for Alarm
  if (curr_hum > humidity && (millis() - prevTime3 > AlarmPause * 1000 * 60)) {
    //Alarm Start fan
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Humidity       %");
    lcd.setCursor(0, 0);
    lcd.print("Start the Fan!! ");
    digitalWrite(Buzzer_pin, HIGH);
    bool buzz = true;
    Button_val = analogRead(Button_pin);
    while (Button_val < 600 || Button_val > 800) {
      Button_val = analogRead(Button_pin);
      curr_hum   = dht.readHumidity();
      if (millis() - prevTime > 1000) {
        lcd.setCursor(1, 12);
        lcd.print(curr_hum);
        if (buzz) {
          digitalWrite(Buzzer_pin, LOW);
          buzz = !buzz;
        }
        else {
          digitalWrite(Buzzer_pin, HIGH);
          buzz = !buzz;
        }
        prevTime = millis();
      }
    }
    digitalWrite(Buzzer_pin, LOW);
    prevTime3 = millis();
  }
  else if (curr_hum < humidity && (millis() - prevTime3 > AlarmPause * 1000 * 60)) {
    //Alarm Stop fan
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Humidity       %");
    lcd.setCursor(0, 0);
    lcd.print("Stop the Fan!! ");
    digitalWrite(Buzzer_pin, HIGH);
    bool buzz = true;
    Button_val = analogRead(Button_pin);
    while (Button_val < 600 || Button_val > 800) {
      Button_val = analogRead(Button_pin);
      curr_hum   = dht.readHumidity();
      if (millis() - prevTime > 1000) {
        lcd.setCursor(1, 12);
        lcd.print(curr_hum);
        if (buzz) {
          digitalWrite(Buzzer_pin, LOW);
          buzz = !buzz;
        }
        else {
          digitalWrite(Buzzer_pin, HIGH);
          buzz = !buzz;
        }
        prevTime = millis();
      }
    }
    digitalWrite(Buzzer_pin, LOW);
    prevTime3 = millis();
  }

  //Code for Bulb
  if (curr_temp >= temperature) {
    digitalWrite(Bulb_pin, LOW);
  }
  else {
    digitalWrite(Bulb_pin, HIGH);
  }

  //Code for Servo
  if (millis() - prevTime2 > rotateAfter * 60 * 1000) {
    if (initPos < finalPos)
      for (initPos; initPos <= finalPos; initPos += direction) {
        motor.write(initPos);
        delay(60);
      }
    else
      for (initPos; initPos >= finalPos; initPos += direction) {
        motor.write(initPos);
        delay(60);
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
    lcd.setCursor(0, 0);
    lcd.print("Set Temperature:");
    lcd.setCursor(0, 1);
    lcd.print(initialTemp);
    lcd.print(" *C");
  }
  while (notSetTemp) {
    Button_val = analogRead(Button_pin);
    if (Button_val < 60) {
      Button_val  = 0;
    }
    else if (Button_val < 200) {
      temperature++;
      lcd.setCursor(0, 1);
      lcd.print(temperature);
      Button_val  = 0;
    }
    else if (Button_val < 400) {
      temperature--;
      lcd.setCursor(0, 1);
      lcd.print(temperature);
      Button_val  = 0;
    }
    else if (Button_val < 600) {
      Button_val  = 0;
    }
    else if (Button_val < 800) {
      notSetTemp  = false;
      Button_val  = 0;
      break;
    }
    else {
      Button_val  = 0;
    }
  }
}

void setHum()
{
  if (notSetHum) {
    lcd.setCursor(0, 0);
    lcd.print("Set Humidity:");
    lcd.setCursor(0, 1);
    lcd.print(initialHum);
    lcd.print(" %");
  }
  while (notSetHum) {
    Button_val = analogRead(Button_pin);
    if (Button_val < 60) {
      Button_val  = 0;
    }
    else if (Button_val < 200) {
      humidity++;
      lcd.setCursor(0, 1);
      lcd.print(humidity);
      Button_val  = 0;
    }
    else if (Button_val < 400) {
      humidity--;
      lcd.setCursor(0, 1);
      lcd.print(humidity);
      Button_val  = 0;
    }
    else if (Button_val < 600) {
      Button_val  = 0;
    }
    else if (Button_val < 800) {
      notSetHum  = false;
      Button_val  = 0;
      break;
    }
    else {
      Button_val  = 0;
    }
  }
}
