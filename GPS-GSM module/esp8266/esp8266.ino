#include <ArduinoJson.h> // For extracting data getting from Arduino Uno
#include <ESP8266WiFi.h>

//Change the info below
#define   ssid                "Masrur Saqib"
#define   password            "fatema03"


DynamicJsonDocument doc(256);

WiFiServer server(80);

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 5000;

String header;

String    SerialData = "";
String    output     = "";
float     Latitude = 0.0, Longitude = 0.0;

void setup() {
  ESP.eraseConfig();
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

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

  if (Serial.available()) {
    SerialData = Serial.readString();
    deserializeJson(doc, SerialData);
    Latitude = doc["Lat"];
    Longitude = doc["Lon"];
  }
}
