/*
 * ------------------------------------------------------------------------ *
 * Source code for displaying motorhome gas bottle levels using locally     *
 * created server, which can be accessed through web browser.               *
 * ------------------------------------------------------------------------ *
 * Microcontroller: ESP32                                                   *
 * Sensor: HX711 load cell amplifier                                        *
 *  ----------------------------------------------------------------------- *
 *  Developed by Aleksi Jokinen © 2021                                      *
 *  https://www.github.com/Aksuups                                          *
 * ------------------------------------------------------------------------ *
 *  CIRCUIT WIRING                                                          *
 *  OUTPUT (HX711 => ESP32)                                                 *
 *  Vcc pin  => 3.3V                                                        *
 *  GND pin  => GND                                                         *
 *  SCK pin  => GPIO 25 (D25)                                               *
 *  DOUT pin => GPIO 26 (D26)                                               *
 * ------------------------------------------------------------------------ *
 *                                   NOTICE                                 *
 *               Lines ending with DEBUG are for debugging purposes,        *
 *         and for simulating sensor before making actual prototype wirings.*
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
//#include "soc/rtc.h"

// HX711 DT and SCK definitions. 
#define DOUT_PIN 26
#define SCK_PIN 25

// Define global variables.
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000; 
float data;

// Initialize HX711 LC amplifier.
HX711 scale;

// Network connection configuration.
const char* ssid = "*****";
const char* passwd = "*****";

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
    data = random(10, 5000) / 100.0; // DEBUG (Sensor not connected to ESP32)
    //data = scale.read();
    Serial.print("HX711 sensor reading : ");
    Serial.println(data);
  } 
  else 
  {
  Serial.println("HX711 sensor not found.");
  data = random(10, 5000) / 100.0; // DEBUG (Sensor not connected to ESP32)
  Serial.println(data); // DEBUG (Sensor not connected to ESP32)
  }
  delay(10000);
}

void setup(){
  Serial.begin(115200);
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
  Serial.println("Status: OK");
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
            client.println("<!DOCTYPE html>");
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
            client.println("<h2>Gas amount monitor</h2><br>");
            client.println("<h2></h2");
            client.println("<h3>Gas on tank:<span id=\"data\">");
            client.println(data);       // Push sensor data to the html-page.
            client.println("</span> kg</h3><br>");
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