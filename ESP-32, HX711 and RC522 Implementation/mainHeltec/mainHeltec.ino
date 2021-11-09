#include <SPI.h>
#include <WiFi.h>
#include "heltec.h"
#include "HX711.h"
#include <MFRC522.h>
#include <HTTPClient.h>

// RF522: Initialization of classes and Defining Macros
#define SS_PIN        21
#define RST_PIN       22    // Reset pin for RFID
#define CardDataSize  64  // A card contains 1024 Bytes of Data. You can check them all if needed.

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte cardData[CardDataSize], oldCardData[CardDataSize];
uint32_t cardDataInInt = 0;



// WiFi-HTTP: Initialization of classes and Defining Macros
const String HARDWARE_Identifier = "Test";
String serverName = "https://dev-us.com/functionarchitecture/BOTTLE/";
unsigned long lastTime = 0, timerDelay = 120000;
int runcount = 0;
String EXTERNAL_APIkey = "";

const char* ssid = "";
const char* password = "";

#define OLED_UPDATE_INTERVAL 500
int oledposition = 0;



// HX711: Initialization of classes and Defining Macros
uint8_t   scaleDataPin    = 12;
uint8_t   scaleClockPin   = 13;
long      scaleOffset     = 0; //************Input Scale Calibrated Data here!!
float     scaleData       = 1.0; //************Input Scale Calibrated Data here!!

HX711     scale;

float     weight          = -1, prevWeight = -1;
float     comparingWeight = 5; //grams. In first condition the weight will be compared with this.
float     trigWeightDiff  = 1; //percent. Trigger if the weight difference is greater than this value.

void setup()
{
  // Setup: Display and WiFi

  Heltec.begin(true, false, true);
  pinMode(25, OUTPUT);    //Your Code
  digitalWrite(25, HIGH); //Your Code


  Serial.begin(9600);

  Serial.println("Initializing...");
  setupWIFI();

  Heltec.display->clear();
  Heltec.display->drawString(0, oledposition, "Roll it out...");
  oledposition = oledposition + 10;


  //Setup: RFID Init

  SPI.begin();
  mfrc522.PCD_Init();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }


  //Setup: Scale Calibration
  Serial.println("Initializing Scale!");
  scale.begin(scaleDataPin, scaleClockPin);
  if (scale.is_ready()) {
    Serial.println("Calibrating Scale!");
    scale.set_scale(scaleData);
    scale.tare();
    scaleOffset = scale.get_offset();
    scale.set_offset(scaleOffset);
    Serial.println("Scale Calibrated!");
  }
  else {
    Serial.println("Scale connection error!!");
  }

  //For First Time Data storing. The bellow code is for filling up the data first time. 
  /* <-- Uncomment if needed
  delay(5000);
  Serial.println("Place an RFID Tag on the reader.");
  delay(5000);
  if (scale.is_ready()) {
    weight = scale.get_units();
    prevWeight = weight;
    if (mfrc522.PICC_IsNewCardPresent()) {
      if (mfrc522.PICC_ReadCardSerial()) {
        readCard();
        if (!checkIfSame()) {
          byteToInt();
        }
      }
      else {
        Serial.println("Scale Error!!");
      }
    }
  }
  */ //<-- Uncomment if needed
}

void loop()
{
  if (scale.is_ready()) {
    weight = scale.get_units();
  }
  else {
    Serial.println("Scale Error!!");
  }

  if (weight > comparingWeight) { // Checking if the weight is greater than 5 grams.
    Serial.println("Weight is greater than 5 grams!");
    if (mfrc522.PICC_IsNewCardPresent()) { //Checking if a new card is present.
      Serial.println("A new card is present!");
      if (mfrc522.PICC_ReadCardSerial()) {
        Serial.println("Reading New Card!");
        readCard(); // Reading the Card
        if (checkIfSame()) { // Checking if the Card is same as previous. Returns True when same.
          Serial.println("NO NEW NFC!!");
          if (checkWeightDiff()) { // Checking Weight Difference. Returns True when Difference is greater than triggering val
            httpReq(weight, cardDataInInt);
            prevWeight = weight; // Changing the Previous weight to new weight.
          }
        }
        else {
          Serial.println("Previous NFC found!");
          byteToInt();
          httpReq(weight, cardDataInInt);
        }
      }
      else {
        Serial.println("Card Reading Failed.");
      }
    }
    else {
      Serial.println("NO NFC TAG DETECTED");
    }
  }
  delay(2000);
}

void httpReq( float weightt, uint32_t bottle_id) {
  if (((millis() - lastTime) > timerDelay) || runcount == 0) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String serverPath = serverName + "?segment=fromESP32&CurrentRun=" + runcount;
      http.begin(serverPath.c_str());
      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST((String)"{\"api\":\""          + EXTERNAL_APIkey
                                       + "\",\"hardware\":\""  + HARDWARE_Identifier
                                       + "\",\"v_weight\":\""  + weightt
                                       + "\",\"v_id\":\""      + bottle_id
                                       + "\"}");
      Serial.println("HTTP FIRING...");
      Heltec.display->drawString(0, oledposition, ((String)"HTTP REST | Runcount:" + runcount + "   Time:" + lastTime));
      oledposition = oledposition + 10;
      Heltec.display->display();

      Serial.println((String)"Current Runcount:" + runcount + " Var 2:" + lastTime + " Var 3:" + serverPath);

      if (httpResponseCode > 0) {
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

bool checkIfSame() {
  bool res = true;
  for (int i = 0 ; i < CardDataSize ; i++) {
    if (cardData[i] != oldCardData[i]) {
      res = false; // Found difference in Card Data
      oldCardData[i] = cardData[i]; // Changing the old card data with new data
    }
  }
  return res;
}

void byteToInt() {
  for (int j = 0 ; j < 4 ; j++) {
    cardDataInInt = cardDataInInt << 8;
    cardDataInInt |= cardData[j];
  }
}

void readCard()
{
  byte buffersize = 20;
  byte buffer[buffersize];
  int nextBlock, buff = 0;
  int blockNumber;
  int trailerBlock = 3;
  for (trailerBlock = 3, nextBlock = 0 ; trailerBlock < (CardDataSize / 16); trailerBlock += 4) {
    byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.println("PCD_Authenticate() failed to read");
      Serial.println(trailerBlock);
    }
    else {
      for (blockNumber = trailerBlock - 3; blockNumber <= trailerBlock ; blockNumber++) {
        status = mfrc522.MIFARE_Read(blockNumber, buffer, &buffersize);
        if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed ");
        }
        else {
          Serial.print(blockNumber);
          Serial.print(" :\n");
          for ( buff = 0 ; buff < 16 ; buff++, nextBlock++) {
            cardData[nextBlock] = buffer[buff];
            Serial.print(cardData[nextBlock], HEX);
            Serial.print(" ");
          }
        }
        Serial.println("");
      }
    }
  }
  Serial.println("\nPrint finished");
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

bool checkWeightDiff() {
  float weightDiff = abs(weight - prevWeight) * 100.0 / prevWeight;
  if (weightDiff >= trigWeightDiff) {
    return true;
  }
  return false;
}

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
  while (WiFi.status() != WL_CONNECTED && count < 10) {
    count ++;
    delay(500);
    Serial.print(".");
  }
  Heltec.display->clear();
  if (WiFi.status() == WL_CONNECTED) {
    Heltec.display->drawString(0, 0, "Connected.  Hi John.");
    Serial.println("Connected to WiFi");
  }
  else {
    Heltec.display->drawString(0, 0, "Connect False");
    Serial.println("WiFi Connection Error");
  }
  Heltec.display->display();
}
