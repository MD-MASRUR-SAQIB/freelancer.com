#include <Arduino.h>
#include <IRremote.hpp>

#define DECODE_DISTANCE
#define IR_RECEIVE_PIN  2
#define IR_SEND_PIN     3

uint32_t data = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Enabling IRin..."));
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop()
{
  if (IrReceiver.decode()) {
    data = IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.printIRResultShort(&Serial);
      Serial.print("Hex value: ");
      Serial.println(data, HEX);
      IrReceiver.resume();
  }
}
