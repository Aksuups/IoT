/*
 * ------------------------------------------------------------------------ *
 *  Source code for displaying motorhome gas bottle levels using locally    *
 *  created server, which can be accessed through web browser.              *
 * ------------------------------------------------------------------------ *
 *  Microcontroller: ESP32                                                  *
 *  Sensor: HX711 load cell amplifier                                       *
 *  ----------------------------------------------------------------------- *
 *  Developed by Aleksi Jokinen © 2021                                      *
 *  https://www.github.com/Aksuups                                          *
 * ------------------------------------------------------------------------ *
 *  SENSOR WIRING                                                           *
 *  HX711    => ESP32                                                       *
 *  Vcc pin  => 3.3V                                                        *
 *  GND pin  => GND                                                         *
 *  SCK pin  => GPIO 25 (D25)                                               *
 *  DOUT pin => GPIO 26 (D26)                                               *
 * ------------------------------------------------------------------------ *
 *  SENSOR WIRING                                                           *
 *  BME280   => ESP32                                                       *
 *  Vcc pin  => 3.3V                                                        *
 *  GND pin  => GND                                                         *
 *  SCL pin  => GPIO 22 (D22) I2C Clock                                     *
 *  SDA pin  => GPIO 21 (D21) I2C Data                                      *
 * ------------------------------------------------------------------------ *
 *                           !!!     NOTICE     !!!                         *
 *             THIS SOURCE CODE IS IN ACTIVE DEVELOPMENT AND WILL           *
 *            CHANGE A LOT DURING THIS PHASE. WHILE IN DEVELOPMENT,         *
 *        THERE WILL BE LINES OF CODE FOR DEBUGGING AND OTHER PURPOSES.     *
 *        THESE LINES WILL BE REMOVED WHEN NEARING THE FINISHING PHASE,     *
 *                  WHERE CODE WILL BE OPTIMIZED AND CLEANED.               *
 * ------------------------------------------------------------------------ *
 *                                   TO DO                                  *
 *                             Sensor calibration                           *
 *                         Defining calibration factor                      *
 *                                Sensor Tare                               *
 *                             HTML Improvements                            *
 *                            Notification system                           *
 * ------------------------------------------------------------------------ *
*/
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <HX711.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
//#include "soc/rtc.h"

// HX711 DT and SCK definitions. 
#define DOUT_PIN 26
#define SCK_PIN 25
#define SEALEVELPRESSURE_HPA (1013.25)

// Define global variables.
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000; 
float data1, data2, temperature, humidity, pressure;

// Initialize HX711 LC amplifier.
HX711 scale;
Adafruit_BME280 bme;

// Network connection configuration.
const char* ssid = "ARRIS-B9BA";
const char* passwd = "D6BE18D1C13CCC4B";

// Set static IP address for the ESP32.
IPAddress local_ip(192, 168, 0, 68);
IPAddress gateway(192, 168, 0, 1); // Defaults to the device IP address with the last octet set to 1
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8); // Google's Public DNS, Secondary: 8.8.4.4 

// Initialize connection.
WiFiClient client;
WiFiMulti WiFiMulti;
HTTPClient ask;

// Server configuration.
WiFiServer server(80);
String header;

void getData(){
  if (scale.is_ready()){
    data1 = random(10, 1100) / 100.0; // DEBUG (Sensor not connected to ESP32)
    data2 = random(10, 1100) / 100.0; // DEBUG (Sensor not connected to ESP32)
    //data = scale.read();
    Serial.print("Tank 1: ");
    Serial.print(data1);
    Serial.println(" kg");
    Serial.print("Tank 2: ");
    Serial.print(data2);
    Serial.println(" kg");
  } 
  else 
  {
    data1 = random(10, 1100) / 100.0; // DEBUG (Sensor not connected to ESP32)
    data2 = random(10, 1100) / 100.0; // DEBUG (Sensor not connected to ESP32)
    Serial.print("Tank 1: ");
    Serial.print(data1);
    Serial.println(" kg");
    Serial.print("Tank 2: ");
    Serial.print(data2);
    Serial.println(" kg");

    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure() / 100.0F;
    Serial.print("Temperature: "); 
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");
  }
  delay(10000);
}

void setup(){
  Serial.begin(115200);
  if (!bme.begin(0x76)) { //Check if BME280 sensor is present.
    Serial.println("BME280 sensor not found.");
    while (1);
    }
  delay(10000);
  Serial.println("Monitoring system for tracking gas bottle levels.");
  delay(500);
  Serial.println("Additionally software tracks");
  delay(500);
  Serial.println("humidity, pressure and temperature in the environment.\n");
  delay(500);
  Serial.println("Software version: 1.0 (beta).");
  delay(500);
  Serial.println("Developed by Aleksi Jokinen © 2021\n");
  Serial.println("Initialising program....\n");
  delay(2000);

  //rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M); // Adjust ESP32 clock rate frequency to the timing of the HX711 (80MHz).
  scale.begin(DOUT_PIN, SCK_PIN);

  //Apply IP address, gateway and subnet mask defined in configuration.
  WiFi.config(local_ip, gateway, subnet);

  // Initialize Access Point mode for ESP32.
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, passwd);

  // Connect ESP32 to the network.
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, passwd);
  Serial.println("Establishing connection to: ");
  Serial.println(ssid);
  // Wait until device is connected to the network.
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
  }
  // Print IP Address to serial monitor when connected.
  delay(2000);
  Serial.println("\nStatus: OK");
  Serial.println("Connection established.");
  Serial.print("Network: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server.
  Serial.println("\nBooting up the server...");
  delay(2000);
  server.begin();
  Serial.println("Server succesfully started.\n");
  delay(500);
  Serial.println("Initialising completed.\n");
}

void loop(){
  getData();
  WiFiClient client = server.available();
  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;                                                 // If a new client connects,
    Serial.println("New Client.");                                              // print a message out in the serial port
    String currentLine = "";                                                    // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {   // loop while the client's connected.
      currentTime = millis();
      if (client.available()) {                                                 // If there's bytes to read from the client,
        char c = client.read();                                                 // read a byte and print it on the serial.
        Serial.write(c);                                                        
        header += c;
        if (c == '\n') {                                                        
          if (currentLine.length() == 0) {
            // Send a response to the client HTTP request.
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><title>Motorhome sensor status monitor</title>");
            client.println("<html>");
            client.println("<style>");
            client.println(".card{");
            client.println("font-family: Tahoma, sans-serif;");
            client.println("max-width: 600px;");
            client.println("min-height: 300px;");
            client.println("background: #02b875;");
            client.println("padding: 30px;");
            client.println("box-sizing: border-box;");
            client.println("color: #FFF;");
            client.println("margin:20px;");
            client.println("box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);");
            client.println("}");
            client.println("</style>");
            client.println("<body>");
            client.println("<div class=\"card\">");
            client.println("<h2>Gas status monitor</h2>");
            client.println("Gas on tank 1:<span id=\"data\">");
            client.println(data1);       // Push sensor data to the html-page.
            client.println("</span> kg<br>");
            client.println("Gas on tank 2:<span id=\"data\">");
            client.println(data2);       // Push sensor data to the html-page.
            client.println("</span> kg<br>");
            client.println("<h2>Motorhome in cabin monitor</h2>");
            client.println("Temperature:<span id=\"data\">");
            client.println(temperature);       // Push sensor data to the html-page.
            client.println("</span>  C<br>");
            client.println("Humidity:<span id=\"data\">");
            client.println(humidity);       // Push sensor data to the html-page.
            client.println("</span>  %<br>");
            client.println("Pressure:<span id=\"data\">");
            client.println(pressure);       // Push sensor data to the html-page.
            client.println("</span>  hPa");
            client.println("<br>");
            client.println("<br>");
            client.println("<button class=\"button\" onClick=\"window.location.reload()\">Refresh</button>");         //Click-button for updating data.
            client.println("</div>");
            client.println("<br><br><br><a href=\"https://github.com/Aksuups/IoT/tree/main/IoT/ESP32/Level%20tracker%20for%20gas%20bottles\">Projects Github page</a>");
            client.println("</body>");
            client.println("</html>");
            client.println();           // End HTTP response with blank line.
            break;                      // Break out of the while loop.
          } else {                      // If newline, clear currentline.
            currentLine = "";
          }
        } else if (c != '\r') {         // If not carriage return character,
          currentLine += c;             // add it to the end of the currentLine.
        }
      }
    }
    header = "";                        // Clear the header variable
    client.stop();                      // Close the connection.
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}