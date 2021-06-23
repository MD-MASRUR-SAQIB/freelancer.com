#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>
#include "WiFiEsp.h"

int RXPin = 2;
int TXPin = 3;

int GPSBaud = 9600;

//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial serial_connection (11, 10);
SoftwareSerial gpsSerial(RXPin, TXPin);
TinyGPSPlus gps;

const int sensor = A1; // Assigning analog pin A1 to variable 'sensor'
float tempc;  //variable to store temperature in degree Celsius
float tempf;  //variable to store temperature in Fahreinheit 
float vout;  //temporary variable to hold sensor reading

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "";            // your network SSID (name)
char pass[] = "";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiEspServer server(80);

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);
  serial_connection.begin(9600);
  gpsSerial.begin(GPSBaud);

pinMode(sensor,INPUT); // Configuring pin A1 as input
Serial.begin(9600);

lcd.begin();//Initializing display
lcd.backlight();//To Power ON the back light
//lcd.backlight();// To Power OFF the back light

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  void printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}
void loop()
{
   while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
       void displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }

  lcd.setCursor(0,0); //Defining positon to write from first row,first column .
lcd.print(" Dog Tracker");
delay(1000);//Delay used to give a dynamic effect
lcd.setCursor(0,1);  //Defining positon to write from second row,first column .
lcd.print(" Device " );
delay(5000); 
lcd.clear();//Clean the screen
lcd.setCursor(0,0); 
lcd.print(" IoT Project ");
lcd.setCursor(0,1);
lcd.print(" 2021 ");
lcd.clear();
delay(3000); 

vout=analogRead(sensor);
vout=(vout*500)/1023;
tempc=vout; // Storing value in Degree Celsius
tempf=(vout*1.8)+32; // Converting to Fahrenheit 
lcd.setCursor(0,0);
lcd.print(" Temparature ");
delay(3000);
lcd.setCursor(0,0);
lcd.print("Degrees=C");
lcd.print(tempc);
lcd.setCursor(0,1);
lcd.print("Fahrenheit=F");
lcd.print(tempf);
delay(10000); //Delay of 10 seconds for ease of viewing in serial monitor
lcd.clear();
delay(5000);

  // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("New client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");
          
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 20\r\n"        // refresh the page automatically every 20 seconds
            "\r\n");
       client.println(F(R"=====( 
<!DOCTYPE html>
<html>
<title>Dog Tracker - Iot Project</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
<body>

<div class="Website_bar">
  <div class="w3-bar w3-white w3-wide w3-padding w3-card">
    <a href="#home" class="w3-bar-item w3-button"><b>Dog Tracker</b></a>
    
    <div class="w3-right w3-hide-small">
      <a class="w3-bar-item w3-button">Project</a>
      <a class="w3-bar-item w3-button">About</a>
      <a class="w3-bar-item w3-button">Sign in</a>
    </div>
  </div>
</div>

<header class="w3-display-container w3-content w3-wide" style="max-width:1500px;" id="home">
  <img class="w3-image" src="https://images.pexels.com/photos/593322/pexels-photo-593322.jpeg?cs=srgb&dl=pexels-jessica-lewis-593322.jpg&fm=jpg" alt="Architecture" width="1500" height="800">
  <div class="w3-display-middle w3-margin-top w3-center">
    <h1 class="w3-xxlarge w3-text-white"><span class="w3-padding w3-black w3-opacity-min"><b>Welcome</b></span> <span class="w3-hide-small w3-text-light-grey">
    
  </div>
</header>

<html>
<head>
<style>
.flex-container {
  display: flex;
  flex-wrap: nowrap;
  background-color: White;
  text-align: center;
}

.flex-container > div {
  background-color: White;
  height: 100px;
  width: 500px;
  margin: 10px;
  text-align: center;
  line-height: 75px;
  font-size: 30px;
}
</style>
</head>
<body>

<div class="flex-container">
  <div> <img src="https://studentsurvey.ie/sites/default/files/2019-10/GMIT_Logo_2012RGBresized_0.jpeg" alt="GMIT" style="width:350px"></div>
  <div><img src="https://i.postimg.cc/fTrTv1BS/dog.png' border='0' alt='dog" alt="Dog" style="width:350px"></div>
  <div><img src= "https://i.postimg.cc/PJfY10KF/GPS.png" alt="Gps" style="width:350px"></div>  
</div>
<html>
<head>
<style>

</body>
</html>
)====="));
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}
}
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}

void printGpsGsm() 
{  
  while(serial_connection.available())
  {
    gps.location.isUpdated();
  }
     
if (gps.location.isUpdated())
{
 Serial.begin(9600);
 Serial.print("\r");
 delay(1000);
 Serial.print("AT+CMGF=1\r");
 delay(1000);
 Serial.print("AT+CMGS=\"35385243321\"\r");
 delay(1000);
 Serial.print("LOST TRACKING OF DOG! Please Check!");
 Serial.print("www.google.com.ph/naga/place");
 Serial.print(gps.location.lat(), 6);
 Serial.print(",");
 Serial.print(gps.location.lat(), 6);
 Serial.print("\r");
 delay(1000);
 Serial.print((char) 26);
 delay(1000);
 Serial.write(0x1A);
 Serial.write(0x00);
 Serial.write(0x0A);
 delay(1000);
}
}
void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}
