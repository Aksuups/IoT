/* ------------------------------------------------------------------------ *
 *  Testing tool for BME280 and BMP280 sensors.                             *
 *                                                                          *
 *  ----------------------------------------------------------------------- *
 *  Developed by Aleksi Jokinen © 2021                                      *
 *  https://www.github.com/Aksuups                                          *
 * ------------------------------------------------------------------------ */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

// Define global variables.
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000; 
float data1, data2, temperature, humidity, pressure;
int counter = 1;

Adafruit_BME280 bme;
Adafruit_BMP280 bmp;

// Network connection configuration.
const char* ssid = "*****";
const char* passwd = "*****";

// Set static IP address for the ESP32.
IPAddress local_ip(192, 168, 100, 68);
IPAddress gateway(192, 168, 100, 1); // Defaults to the device IP address with the last octet set to 1.
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);         // Google's Public DNS, Secondary: 8.8.4.4.

// Initialize connection.
WiFiClient client;
WiFiMulti WiFiMulti;
HTTPClient ask;

// Server configuration.
WiFiServer server(80);
String header;

void setup(){
  Serial.begin(9600);
  Serial.println("BME280 and BMP280 sensor testing tool.\n");
  Serial.println("Software version: 1.0");
  Serial.println("Developed by Aleksi Jokinen © 2021\n");

    // Apply IP address, gateway and subnet mask defined in configuration.
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

  if (!bme.begin(0x76)) {
    Serial.println("BME280-sensor not found.");
    Serial.print("Sensor address: ");
    Serial.println(bme.sensorID(),16);
    while(1);
    }

  if (!bmp.begin(0x77)) {
    Serial.println("BMP280-sensor not found.");
    Serial.print("Sensor address: ");
    Serial.println(bmp.sensorID(),16);
    while(1);
    }
}

void Test(){
    Serial.println("Sensor:           BME280          BMP280");
    Serial.print("Address:            0x");
    Serial.print(bme.sensorID(), 16);
    Serial.print("            0x");
    Serial.println(bmp.sensorID(), 16);
    Serial.print("Temperature:       ");
    Serial.print(bme.readTemperature());
    Serial.print("           ");
    Serial.println(bmp.readTemperature() - 1.15); // Calibration value to match BME280 within one decimal place.
    Serial.print("Humidity:          ");
    Serial.print(bme.readHumidity());
    Serial.print("           ");
    Serial.println("--.--");
    Serial.print("Pressure:        ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.print("         ");
    Serial.println(bmp.readPressure() / 100.0F - 0.85); // Calibration value to match BME280 within one decimal place.
}



void loop() {
    Serial.println("-----------------------------------------");
    Serial.print("Test cycle [");
    Serial.print(counter);
    Serial.println("]");

    Test();
    counter++;

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
            client.println("h2 { margin-bottom: -20px;}");
            client.println("td, th {text-align: center; padding: 2px;}");
            client.println(".card{");
            client.println("font-family: Tahoma, sans-serif;");
            client.println("max-width: 600px;");
            client.println("min-height: 350px;");
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
            client.println("<h2>Sensor I2C-address</h2>");
            client.println("<table><tr><td>");
            client.println("BME280&emsp;</td><td>0x");
            client.print(bme.sensorID(),16);       
            client.println("</td><br>");
            client.println("</tr><tr><td>");
            client.println("BMP280&emsp;</td><td>0x");
            client.print(bmp.sensorID(),16);       
            client.println("</td><br></tr></table><br>");
            client.println("<h2>Sensor values</h2>");
            client.println("<table><tr><td>");
            client.println("BME280</td><td>");
            client.println(bme.readTemperature());       
            client.println("&#8451;</td><br>");
            client.println("</tr><tr><td>");
            client.println("BMP280</td><td>");
            client.println(bmp.readTemperature() - 1.15);       
            client.println("&#8451;</td><br>");
            client.println("</tr><tr><td>");
            client.println("BME280</td><td>");
            client.println(bme.readHumidity());       
            client.println("%</td><br>");
            client.println("</tr><tr><td>");
            client.println("BME280</td><td>");
            client.println(bme.readPressure() / 100.0F);       
            client.println("hPa</td></tr>");
            client.println("</tr><tr><td>");
            client.println("BMP280</td><td>");
            client.println(bmp.readPressure() / 100.0F - 0.85);       
            client.println("hPa</td></tr></table>");
            client.println("<br>");
            client.println("<br>");
            client.println("<button class=\"button\" onClick=\"window.location.reload()\">Refresh</button>");
            client.println("</div>");
            client.println("<br><br><br><a href=\"https://github.com/Aksuups/IoT/tree/main/IoT\"target=\"_blank\">Developer github</a>");
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
    delay(20000);
}
