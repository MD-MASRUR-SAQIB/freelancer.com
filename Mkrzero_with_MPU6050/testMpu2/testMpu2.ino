/*
 * Connect the MPU 6050 like below:
 * VCC: +3.3v
 * GND: gnd
 * SCL: D12
 * SDA: D11
 * INT: D0
 */

#include <Wire.h>
#include <ArduinoLowPower.h>

uint64_t prevTime, newTime;
uint64_t waitTime = 5000; //seconds wait before going to sleep
int      wakePin = 0;

//----------------- for internal timer ---------------------------

#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "SAMDTimerInterrupt.h"

#define TIMER1_INTERVAL_MS        3000

volatile uint32_t preMillisTimer1 = 0;

SAMDTimer ITimer1(TIMER_TCC);

void TimerHandler1()
{
  Serial.println("Internal Timer Triggered!!");
  newTime = millis();
  if (newTime - prevTime > waitTime) {
    detachInterrupt(digitalPinToInterrupt(wakePin));
    Serial.println("Normal Interrupt Detached!");
    LowPower.attachInterruptWakeup(wakePin, timerCalc, LOW);
    Serial.println("Low Power Interrupt attached!");
    Serial.println("Going to sleep!");
    LowPower.sleep();
  }
}

void timerCalc() {
  Serial.println("Woke up from sleep. Inside timerCalc().");
  prevTime = millis();
  attachInterrupt(digitalPinToInterrupt(wakePin), timerCalc2, LOW);
}

void timerCalc2() {
  Serial.println("Interrupt triggered. Inside timerCalc2().");
  prevTime = millis();
}


//----------------- for motion sensor  ---------------------------

#define Motion_triggering_treshold 10 // Change this value to increase or decrease the triggering treshold. Range (0-255)

#define SIGNAL_PATH_RESET   0x68
#define I2C_SLV0_ADDR       0x37
#define ACCEL_CONFIG        0x1C
#define MOT_THR             0x1F
#define MOT_DUR             0x20
#define MOT_DETECT_CTRL     0x69
#define INT_ENABLE          0x38
#define WHO_AM_I_MPU6050    0x75
#define INT_STATUS          0x3A
#define MPU6050_ADDRESS     0x68

int       flag = 0;
uint16_t  readdata;

void writeByte(uint8_t address, uint8_t subAddress, uint8_t data) {
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(subAddress);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t readByte(uint8_t address, uint8_t subAddress) {
  uint8_t data;
  Wire.beginTransmission(address);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(address, (uint8_t) 1);
  data = Wire.read();
  return data;
}


// -----------------   Setup  ---------------------------

void setup() {

  // ----------------- Setting Serial  ---------------------------
  Serial.begin(115200);
  delay(8000);
  Serial.println("Serial Initiated");

  // ----------------- Setting MPU6050  ---------------------------
  Serial.println("Setting MPU6050 to interrupt mode.");
  writeByte(MPU6050_ADDRESS, 0x6B, 0x00);
  writeByte(MPU6050_ADDRESS, SIGNAL_PATH_RESET, 0x07);
  writeByte(MPU6050_ADDRESS, I2C_SLV0_ADDR, 0x20);
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x01);
  writeByte(MPU6050_ADDRESS, MOT_THR, Motion_triggering_treshold);
  writeByte(MPU6050_ADDRESS, MOT_DUR, 40);
  writeByte(MPU6050_ADDRESS, MOT_DETECT_CTRL, 0x15);
  writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x40);
  writeByte(MPU6050_ADDRESS, 0x37, 160);
  Serial.println("Setting MPU6050 complete.\nSetting Internal Timer.");

  // ----------------- Setting Timer   ---------------------------
  Serial.print(F("\nStarting TimerInterruptTest on ")); Serial.println(BOARD_NAME);
  Serial.println(SAMD_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    preMillisTimer1 = millis();
    Serial.print(F("Starting ITimer1 OK, millis() = ")); Serial.println(preMillisTimer1);
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));


  // ----------------- Setting Interrupts  ---------------------------
  Serial.println("Attaching Low Power Interrupt.");
  pinMode(wakePin, INPUT_PULLUP);
  LowPower.attachInterruptWakeup(wakePin, timerCalc, LOW);
  Serial.println("Attaching Low Power Complete.\n");

  prevTime = millis();
  newTime = millis();

  Serial.println("Going to Sleep from Setup!");
  LowPower.sleep();
}


// ----------------- Loop  ---------------------------

void loop() {
  
}
