#include "sim800L.h"

sim800L gsm;

void setup()
{
  
  Serial.begin(9600);
  Serial3.begin(9600);
  //Serial3.println("AT"); //Handshaking with SIM900
  updateSerial();
  gsm.begin("01743473012");
  //gsm.sendMessage("Hello Bangladesh!!");
}

void loop()
{
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial3.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial3.available())
  {
    Serial.write(Serial3.read());//Forward what Software Serial received to Serial Port
  }
}

boolean ReadGSMRXDataResponse(const String &p_resp, const int &p_max_response_seconds) {
  //Many commands have a standard 5 sec max response time, others twenty or sixty seconds
  //Sending an SMS has a max 60 second response time (it allows for poor network connectivity/retries)

  //Prior to sending any AT command we cleared the Serial1 Rx buffer.
  //So we do not expect an extraneous response related to a previous long running command.

  //Iterate for the response delay until we have received the first character of the response
  unsigned long l_timer = millis();
  while (!CheckSecondsDelay(l_timer, p_max_response_seconds * 1000)) {
    if (Serial1.available()) {
      break;
    }
  }
  //Once any response character is received we assume we have the whole response

  //ReadGSMRxData includes a 250ms delay between characters to
  //allow for a response that is still coming.
  ReadGSMRXData();

  String l_resp = p_resp;
  l_resp.trim();
  if (G_GSMRXData == "") {
    if (G_SerialOn)
      Serial.println(F("No Data Received"));
    //
    CheckFreeSRAM();
    return false;
  }
  else if (l_resp == "") { //No particular response is expected
    CheckFreeSRAM();
    return true; //The response can be ignored
  }
  else if (G_GSMRXData.indexOf(l_resp) != -1) {
    //We may want to trim the expected response (OK) off G_GSMRXData
    CheckFreeSRAM();
    return true;
  }

  //We may have expected OK but got an ERROR.
  //Or we may have suffered a comms error and recd garbage.
  if (G_SerialOn) {
    Serial.print(F("Response Not Found - "));
    Serial.println(G_GSMRXData);
  }
  CheckFreeSRAM();
  return false;
} //ReadGSMRXDataResponse

//----------------------------------------------------------------------------------------------

void ReadGSMRXData() {
  //Available data is known to be in the RX buffer
  //Read it and return via G_GSMRXData
  //Application wide this is the only place we do Serial1.read()
  G_GSMRXData = "";
  unsigned long l_timer = millis();
  while (!CheckSecondsDelay(l_timer, 250)) {
    while (Serial1.available()) {
      char l_char; //Always read a char, then append char to G_GSMRXData String
      l_char = Serial1.read();
      G_GSMRXData += l_char;
      l_timer = millis(); //reset qtr sec timer
    }
  }
  G_GSMRXData.trim();
  //No need to force uppercase
  CheckFreeSRAM();
} //ReadGSMRXData

//----------------------------------------------------------------------------------------------
