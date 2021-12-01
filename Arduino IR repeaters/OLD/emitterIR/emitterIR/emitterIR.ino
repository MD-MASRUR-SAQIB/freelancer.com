/**  emitterIR.ino: A very small and simple to use Infrared Emitter demonstration
  *  Totally free to use and modify. Ewald  Kendziorra sr, 2020, www.kendziorra.nl 
  *  -----------------------------------------------------------------------------  
  *  values: OK/LIST button from my satellite receiver remote control
  *  decoded with my "receiverIR.ino"  (on my web site)
  *  read: DECODING INFRARED PROTOCOLS (on my web site)
  **/// NO NEED FOR A INFRARED LIBRARY ;=) //
     //====================================//
const byte irPin = 4,   // use a digital 'OUT' pin
           nbytes = 8;  // number of bytes
const uint16_t T1L = 520, T15L = 9000,                          // LOW times
               T1P = 550, T3P  = 1660, T4P = 2200, T8P = 4440;  // PULSED times
byte cmdBytes[nbytes] = {0};  // command bytes buffer

void sendCmd();  // public
void _sendIt(uint16_t t1, uint16_t t2);  // private

void setup() {
  Serial.begin(115200);
  while (!Serial); delay(1000);
  Serial.println("\n...\n");
  pinMode(irPin, OUTPUT);
  digitalWrite(irPin, LOW);
  
  /** only on this place as a test and demo example
    * change BYTE(s) in a function to send other commands
    **/
  // 0x80 0xBF 0x73 0x8C values: OK/LIST button from my satellite receiver remote control
  // 0x80 0xBF 0x73 0x8C: binair
  cmdBytes[0] = 0x8;  // 1000
  cmdBytes[1] = 0x0;  // 0000
  cmdBytes[2] = 0xB;  // 1011
  cmdBytes[3] = 0xF;  // 1111
  cmdBytes[4] = 0x7;  // 0111
  cmdBytes[5] = 0x3;  // 0011
  cmdBytes[6] = 0x8;  // 1000
  cmdBytes[7] = 0xC;  // 1100
}

void loop() {
  /** only on this place as a test and demo example
    * use receiverIR.ino to test it for: 80 BF 73 8C
    **/
  Serial.println("send...");
  sendCmd();  // my satellite receiver EPG-list switch ON and OFF ;=)
  delay(20000);
}

void sendCmd() {
  _sendIt(T15L, T8P);  // START stream 
  for (byte i = 0; i < nbytes; i++) {
    for (byte j = 0; j < 4; j++) _sendIt(T1L, ((cmdBytes[i] << j) & 0x8  ? T3P : T1P)); 
  }
  _sendIt(T15L, T4P);  // END stream
  _sendIt(T15L, T4P);  //   2x  
}

void _sendIt(uint16_t t1, uint16_t t2) {  // 38KHz PULSED by OPEN-SMART Infrared Emitter
  digitalWrite(irPin, HIGH);
  delayMicroseconds(t1);
  digitalWrite(irPin, LOW);
  delayMicroseconds(t2);
}
