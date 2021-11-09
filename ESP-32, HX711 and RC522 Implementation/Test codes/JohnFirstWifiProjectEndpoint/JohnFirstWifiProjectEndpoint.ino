
#include <WiFi.h>
#include <HTTPClient.h>
#include "heltec.h"


const String HARDWARE_Identifier = "Provision-HTITWB32-001";


String serverName = "https://dev-us.com/functionarchitecture/BOTTLE/";
unsigned long lastTime = 0;
unsigned long timerDelay = 120000;
int runcount = 0;
int oledposition = 0;
String EXTERNAL_APIkey = "";


const char* ssid = ""; 
const char* password = ""; 

#define OLED_UPDATE_INTERVAL 500      

void setupWIFI()
{
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Connecting...");
  Heltec.display->drawString(0, 10, String(ssid));
  Heltec.display->display();

  WiFi.disconnect(true);
  delay(1000);

  WiFi.mode(WIFI_STA);
  //WiFi.onEvent(WiFiEvent);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);   
  //WiFi.setHostname(HOSTNAME);
  WiFi.begin(ssid, password);



  byte count = 0;
  while(WiFi.status() != WL_CONNECTED && count < 10)
  {
    count ++;
    delay(500);
    Serial.print(".");
  }

  Heltec.display->clear();
  if(WiFi.status() == WL_CONNECTED)
    Heltec.display->drawString(0, 0, "Connected.  Hi John.");
  else
    Heltec.display->drawString(0, 0, "Connect False");
  Heltec.display->display();
}






void setup()
{
  Heltec.begin(true, false, true);
  pinMode(25, OUTPUT);
  digitalWrite(25,HIGH);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Initialize...booting John...");
  setupWIFI();


        Heltec.display->clear();
        Heltec.display->drawString(0, oledposition, "Roll it out...");
        oledposition = oledposition + 10;
}


void loop()
{

//Send an HTTP POST request every X delay
  if (((millis() - lastTime) > timerDelay) || runcount == 0) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "?segment=fromESP32&CurrentRun=" + runcount;
      
      http.begin(serverPath.c_str());
      http.addHeader("Content-Type", "application/json");

      // variable to be replaced with weight inbound from scale
      int randomweight = rand() % 1000 + 1;   

      // variable to be replaced with the variable scanned from RFID/NFC tag
      String bottle_id = "xid-2019x";  
      
      
      int httpResponseCode = http.POST((String)"{\"api\":\"" + EXTERNAL_APIkey + "\",\"hardware\":\"" + HARDWARE_Identifier + "\",\"v_weight\":\"" + randomweight + "\",\"v_id\":\"" + bottle_id + "\"}");
      Serial.println("HTTP FIRING...");


        Heltec.display->drawString(0, oledposition, ((String)"HTTP REST | Runcount:" + runcount + "   Time:" + lastTime));
        oledposition = oledposition + 10;
        Heltec.display->display();

      //test
      Serial.println((String)"Current Runcount:" + runcount + " Var 2:" + lastTime + " Var 3:" + serverPath);
      
      if (httpResponseCode>0) {
        Serial.print("HTTP REST CALL: Success.  Unique Identifier: ");
        Serial.println(runcount);      
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.println("HTTP REST CALL: Exception. Unique Identifier: " + runcount + ' Response Code: ');
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
    runcount = runcount + 1;
  }
  
}
