/* ------------------------------------------------------------------------ *
 *  Home temperature system                                                 *            
 *  System uses four (4) BMP-280 sensors, each connected to individual      *
 *  MCU, to measure ambient temperatue in the environment around the house  *
 *                                                                          *
 *  ----------------------------------------------------------------------- *
 *  Developed by Aleksi Jokinen © 2021                                      *
 *  https://www.github.com/Aksuups                                          *
 * ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ *
 *  Testing tool for BME280 and BMP280 sensors.                             *
 *                                                                          *
 *  ----------------------------------------------------------------------- *
 *  Developed by Aleksi Jokinen © 2021                                      *
 *  https://www.github.com/Aksuups                                          *
 * ------------------------------------------------------------------------ */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>

// Initialize BMP280-sensor.
Adafruit_BMP280 bmp;

// Network connection configuration.
const char* ssid = "*****";
const char* passwd = "*****"


void setup(){
  Serial.begin(9600);
  Serial.println("Home ambient temperature monitor system.\n");
  Serial.println("System uses four different BME280-sensors, connected \n");
  Serial.println("to individual MCU's. For measuring temperature in the set measurement points.\n");
  Serial.println("Software version: 1.0");
  Serial.println("Developed by Aleksi Jokinen © 2021\n");

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

  if (!bmp.begin(0x77)) {
    Serial.println("BMP280-sensor not found.");
    Serial.print("Sensor address: ");
    Serial.println(bmp.sensorID(),16);
    while(1);
    }
}

void Test(){
    Serial.println("Sensor: BMP280");
    Serial.print("Address: 0x");
    Serial.println(bmp.sensorID(), 16);
    Serial.print("Temperature:       ");
    Serial.println(bmp.readTemperature());
    Serial.print("Pressure:        ");
    Serial.println(bmp.readPressure() / 100.0F);



void loop() {
    Serial.println("-----------------------------------------");
    Serial.print("Test cycle [");
    Serial.print(counter);
    Serial.println("]");

    Test();
    counter++;
    delay(20000);
