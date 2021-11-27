#include <NTPtimeESP.h>
#include <ESP8266WiFi.h>

#define DEBUG_ON

#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   

NTPtime NTPch("ch.pool.ntp.org");   // Choose server pool as required


#define PIN_1 D4
#define NUMpixels_1 14
Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(NUMpixels_1, PIN_1, NEO_GRB + NEO_KHZ800);
#define PIN_2 D8
#define NUMpixels_2 14
Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(NUMpixels_2, PIN_2, NEO_GRB + NEO_KHZ800);
#define PIN_3 D3
#define NUMpixels_3 14
Adafruit_NeoPixel pixels_3 = Adafruit_NeoPixel(NUMpixels_3, PIN_3, NEO_GRB + NEO_KHZ800);

int myARay[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
//***************************** wifi and ntp parametrer***********************

int Black = pixels_2.Color(0, 0, 0);
int white = pixels_2.Color(255, 255, 255);











int Tens, Units;
int hour_;

//***************************** wifi and ntp parametrer***********************

//-******************************************my para meter***************************

bool button1 = false;
bool button2 = false;
bool button3 = false;

int ldr_value = 0;

#define btn_1 D5
#define btn_2 D6
#define btn_3 D7
//                 L1  L2 L3
int R_color[3] = {0, 0, 0};
int G_color[3] = {0, 0, 0};
int B_clor[3] = {0, 0, 0};

int Light_1_Mode = 0;
int Light_2_Mode = 0;
int Light_3_Mode = 0;

int pressed_1 = 0;
int pressed_2 = 0;
int pressed_3 = 0;

//-******************************************my para meter***************************

//*****************************************my function*****************************************

void set_Light_1_mode(int x)
{

  switch (x)
  {
  case 0: // red
    R_color[0] = 255;
    G_color[0] = 0;
    B_clor[0] = 0;

    break;
  case 1: // green
    R_color[0] = 0;
    G_color[0] = 255;
    B_clor[0] = 0;
    break;
  case 2: // blue
    R_color[0] = 0;
    G_color[0] = 0;
    B_clor[0] = 255;
    break;
  case 3: //violet
    R_color[0] = 127;
    G_color[0] = 0;
    B_clor[0] = 255;
    break;
  case 4: // yellow
    R_color[0] = 255;
    G_color[0] = 255;
    B_clor[0] = 0;
    break;
  case 5: // white
    R_color[0] = 255;
    G_color[0] = 255;
    B_clor[0] = 255;
    break;
  case 6: // chayan
    R_color[0] = 0;
    G_color[0] = 255;
    B_clor[0] = 255;
    break;
  }
}

void set_Light_2_mode(int x)
{

  switch (x)
  {
  case 0: // red
    R_color[1] = 255;
    G_color[1] = 0;
    B_clor[1] = 0;

    break;
  case 1: // green
    R_color[1] = 0;
    G_color[1] = 255;
    B_clor[1] = 0;
    break;
  case 2: // blue
    R_color[1] = 0;
    G_color[1] = 0;
    B_clor[1] = 255;
    break;
  case 3: //violet
    R_color[1] = 127;
    G_color[1] = 0;
    B_clor[1] = 255;
    break;
  case 4: // yellow
    R_color[1] = 255;
    G_color[1] = 255;
    B_clor[1] = 0;
    break;
  case 5: // white
    R_color[1] = 255;
    G_color[1] = 255;
    B_clor[1] = 255;
    break;
  case 6: // chayan
    R_color[1] = 0;
    G_color[1] = 255;
    B_clor[1] = 255;
    break;
  }
}

void set_Light_3_mode(int x)
{

  switch (x)
  {
  case 0: // red
    R_color[2] = 255;
    G_color[2] = 0;
    B_clor[2] = 0;

    break;
  case 1: // green
    R_color[2] = 0;
    G_color[2] = 255;
    B_clor[2] = 0;
    break;
  case 2: // blue
    R_color[2] = 0;
    G_color[2] = 0;
    B_clor[2] = 255;
    break;
  case 3: //violet
    R_color[2] = 127;
    G_color[2] = 0;
    B_clor[2] = 255;
    break;
  case 4: // yellow
    R_color[2] = 255;
    G_color[2] = 255;
    B_clor[2] = 0;
    break;
  case 5: // white
    R_color[2] = 255;
    G_color[2] = 255;
    B_clor[2] = 255;
    break;
  case 6: // chayan
    R_color[2] = 0;
    G_color[2] = 255;
    B_clor[2] = 255;
    break;
  }
}

strDateTime dateTime;
//*****************************************my function*****************************************

void setup()
{
  flash();
blank();
  //********************** wifi  setting up  and  ntp time client  ************************
      // put your setup code here, to run once:
    Serial.begin(115200);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
//    reset saved settings
   // wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("Domeeno Clock");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    

  

  //********************** wifi  setting up ntp time client ************************

  //********************* setting up neo pixel  and other things****************************************

  Serial.begin(9600);
  pixels_1.clear();
  pixels_2.clear();
  pixels_3.clear();
  pixels_1.begin(); //Hours
  pixels_2.begin(); // Tens Minutes
  pixels_3.begin(); // Units Minutes

  pinMode(btn_1, INPUT_PULLUP);
  pinMode(btn_2, INPUT_PULLUP);
  pinMode(btn_3, INPUT_PULLUP);


  for (int i = 0; i < 3; i++)
  {
    int s = EEPROM.read(i);
    if (s < 0 || s > 6)
    {
      EEPROM.write(i, 0);
    }
  }
  

  Light_1_Mode = EEPROM.read(0);
  Light_2_Mode = EEPROM.read(1);
  Light_3_Mode = EEPROM.read(2);

 
  //********************* setting up neo pixel  and other things ****************************************
}

void loop()
{

  dateTime = NTPch.getNTPtime(1.0, 1);
   if(dateTime.valid){
    NTPch.printDateTime(dateTime);

     byte actualHour = dateTime.hour;
    byte actualMinute = dateTime.minute;
    byte actualsecond = dateTime.second;
    int actualyear = dateTime.year;
    byte actualMonth = dateTime.month;
    byte actualday =dateTime.day;
    byte actualdayofWeek = dateTime.dayofWeek;
    
    
  int h_o_u_r = dateTime.hour;

  Tens = (dateTime.minute / 10);
  Units = (dateTime.minute - (Tens * 10));

  if (h_o_u_r == 13 || h_o_u_r == 1)
  {
    hour_ = 1;
  }
  if (h_o_u_r == 14 || h_o_u_r == 2)
  {
    hour_ = 2;
  }
  if (h_o_u_r == 15 || h_o_u_r == 3)
  {
    hour_ = 3;
  }
  if (h_o_u_r == 16 || h_o_u_r == 4)
  {
    hour_ = 4;
  }
  if (h_o_u_r == 17 || h_o_u_r == 5)
  {
    hour_ = 5;
  }
  if (h_o_u_r == 18 || h_o_u_r == 6)
  {
    hour_ = 6;
  }
  if (h_o_u_r == 19 || h_o_u_r == 7)
  {
    hour_ = 7;
  }
  if (h_o_u_r == 20 || h_o_u_r == 8)
  {
    hour_ = 8;
  }
  if (h_o_u_r == 21 || h_o_u_r == 9)
  {
    hour_ = 9;
  }
  if (h_o_u_r == 22 || h_o_u_r == 10)
  {
    hour_ = 10;
  }
  if (h_o_u_r == 23 || h_o_u_r == 11)
  {
    hour_ = 11;
  }
  if (h_o_u_r == 0 || h_o_u_r == 12)
  {
    hour_ = 12;
  }

  //************************ for controlling neopixel pannel ****************************************

  int x = analogRead(A0);

  ldr_value = map(x, 1024, 0, 20, 255);   // if you don't want control intensity just write   ldr_value =255;

  while (digitalRead(btn_1) != HIGH)
  {

    if (pressed_1 == 0)
    {
      pressed_1 = 1; // inidcating button is pressed

      if (Light_1_Mode == 0)
      {
        Light_1_Mode = 1;
        EEPROM.write(0, Light_1_Mode);
      }
      else if (Light_1_Mode == 1)
      {
        Light_1_Mode = 2;
        EEPROM.write(0, Light_1_Mode);
      }
      else if (Light_1_Mode == 2)
      {
        Light_1_Mode = 3;
        EEPROM.write(0, Light_1_Mode);
      }
      else if (Light_1_Mode == 3)
      {
        Light_1_Mode = 4;
        EEPROM.write(0, Light_1_Mode);
      }

      else if (Light_1_Mode == 4)
      {
        Light_1_Mode = 5;
        EEPROM.write(0, Light_1_Mode);
      }

      else if (Light_1_Mode == 5)
      {
        Light_1_Mode = 6;
        EEPROM.write(0, Light_1_Mode);
      }
      else if (Light_1_Mode == 6)
      {
        Light_1_Mode = 0;
        EEPROM.write(0, Light_1_Mode);
      }
    }
  }
  pressed_1 = 0;

  while (digitalRead(btn_2) != HIGH)
  {

    if (pressed_2 == 0)
    {
      pressed_2 = 1; // inidcating button is pressed

      if (Light_2_Mode == 0)
      {
        Light_2_Mode = 1;
        EEPROM.write(1, Light_2_Mode);
      }
      else if (Light_2_Mode == 1)
      {
        Light_2_Mode = 2;
        EEPROM.write(1, Light_2_Mode);
      }
      else if (Light_2_Mode == 2)
      {
        Light_2_Mode = 3;
        EEPROM.write(1, Light_2_Mode);
      }
      else if (Light_2_Mode == 3)
      {
        Light_2_Mode = 4;
        EEPROM.write(1, Light_2_Mode);
      }
      else if (Light_2_Mode == 4)
      {
        Light_2_Mode = 5;
        EEPROM.write(1, Light_2_Mode);
      }

      else if (Light_2_Mode == 5)
      {
        Light_2_Mode = 6;
        EEPROM.write(1, Light_2_Mode);
      }
      else if (Light_2_Mode == 6)
      {
        Light_2_Mode = 0;
        EEPROM.write(1, Light_2_Mode);
      }
    }
  }
  pressed_2 = 0;

  while (digitalRead(btn_3) != HIGH)
  {

    if (pressed_3 == 0)
    {
      pressed_3 = 1; // inidcating button is pressed

      if (Light_3_Mode == 0)
      {
        Light_3_Mode = 1;
        EEPROM.write(2, Light_3_Mode);
      }
      else if (Light_3_Mode == 1)
      {
        Light_3_Mode = 2;
        EEPROM.write(2, Light_3_Mode);
      }
      else if (Light_3_Mode == 2)
      {
        Light_3_Mode = 3;
        EEPROM.write(2, Light_3_Mode);
      }
      else if (Light_3_Mode == 3)
      {
        Light_3_Mode = 4;
        EEPROM.write(2, Light_3_Mode);
      }
      else if (Light_3_Mode == 4)
      {
        Light_3_Mode = 5;
        EEPROM.write(2, Light_3_Mode);
      }

      else if (Light_3_Mode == 5)
      {
        Light_3_Mode = 6;
        EEPROM.write(2, Light_3_Mode);
      }

      else if (Light_3_Mode == 6)
      {
        Light_3_Mode = 0;
        EEPROM.write(2, Light_3_Mode);
      }
    }
  }
  pressed_3 = 0;

  set_Light_1_mode(Light_1_Mode);
  set_Light_2_mode(Light_2_Mode);
  set_Light_3_mode(Light_3_Mode);

  //************************ for controlling neopixel pannel ****************************************

  //**********************************************simple modified **************************************

  switch (hour_)
  {
  case 1:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(10, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 2:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(10, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(3, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 3:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(10, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 4:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(0, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(6, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 5:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(0, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(6, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(10, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 6:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(0, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(3, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(6, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(10, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 7:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(0, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(1, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(5, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(6, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(10, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 8:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(0, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(1, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(5, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(6, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(8, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(12, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 9:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(0, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(1, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(3, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(5, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(6, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(8, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(12, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 10:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(0, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(1, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(3, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(5, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(6, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(8, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(10, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(12, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 11:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(0, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(1, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(3, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(5, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(6, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(8, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(9, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(11, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(12, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  case 12:
    for (byte i = 0; i < 14; i = i + 1)
    {
      pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
      delay(5);
    }
    pixels_1.setPixelColor(0, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(1, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(2, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(4, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(5, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(6, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(7, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(8, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(9, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(11, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(12, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setPixelColor(13, pixels_1.Color(R_color[0], G_color[0], B_clor[0]));
    pixels_1.setBrightness(ldr_value);
    pixels_1.show();
    break;
  }
  ///////////////////////////////////////////////////////////////////////TENS UNITS
  switch (Units)
  {
  case 0:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));

      delay(5);
    }
    pixels_3.show();
    break;
  case 1:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
      delay(5);
    }
    pixels_3.setPixelColor(10, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setBrightness(ldr_value);
    pixels_3.show();
    break;
  case 2:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
      delay(5);
    }
    pixels_3.setPixelColor(3, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(10, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setBrightness(ldr_value);
    pixels_3.show();
    break;
  case 3:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
      delay(5);
    }
    pixels_3.setPixelColor(3, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(7, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(13, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setBrightness(ldr_value);
    pixels_3.show();
    break;
  case 4:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
      delay(5);
    }
    pixels_3.setPixelColor(0, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(6, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(7, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(13, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setBrightness(ldr_value);
    pixels_3.show();
    break;
  case 5:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
      delay(5);
    }
    pixels_3.setPixelColor(0, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(1, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(3, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(5, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(6, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setBrightness(ldr_value);
    pixels_3.show();
    break;
  case 6:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
      delay(5);
    }
    pixels_3.setPixelColor(7, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(8, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(9, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(11, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(12, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(13, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setBrightness(ldr_value);
    pixels_3.show();
    break;
  case 7:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
      delay(5);
    }
    pixels_3.setPixelColor(0, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(1, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(3, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(5, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(6, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(7, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(13, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setBrightness(ldr_value);
    pixels_3.show();
    break;
  case 8:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
      delay(5);
    }
    pixels_3.setPixelColor(0, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(1, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(5, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(6, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(7, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(8, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(12, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(13, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setBrightness(ldr_value);
    pixels_3.show();
    break;
  case 9:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
      delay(5);
    }
    pixels_3.setPixelColor(0, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(1, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(5, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(6, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(7, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(8, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(10, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(12, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setPixelColor(13, pixels_3.Color(R_color[2], G_color[2], B_clor[2]));
    pixels_3.setBrightness(ldr_value);
    pixels_3.show();
    break;
  }
  //////////////////////////////////////////////////////////////////////////////TENS MINUTES
  switch (Tens)
  {
  case 0:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
      delay(5);
    }
    pixels_2.show();
    break;
  case 1:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
      delay(5);
    }
    pixels_2.setPixelColor(10, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setBrightness(ldr_value);
    pixels_2.show();
    break;
  case 2:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
      delay(5);
    }
    pixels_2.setPixelColor(3, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(10, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setBrightness(ldr_value);
    pixels_2.show();
    break;
  case 3:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
      delay(5);
    }
    pixels_2.setPixelColor(7, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(10, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(13, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setBrightness(ldr_value);
    pixels_2.show();
    break;
  case 4:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
      delay(5);
    }
    pixels_2.setPixelColor(0, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(6, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(7, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(13, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setBrightness(ldr_value);
    pixels_2.show();
    break;
  case 5:
    for (byte j = 0; j < 14; j = j + 1)
    {
      pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
      delay(5);
    }
    pixels_2.setPixelColor(7, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(8, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(10, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(12, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setPixelColor(13, pixels_2.Color(R_color[1], G_color[1], B_clor[1]));
    pixels_2.setBrightness(ldr_value);
    pixels_2.show();
    break;
  }
  //**********************************************simple modified **************************************
}
}

void blank() {
  //clear the decks
  for (int x = 0; x < NUMpixels_2; ++x) {
    pixels_2.setPixelColor(x, Black);
  }
 }

 void flash() {

  blank();
  for (int y = 0; y < 10; ++y) {
    for (int x = 0; x < NUMpixels_1; x = x + 2) {
      pixels_1.setPixelColor(x, white);
    }
    
    pixels_1.begin();
    delay(50);
    blank();
    delay(50);

    for (int x = 1; x < NUMpixels_1; x = x + 2) {
      pixels_1.setPixelColor(x, white);
    }
  
    pixels_1.begin();
    delay(50);
    blank();
    delay(50);
  }

 
    
  }
