#include <Wire.h>
#include <EEPROM.h>
#include <DS3231.h>
#include "PinChangeInterrupt.h"
#include <Adafruit_NeoPixel.h>

#define PIN_1 8 //change the pin to D8
#define NUMpixels_1 14
Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(NUMpixels_1, PIN_1, NEO_GRB + NEO_KHZ800);

#define PIN_2 9 //change the pin to D9
#define NUMpixels_2 14
Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(NUMpixels_2, PIN_2, NEO_GRB + NEO_KHZ800);

#define PIN_3 10 //change the pin to D10
#define NUMpixels_3 14
Adafruit_NeoPixel pixels_3 = Adafruit_NeoPixel(NUMpixels_3, PIN_3, NEO_GRB + NEO_KHZ800);

int myARay[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

int Black = pixels_2.Color(0, 0, 0);
int white = pixels_2.Color(255, 255, 255);

int Tens, Units;
int hour_;

uint16_t rain_j1 = 0, rain_j2 = 0, rain_j3 = 0;
uint16_t rain_jn1 = 0, rain_jn2 = 0, rain_jn3 = 0;

bool button1 = false;
bool button2 = false;
bool button3 = false;

int ldr_value = 0;

#define btn_1 2 //change the pin to D2
#define btn_2 3 //change the pin to D3
#define btn_3 4 //change the pin to D4

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

int rainSpeed = 10;

//-******************************************my para meter***************************

//*****************************************my function*****************************************

uint32_t set_Light_1_mode(int x) {
  if (x == 0) { // red
    R_color[0] = 255;
    G_color[0] = 0;
    B_clor[0] = 0;
    return pixels_1.Color(R_color[0], G_color[0], B_clor[0]);
  }
  else if (x == 1) { // green
    R_color[0] = 0;
    G_color[0] = 255;
    B_clor[0] = 0;
    return pixels_1.Color(R_color[0], G_color[0], B_clor[0]);
  }
  else if (x == 2) { // blue
    R_color[0] = 0;
    G_color[0] = 0;
    B_clor[0] = 255;
    return pixels_1.Color(R_color[0], G_color[0], B_clor[0]);
  }
  else if (x == 3) { //violet
    R_color[0] = 127;
    G_color[0] = 0;
    B_clor[0] = 255;
    return pixels_1.Color(R_color[0], G_color[0], B_clor[0]);
  }
  else if (x == 4) { // yellow
    R_color[0] = 255;
    G_color[0] = 255;
    B_clor[0] = 0;
    return pixels_1.Color(R_color[0], G_color[0], B_clor[0]);
  }
  else if (x == 5) { // white
    R_color[0] = 255;
    G_color[0] = 255;
    B_clor[0] = 255;
    return pixels_1.Color(R_color[0], G_color[0], B_clor[0]);
  }
  else if (x == 6) { // chayan
    R_color[0] = 0;
    G_color[0] = 255;
    B_clor[0] = 255;
    return pixels_1.Color(R_color[0], G_color[0], B_clor[0]);
  }
  else if (x == 7) { // rainbow
    //rain_j1 += 2;
    rain_j1 = rain_jn1 & 255;
    if (rain_j1 > 256) {
      rain_j1 = 0;
    }
    if (rain_j1 < 85) {
      return pixels_1.Color(rain_j1 * 3, 255 - rain_j1 * 3, 0);
    }
    else if (rain_j1 < 170) {
      rain_j1 -= 85;
      return pixels_1.Color(255 - rain_j1 * 3, 0, rain_j1 * 3);
    }
    else {
      rain_j1 -= 170;
      return pixels_1.Color(0, rain_j1 * 3, 255 - rain_j1 * 3);
    }
  }
}


uint32_t set_Light_2_mode(int x) {
  if (x == 0) { // red
    R_color[1] = 255;
    G_color[1] = 0;
    B_clor[1] = 0;
    return pixels_2.Color(R_color[1], G_color[1], B_clor[1]);
  }
  else if (x == 1) { // green
    R_color[1] = 0;
    G_color[1] = 255;
    B_clor[1] = 0;
    return pixels_2.Color(R_color[1], G_color[1], B_clor[1]);
  }
  else if (x == 2) { // blue
    R_color[1] = 0;
    G_color[1] = 0;
    B_clor[1] = 255;
    return pixels_2.Color(R_color[1], G_color[1], B_clor[1]);
  }
  else if (x == 3) { //violet
    R_color[1] = 127;
    G_color[1] = 0;
    B_clor[1] = 255;
    return pixels_2.Color(R_color[1], G_color[1], B_clor[1]);
  }
  else if (x == 4) { // yellow
    R_color[1] = 255;
    G_color[1] = 255;
    B_clor[1] = 0;
    return pixels_2.Color(R_color[1], G_color[1], B_clor[1]);
  }
  else if (x == 5) { // white
    R_color[1] = 255;
    G_color[1] = 255;
    B_clor[1] = 255;
    return pixels_2.Color(R_color[1], G_color[1], B_clor[1]);
  }
  else if (x == 6) { // chayan
    R_color[1] = 0;
    G_color[1] = 255;
    B_clor[1] = 255;
    return pixels_2.Color(R_color[1], G_color[1], B_clor[1]);
  }
  else if (x == 7) { // rainbow
    //rain_j2 += 2;
    rain_j2 = rain_jn2 & 255;
    if (rain_j2 > 256) {
      rain_j2 = 0;
    }
    if (rain_j2 < 85) {
      return pixels_2.Color(rain_j2 * 3, 255 - rain_j2 * 3, 0);
    }
    else if (rain_j2 < 170) {
      rain_j2 -= 85;
      return pixels_2.Color(255 - rain_j2 * 3, 0, rain_j2 * 3);
    }
    else {
      rain_j2 -= 170;
      return pixels_2.Color(0, rain_j2 * 3, 255 - rain_j2 * 3);
    }
  }
}


uint32_t set_Light_3_mode(int x) {
  if (x == 0) { //red
    R_color[2] = 255;
    G_color[2] = 0;
    B_clor[2] = 0;
    return pixels_3.Color(R_color[2], G_color[2], B_clor[2]);
  }
  else if (x == 1) { // green
    R_color[2] = 0;
    G_color[2] = 255;
    B_clor[2] = 0;
    return pixels_3.Color(R_color[2], G_color[2], B_clor[2]);
  }
  else if (x == 2) { // blue
    R_color[2] = 0;
    G_color[2] = 0;
    B_clor[2] = 255;
    return pixels_3.Color(R_color[2], G_color[2], B_clor[2]);
  }
  else if (x == 3) { //violet
    R_color[2] = 127;
    G_color[2] = 0;
    B_clor[2] = 255;
    return pixels_3.Color(R_color[2], G_color[2], B_clor[2]);
  }
  else if (x == 4) { // yellow
    R_color[2] = 255;
    G_color[2] = 255;
    B_clor[2] = 0;
    return pixels_3.Color(R_color[2], G_color[2], B_clor[2]);
  }
  else if (x == 5) { // white
    R_color[2] = 255;
    G_color[2] = 255;
    B_clor[2] = 255;
    return pixels_3.Color(R_color[2], G_color[2], B_clor[2]);
  }
  else if (x == 6) { // chayan
    R_color[2] = 0;
    G_color[2] = 255;
    B_clor[2] = 255;
    return pixels_3.Color(R_color[2], G_color[2], B_clor[2]);
  }
  else if (x == 7) { // rainbow
    //rain_j3 += 2;
    rain_j3 = rain_jn3 & 255;
    if (rain_j3 > 256) {
      rain_j3 = 0;
    }
    if (rain_j3 < 85) {
      return pixels_3.Color(rain_j3 * 3, 255 - rain_j3 * 3, 0);
    }
    else if (rain_j3 < 170) {
      rain_j3 -= 85;
      return pixels_3.Color(255 - rain_j3 * 3, 0, rain_j3 * 3);
    }
    else {
      rain_j3 -= 170;
      return pixels_3.Color(0, rain_j3 * 3, 255 - rain_j3 * 3);
    }
  }
}


//*****************************************my function*****************************************

RTClib myRTC;

void setup() {
  flash();
  blank();
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();

  pixels_1.clear();
  pixels_2.clear();
  pixels_3.clear();
  pixels_1.begin(); //Hours
  pixels_2.begin(); // Tens Minutes
  pixels_3.begin(); // Units Minutes

  pinMode(btn_1, INPUT_PULLUP);
  pinMode(btn_2, INPUT_PULLUP);
  pinMode(btn_3, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(btn_1), btnPress1, HIGH);
  attachInterrupt(digitalPinToInterrupt(btn_2), btnPress2, HIGH);
  attachPCINT(digitalPinToPCINT(btn_3), btnPress3, HIGH);

  for (int i = 0; i < 3; i++) {
    int s = EEPROM.read(i);
    if (s < 0 || s > 7) {
      EEPROM.write(i, 0);
    }
  }
  Light_1_Mode = EEPROM.read(0);
  Light_2_Mode = EEPROM.read(1);
  Light_3_Mode = EEPROM.read(2);


  //********************* setting up neo pixel  and other things ****************************************
}

void loop() {

  DateTime now = myRTC.now();

  byte actualHour = now.hour();
  byte actualMinute = now.minute();

  int h_o_u_r = actualHour;

  Tens = (actualMinute / 10);
  Units = (actualMinute - (Tens * 10));

  if (h_o_u_r > 12)    {
    hour_ = h_o_u_r  - 12;
  }
  else {
    hour_ = h_o_u_r;
  }

  //************************ for controlling neopixel pannel ****************************************

  int x = analogRead(A0);

  ldr_value = map(x, 1024, 0, 20, 255);   // if you don't want control intensity just write   ldr_value =255;

  //************************ for controlling neopixel pannel ****************************************

  //**********************************************simple modified **************************************

  switch (hour_) {
    case 1:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(10, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 2:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(10, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(3, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 3:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(10, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 4:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(0, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(6, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 5:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(0, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(6, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(10, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 6:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(0, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(3, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(6, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(10, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 7:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(0, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(1, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(5, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(6, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(10, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 8:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(0, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(1, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(5, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(6, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(8, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(12, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 9:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(0, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(1, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(3, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(5, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(6, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(8, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(12, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 10:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(0, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(1, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(3, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(5, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(6, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(8, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(10, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(12, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 11:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(0, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(1, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(3, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(5, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(6, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(8, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(9, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(11, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(12, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
    case 12:
      for (byte i = 0; i < 14; i = i + 1)        {
        pixels_1.setPixelColor(myARay[i], pixels_1.Color(0, 0, 0));
        //delay(5);
      }
      pixels_1.setPixelColor(0, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(1, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(2, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(4, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(5, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(6, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(7, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(8, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(9, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(11, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(12, set_Light_1_mode(Light_1_Mode));
      pixels_1.setPixelColor(13, set_Light_1_mode(Light_1_Mode));
      pixels_1.setBrightness(ldr_value);
      pixels_1.show();
      break;
  }
  ///////////////////////////////////////////////////////////////////////TENS UNITS
  switch (Units) {
    case 0:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.show();
      break;
    case 1:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.setPixelColor(10, set_Light_3_mode(Light_3_Mode));
      pixels_3.setBrightness(ldr_value);
      pixels_3.show();
      break;
    case 2:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.setPixelColor(3, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(10, set_Light_3_mode(Light_3_Mode));
      pixels_3.setBrightness(ldr_value);
      pixels_3.show();
      break;
    case 3:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.setPixelColor(3, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(7, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(13, set_Light_3_mode(Light_3_Mode));
      pixels_3.setBrightness(ldr_value);
      pixels_3.show();
      break;
    case 4:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.setPixelColor(0, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(6, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(7, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(13, set_Light_3_mode(Light_3_Mode));
      pixels_3.setBrightness(ldr_value);
      pixels_3.show();
      break;
    case 5:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.setPixelColor(0, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(1, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(3, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(5, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(6, set_Light_3_mode(Light_3_Mode));
      pixels_3.setBrightness(ldr_value);
      pixels_3.show();
      break;
    case 6:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.setPixelColor(7, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(8, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(9, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(11, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(12, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(13, set_Light_3_mode(Light_3_Mode));
      pixels_3.setBrightness(ldr_value);
      pixels_3.show();
      break;
    case 7:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.setPixelColor(0, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(1, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(3, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(5, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(6, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(7, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(13, set_Light_3_mode(Light_3_Mode));
      pixels_3.setBrightness(ldr_value);
      pixels_3.show();
      break;
    case 8:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.setPixelColor(0, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(1, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(5, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(6, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(7, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(8, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(12, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(13, set_Light_3_mode(Light_3_Mode));
      pixels_3.setBrightness(ldr_value);
      pixels_3.show();
      break;
    case 9:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_3.setPixelColor(myARay[j], pixels_3.Color(0, 0, 0));
        //delay(5);
      }
      pixels_3.setPixelColor(0, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(1, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(5, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(6, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(7, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(8, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(10, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(12, set_Light_3_mode(Light_3_Mode));
      pixels_3.setPixelColor(13, set_Light_3_mode(Light_3_Mode));
      pixels_3.setBrightness(ldr_value);
      pixels_3.show();
      break;
  }
  //////////////////////////////////////////////////////////////////////////////TENS MINUTES
  switch (Tens) {
    case 0:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
        //delay(5);
      }
      pixels_2.show();
      break;
    case 1:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
        //delay(5);
      }
      pixels_2.setPixelColor(10, set_Light_2_mode(Light_2_Mode));
      pixels_2.setBrightness(ldr_value);
      pixels_2.show();
      break;
    case 2:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
        //delay(5);
      }
      pixels_2.setPixelColor(3, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(10, set_Light_2_mode(Light_2_Mode));
      pixels_2.setBrightness(ldr_value);
      pixels_2.show();
      break;
    case 3:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
        //delay(5);
      }
      pixels_2.setPixelColor(7, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(10, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(13, set_Light_2_mode(Light_2_Mode));
      pixels_2.setBrightness(ldr_value);
      pixels_2.show();
      break;
    case 4:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
        //delay(5);
      }
      pixels_2.setPixelColor(0, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(6, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(7, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(13, set_Light_2_mode(Light_2_Mode));
      pixels_2.setBrightness(ldr_value);
      pixels_2.show();
      break;
    case 5:
      for (byte j = 0; j < 14; j = j + 1)        {
        pixels_2.setPixelColor(myARay[j], pixels_2.Color(0, 0, 0));
        //delay(5);
      }
      pixels_2.setPixelColor(7, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(8, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(10, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(12, set_Light_2_mode(Light_2_Mode));
      pixels_2.setPixelColor(13, set_Light_2_mode(Light_2_Mode));
      pixels_2.setBrightness(ldr_value);
      pixels_2.show();
      break;
  }
  rain_jn1 += rainSpeed;
  rain_jn2 += rainSpeed;
  rain_jn3 += rainSpeed;
  //**********************************************simple modified **************************************
}

void btnPress1() {
  detachInterrupt(digitalPinToInterrupt(btn_1));
  Light_1_Mode++;
  if (Light_1_Mode > 7) {
    Light_1_Mode = 0;
  }
  EEPROM.write(0, Light_1_Mode);
  delay(500);
  attachInterrupt(digitalPinToInterrupt(btn_1), btnPress1, HIGH);
}

void btnPress2() {
  detachInterrupt(digitalPinToInterrupt(btn_2));
  Light_2_Mode++;
  if (Light_2_Mode > 7) {
    Light_2_Mode = 0;
  }
  EEPROM.write(1, Light_2_Mode);
  delay(500);
  attachInterrupt(digitalPinToInterrupt(btn_2), btnPress2, HIGH);
}

void btnPress3() {
  disablePinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_3));
  Light_3_Mode++;
  if (Light_3_Mode > 7) {
    Light_3_Mode = 0;
  }
  EEPROM.write(2, Light_3_Mode);
  delay(500);
  enablePinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_3));
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
