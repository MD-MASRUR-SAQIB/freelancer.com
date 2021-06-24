#include <SoftwareSerial.h>// import the serial library
 
SoftwareSerial BT(10, 11); // RX, TX
int ledpin=13; // led on D13 will show blink on / off
int BluetoothData; // the data given from Computer
 
void setup() 
{
  // put your setup code here, to run once:
  BT.begin(9600);
  BT.println("Bluetooth On please press 1 or 0 blink LED ..");
  pinMode(ledpin,OUTPUT);
}
 
void loop() 
{
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    BT.print(Serial.read());
  }
//delay(5000);// prepare for next data ...
}
