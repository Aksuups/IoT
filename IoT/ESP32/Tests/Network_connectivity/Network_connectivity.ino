/* Network connectivity test tool for the ESP32 microcontroller boards.
 * This tool is made to see that basic functionality and boards work. 
 * 
 * Developed by Aleksi Jokinen 2021
 */

#include "WiFi.h"
 
const char* ssid = "ssid_here";
const char* passwd = "passwd_here";
 
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("\nConnecting to: ");
    Serial.println(ssid);
    delay(5000);
  }
  Serial.println("Connection successful.");
  Serial.print("IP Address:  ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}
 
void loop() {
  //loop zoop
  }
