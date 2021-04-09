/*------------------------------------------------------------------------------*/
  /*Edited version of the implementation made for University assignment. 
   * NOTICE: Apikeys shown on the older version are for the trial version
   * which has been expired, so they are useless. 
   */
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
/*Definitions for connection*/
WiFiClient client;
WiFiMulti WiFiMulti;
HTTPClient ask;
/*------------------------------------------------------------------------------*/
/*Connection config*/
const char* ssid     = "ssid"; //Wifi SSID
const char* password = "password"; //Wifi Password
/*Timing definitions, value as milliseconds*/
unsigned long previousMillis = 18000000; /*Last time data was written (0 for first upload to be at 30min; 18000000 for first upload at power on)*/
unsigned int writeInterval = 1800000;    /*30 minute interval for the data*/
/*------------------------------------------------------------------------------*/
/*asksensors.com API host config*/
const char* host = "api.asksensors.com";  /*API host name*/
const int httpPort = 80;      // port
/*------------------------------------------------------------------------------*/
/*Define sensor pins and global variables used in the program*/
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
  pinMode(LUX, INPUT);
  pinMode(PIR, INPUT);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PIR), movement, CHANGE);
  /*Open serial*/
  Serial.begin(115200);
  Serial.println("*****************************************************");
  Serial.println("Program Start : Connect ESP32 to AskSensors.");
  Serial.println("Wait for WiFi... ");
    //Check if BME280 sensor is present.
    if (!bme.begin(0x76)) {
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
}
/*------------------------------------------------------------------------------*/
/*Use WiFiClient class to create TCP connections*/
void connectToService(){
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }else {
    Serial.println("Uploading sensor data to cloud...");
  }
}
/*------------------------------------------------------------------------------*/
/*Close TCP connection when it is not needed*/
void disconnectFromService(){
  client.stop();
  Serial.println("Sensor data uploaded.");
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
  connectToService();
  // Create a URL for updating all modules
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100;
  String url = "http://api.asksensors.com/write/";
  url += apiKeyIn;
  url += "?module1=";
  url += temperature;
  url += "&module2=";
  url += temperature;
  url += "&module3=";
  url += humidity;  
  url += "&module4=";
  url += pressure;

  Serial.print("Requesting URL for Sensor1 data: ");
  Serial.println(url);
  Serial.println("*****************************************************");
  Serial.print("Temperature: "); 
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.println("*****************************************************");
  
    // Send data
    ask.begin(url); //Specify the URL
    //Check for the returning code
    int httpCode = ask.GET();          
    if (httpCode > 0) { 
        String payload = ask.getString();
        Serial.print("Http code: "); 
        Serial.println(httpCode);
        Serial.print("Sensor1 modules: ");
        Serial.println(payload);
      } else {
      Serial.println("Error on HTTP request");
    }
    ask.end(); //End
    disconnectFromService();
    Serial.println("*****************************************************");
    Serial.println();
}
/*------------------------------------------------------------------------------*/
 /*Function to upload sensor2 data. Inside this function TCP connection is opened
   when data is uploaded and closed when upload has been done*/
void sensor2() {
  connectToService();
  int lux = analogRead(34) * 0.9765625; //Convert current to lux using y = 1/2(x) + 0
  String url2 = "http://api.asksensors.com/write/";
  url2 += apiKeyInSensor2;
  url2 += "?module1=";
  url2 += lux;
  url2 += "&module4=";
  url2 += OWMtemperature;  
  Serial.print("Requesting URL for Sensor2 data: ");
  Serial.println(url2);
  Serial.println("*****************************************************");
  Serial.print("Lux: ");
  Serial.println(lux);
  Serial.print("Openweathermap.org temp: ");
  Serial.println(OWMtemperature);
  Serial.println("*****************************************************");
    // Send data 
    ask.begin(url2); //Specify the URL
    //Check for the returning code
    int httpCode2 = ask.GET();          
    if (httpCode2 > 0) { 
        String payload2 = ask.getString();
        Serial.print("Http code: ");
        Serial.println(httpCode2);
        Serial.print("Sensor2 modules: ");
        Serial.println(payload2);
      } else {
      Serial.println("Error on HTTP request");
    }
    ask.end(); //End 
    disconnectFromService();
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
  connectToService();
  String url3 = "http://api.asksensors.com/write/";
  url3 += apiKeyInSensor2;
  url3 += "?module2=";
  url3 += pir;
  url3 += "&module3=";
  url3 += pir;
  Serial.print("Requesting URL for PIR data: ");
  Serial.println(url3);
  Serial.println("*****************************************************");
  Serial.print("Movement: ");
  Serial.println(pir_movement);
  Serial.println("*****************************************************");
    // Send data 
    ask.begin(url3); //Specify the URL
    //Check for the returning code
    int httpCode3 = ask.GET();          
    if (httpCode3 > 0) { 
        String payload3 = ask.getString();
        Serial.print("Http code: ");
        Serial.println(httpCode3);
        Serial.print("PIR module: ");
        Serial.println(payload3);
      } else {
      Serial.println("Error on HTTP request");
    }
    ask.end(); //End
    disconnectFromService();
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
