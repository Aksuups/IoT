/*------------------------------------------------------------------------------*/
  /*Weatherstation running on the webserver. 
  /*Aleksi Jokinen*/
/*------------------------------------------------------------------------------*/
/*Libraries used by the program*/
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
/*Library for decoding the JSON data*/
#include <ArduinoJson.h>
/*------------------------------------------------------------------------------*/
/*Sensor used*/
Adafruit_BME280 bme;

#define SEALEVELPRESSURE_HPA (1013.25)

/*Definitions for connection*/
WiFiClient client;
WiFiMulti WiFiMulti;
HTTPClient ask;
/*------------------------------------------------------------------------------*/
/*Connection config*/
const char* ssid     = "ssid"; //Wifi SSID
const char* password = "passwd"; //Wifi Password
/*Timing definitions, value as milliseconds*/
unsigned long previousMillis = 18000000; /*Last time data was written (0 for first upload to be at 30min; 18000000 for first upload at power on)*/
unsigned int writeInterval = 1800000;    /*30 minute interval for the data*/
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000; 
/*------------------------------------------------------------------------------*/
//Webserver configuration
WiFiServer server(80);
String header;
/*------------------------------------------------------------------------------*/
/*Define sensor pins and global variables used in the application*/
int LUX = 34;
byte PIR = 26;
byte ledPin = 4;
String pir_movement;
int pir;
int pir_previous;
float temperature = 0, humidity = 0, pressure = 0;
float OWMtemperature;
/*------------------------------------------------------------------------------*/
void setup(){
   bool status;
  pinMode(LUX, INPUT);
  pinMode(PIR, INPUT);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PIR), movement, CHANGE);
  /*Open serial*/
  Serial.begin(115200);
  Serial.println("*****************************************************");
  Serial.println();
  Serial.println("Establishing wifi connection...");
    if (!bme.begin(0x76)) //Check if BME280 sensor is present.
    {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
    }
  /*Connect to wifi network*/
  WiFiMulti.addAP(ssid, password);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  /*Successful connection*/
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  server.begin();
}
/*------------------------------------------------------------------------------*/
/*Function to get data and parse it from openweathermap.org api using JSON format*/
void GetJSONdata() {
  HTTPClient http;
  http.begin("http://api.openweathermap.org/data/2.5/weather?q=RIIHIMAKI,FI&APPID=12a9ae70c4484ea68a3b691c5ac3d2b8"); /*http request with API key to pull JSON data*/
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);  /*Deserialize JSON data*/
    auto error = deserializeJson(doc, payload); /*If error occurs print error message and return back without continuing to the end of function*/
    if (error) {                    
      Serial.println(F("Parsing unsuccessful"));
      return;
    }
    OWMtemperature = (float)(doc["main"]["temp"]) - 273.15; /*Get only temperature data from requested JSON, no need to parse*/
  }
  http.end();
}
/*------------------------------------------------------------------------------*/
 /*Function to store detected interrupt into variable called pir.
   Also stores string into pir_movement variable used in serial monitor.*/
void movement() {
  byte state = digitalRead(PIR);
  //digitalWrite(ledPin,state);
  if (state == 1) {
  pir = 1;
  pir_movement = "Detected";
  }
  else if (state == 0) {
  pir = 0;
  pir_movement = "Not detected";
  }
}
/*------------------------------------------------------------------------------*/
 /*Function to upload sensor1 data. Inside this function TCP connection is opened
   when data is uploaded and closed when upload has been done*/
void sensor1() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100;
  Serial.println("*****************************************************");
  Serial.print("Temperature: "); 
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.println("*****************************************************");
  Serial.println();
}
/*------------------------------------------------------------------------------*/
 /*Function to upload sensor2 data. Inside this function TCP connection is opened
   when data is uploaded and closed when upload has been done*/
void sensor2() {
  int lux = analogRead(34) * 0.9765625; //Convert current to lux using y = 1/2(x) + 0 
  Serial.println("*****************************************************");
  Serial.print("Lux: ");
  Serial.println(lux);
  Serial.print("Openweathermap.org temp: ");
  Serial.println(OWMtemperature);
  Serial.println("*****************************************************");
    Serial.println("*****************************************************");
    Serial.println();
}
/*------------------------------------------------------------------------------*/
 /*Function to upload movement detection (pir) data. if-else statement used 
   to check whether state of the pir has changed since last time. This is made to prevent
   flooding asksensors modules with continuous ones or zeros, but rather update modules
   if there has been change in the pir value, equaling movement*/
void pir_data() {
  if (pir != pir_previous) {
  Serial.println("*****************************************************");
  Serial.print("Movement: ");
  Serial.println(pir_movement);
  Serial.println("*****************************************************");
  Serial.println();
  } else {
    /*Do nothing*/
  }
  pir_previous = pir; /*Assign same value to both integers for state change check*/
}
/*------------------------------------------------------------------------------*/
/*Loop function which is executed on every processor cycle. 
  Inside the loop function there is counter to see if time interval of 30 minutes has
  passed or not. If it has passed, write execute sensor1 and sensor2 functions to 
  upload data, if time has not passed, do not execute the functions*/
void loop(){
  WiFiClient client = server.available();
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the table 
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
            client.println("table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 12px; background-color: #0043af; color: white; }");
            client.println("tr { border: 1px solid #ddd; padding: 12px; }");
            client.println("tr:hover { background-color: #bcbcbc; }");
            client.println("td { border: none; padding: 12px; }");
            client.println(".sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }");
            
            // Web Page Heading
            client.println("</style></head><body><h1>ESP32 with BME280</h1>");
            client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
            client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
            client.println(bme.readTemperature());
            client.println(" *C</span></td></tr>");  
            client.println("<tr><td>Temp. Fahrenheit</td><td><span class=\"sensor\">");
            client.println(1.8 * bme.readTemperature() + 32);
            client.println(" *F</span></td></tr>");       
            client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
            client.println(bme.readPressure() / 100.0F);
            client.println(" hPa</span></td></tr>");
            client.println("<tr><td>Approx. Altitude</td><td><span class=\"sensor\">");
            client.println(bme.readAltitude(SEALEVELPRESSURE_HPA));
            client.println(" m</span></td></tr>"); 
            client.println("<tr><td>Humidity</td><td><span class=\"sensor\">");
            client.println(bme.readHumidity());
            client.println(" %</span></td></tr>"); 
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > writeInterval) {
  previousMillis = currentMillis;
      sensor1();
      GetJSONdata();
      sensor2();
  }
  pir_data();
  delay(1000);
}
/*------------------------------------------------------------------------------*/
