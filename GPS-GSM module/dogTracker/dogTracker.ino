#include  <SoftwareSerial.h>
#include  <ArduinoJson.h>
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
#include  <TinyGPS++.h>
#include  "WiFiESP.h"

#define   simRx               2
#define   simTx               3
#define   gpsRx               10
#define   gpsTx               11
#define   espRx               6
#define   espTx               7
#define   ssid                "Your WiFi Name"
#define   password            "Your WiFi Password"

String    textSendingNumber   = "Enter the number of your phone, where you will get sms";
int       tempSensor          = A1; //Temperature Sensor Pin

float     Latitude = 0.0, Longitude = 0.0, Altitude = 0; //Variable to store Latitude & Longitude & Altitude
float     tempc, vout; //Variable to store temperature values
uint32_t  prev, prev2; //Variable to create events for different functions
int       scrNum = 1; //display different screens

SoftwareSerial    simSerial (simRx, simTx);
SoftwareSerial    gpsSerial (gpsRx, gpsTx);
SoftwareSerial    espSerial (espRx, espTx);
LiquidCrystal_I2C lcd(0x27, 16, 2);
TinyGPSPlus       gps;


int status = WL_IDLE_STATUS;
int reqCount = 0;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 5000;

String header;

WiFiEspServer server(80);


void setup() {
  String data;
  Serial.begin(9600);

  simSerial.begin(9600);

  simSerial.println("AT&W");
  delay(500);
  while (simSerial.available()) {
    data = simSerial.readString();
    //Serial.println(data);
  }
  simSerial.println("AT+CMGF=1");//Changing the sim Module to messaging mode
  delay(500);
  while (simSerial.available()) {
    data = simSerial.readString();
    //Serial.println(data);
  }
  simSerial.println("AT+CNMI=1,2,0,0,0");//Changing the sim Module to messaging mode
  delay(500);
  while (simSerial.available()) {
    data = simSerial.readString();
    //Serial.println(data);
  }
  simSerial.println("AT+CMGD=1,4");//Changing the sim Module to messaging mode
  delay(500);
  while (simSerial.available()) {
    data = simSerial.readString();
    //Serial.println(data);
  }


  gpsSerial.begin(9600);

  pinMode(tempSensor, INPUT);

  //initiating Display and showing first screen
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("  Dog Tracker   ");
  lcd.setCursor(0, 1);
  lcd.print("IoT Project 2021");
  delay(3000);

  espSerial.begin(9600);
  WiFi.init(&espSerial);

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);
  }

  Serial.println("You're connected to the network");

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();

  server.begin();

  prev = millis();
  prev2 = millis();
}

void loop() {
  simSerial.listen(); //Changing the simSerial active
  incomingProcess();  //Looking for new message

  espSerial.listen();
  espClient();

  if ( millis() - prev > 5000) { // doing this function after every 5 seconds
    prev = millis();
    getTemp(); //getting temperature

    gpsSerial.listen();//Changing the simSerial active
    getGPS(); //Looking for new co-ordinates

    sendESP();  //Sending the location data to ESP
  }
  if (millis() - prev2 > 3000) { // changing screen after every 3 seconds
    prev2 = millis();
    if (scrNum == 0) { // screen 1 contents
      scrNum = 1;
      lcd.setCursor(0, 0);
      lcd.print("Dog Tracker GPS!");
      lcd.setCursor(0, 1);
      lcd.print("Lon ");
      lcd.print(Longitude);
      lcd.print("        ");
    }
    else if (scrNum == 1) { // screen 2 contents
      scrNum = 2;
      lcd.setCursor(0, 0);
      lcd.print("IoT Project 2021");
      lcd.setCursor(0, 1);
      lcd.print("Temp ");
      lcd.print(tempc);
      lcd.print(" C    ");
    }
    else { // screen 3 contents
      scrNum = 0;
      lcd.setCursor(0, 0);
      lcd.print("Dog Tracker GPS!");
      lcd.setCursor(0, 1);
      lcd.print("Lat ");
      lcd.print(Latitude);
      lcd.print("        ");
    }
  }
}

void incomingProcess() { // checking for new message
  String data = "";
  uint32_t l_timer = millis();
  while (millis() - l_timer < 250) {
    while (simSerial.available()) {
      char l_char;
      l_char = simSerial.read();
      data += l_char;
      l_timer = millis();
    }
  }
  //Serial.print(data);
  if (data.indexOf("locate") != -1) { // sending location info to the phone number
    String serialData = "AT+CMGS=\"" + textSendingNumber + "\"";
    String message = "Your dog is now at Longitude: " + (String)Longitude
                     + ", Latitude: " + (String)Latitude
                     + "\nwww.google.com/maps/?q="
                     + (String)Latitude + "," + (String)Longitude;

    simSerial.println(serialData);
    delay(500);
    simSerial.print(message);
    delay(500);
    simSerial.write(26);
    delay(500);
    simSerial.print("AT");
    delay(500);
    while (simSerial.available()) {
      data = simSerial.readString();
      //Serial.println(data);
    }
    delay(500);
  }
}

void sendESP() { // sending data to esp
  String info = "{\"Lat\":" + (String)Latitude + ",\"Lon\":"
                + (String)Longitude + "}";

  Serial.println(info);
}


void getTemp() { //calculating temperature
  vout = analogRead(tempSensor);
  vout = (vout * 500) / 1023;
  tempc = vout;
}


void getGPS() { //getting gps info

  uint32_t l_timer = millis();
  while (millis() - l_timer < 1000) {
    while (gpsSerial.available() > 0) {
      if (gps.encode(gpsSerial.read()))
        if (gps.location.isValid()) {
          Latitude = gps.location.lat();
          //Serial.print("Latitude: ");
          //Serial.println(gps.location.lat(), 6);
          Longitude = gps.location.lng();
          //Serial.print("Longitude: ");
          //Serial.println(gps.location.lng(), 6);
          Altitude = gps.altitude.meters();
          //Serial.print("Altitude: ");
          //Serial.println(gps.altitude.meters());
        }
        else {
          //Serial.println("Location: Not Available");
        }
    }
  }
}

void espClient() {
  WiFiEspClient client = server.available();
  if (client) {
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<style>");
            client.println(".flex-container {");
            client.println("display: flex;");
            client.println("flex-wrap: nowrap;");
            client.println("background-color: White;");
            client.println("text-align: center;");
            client.println("}");

            client.println(".flex-container > div {");
            client.println("background-color: White;");
            client.println("height: 100px;");
            client.println("width: 500px;");
            client.println("margin: 10px;");
            client.println("text-align: center;");
            client.println("line-height: 75px;");
            client.println("font-size: 30px;");
            client.println("}");

            client.println(".backround {");
            client.println("background-image: url(\"https://images.pexels.com/photos/593322/pexels-photo-593322.jpeg?cs=srgb&dl=pexels-jessica-lewis-593322.jpg&fm=jpg\");");
            client.println("background-size: 100%;");
            client.println("width: 100%;");
            client.println("height: auto;");
            client.println("}");

            client.println("@media screen and (max-width: 500px) {");
            client.println(".column {");
            client.println("width: 100%;");
            client.println("}");
            client.println("}");

            client.println(".row {");
            client.println("display: flex;");
            client.println("}");

            client.println(".column {");
            client.println("flex: 33.33%;");
            client.println("padding: 5px;");
            client.println("}");

            client.println("</style>");
            client.println("<title>Dog Tracker - Iot Project</title>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">");
            client.println("</head>");

            client.println("<body>");
            client.println("<div class=\"Website_bar\">");
            client.println("<div class=\"w3-bar w3-white w3-wide w3-padding w3-card\">");
            client.println("<a href=\"#home\" class=\"w3-bar-item w3-button\"><b>Dog Tracker</b></a>");
            client.println("<div class=\"w3-right w3-hide-small\">");
            client.println("<a class=\"w3-bar-item w3-button\">IoT Project 2021</a>");
            client.println("</div>");
            client.println("</div>");
            client.println("</div>");

            client.println("<div class=\"backround\">");
            client.println("<header class=\"w3-display-container w3-content w3-wide\" style=\"max-width:100%;\" id=\"home\">");
            client.println("<center><img class=\"w3-image\" src=\"https://i.postimg.cc/fTrTv1BS/dog.png' border='0' alt='dog\" align=\"center\" alt=\"Dog\" width=\"20%\" height=\"auto\" ></center>");
            client.println("<div class=\"w3-display-middle w3-margin-top w3-center\">");
            client.println("<h1 class=\"w3-xxlarge w3-text-white\">");
            client.println("<span class=\"w3-padding w3-black w3-opacity-min\">");
            client.println("<b>Welcome</b>");
            client.println("</span> ");
            client.println("</h1>");
            client.println("</div>");
            client.println("</header>");

            client.println("</div>");
            client.println("<br>");

            client.println("<center><h2><b>Location of the Dog</b></h2></center>");


            client.println("<div class=\"Project\">");
            client.println("<br>");
            client.println("<center>");
            client.println("<iframe src=\"https://www.google.com/maps/embed/v1/place?key=AIzaSyCF00irKBkN20JWQ8ipByUX6D1MCYlQZmI&q="
                           + (String)Latitude + "," + (String)Longitude + "&zoom=10&maptype=roadmap\" width=\"70%\" height=\"450\" style=\"border:0;\" allowfullscreen=\"\" loading=\"lazy\"></iframe>");
            client.println("</center>");
            client.println("<br>");

            client.println("<div class=\"row\">");
            client.println("<div class=\"column\">");
            client.println("<img src=\"https://studentsurvey.ie/sites/default/files/2019-10/GMIT_Logo_2012RGBresized_0.jpeg\" alt=\"Logo\" style=\"width:100%\">");
            client.println("</div>");
            client.println("<div class=\"column\">");
            client.println("<center>");
            client.println("<h2> Hope you have liked this project! </h2>");
            client.println("<p> Thank you </p>");
            client.println("</center>");
            client.println("</div>");
            client.println("<div class=\"column\">");
            client.println("<img src=\"https://i.postimg.cc/PJfY10KF/GPS.png\" alt=\"GPS\" style=\"width:100%\">");
            client.println("</div>");
            client.println("</div>");
            client.println("</div>");


            client.println("</body>");

            client.println("</html>");

            client.println();
            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }
      }

    }
    header = "";
    client.stop();
  }
}
