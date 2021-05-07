    /* Network scanning test tool for the ESP32 microcontroller boards.
     * This tool is made to see that basic functionality and boards work. 
     * 
     * Developed by Aleksi Jokinen 2021
     */
    #include "WiFi.h"
     
    void setup() {
      Serial.begin(115200);
     
      // Set WiFi to station mode and disconnect from an AP if it was previously connected
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      delay(100);
     
      Serial.println("Setup done");
    }
     
    void loop() {
      Serial.println("Scanning available networks...");
      Serial.println("");
      Serial.println("");
      
      int n = WiFi.scanNetworks(); //Return the number of networks found.
      Serial.println("Scanning completed.");
      if (n == 0)
        Serial.println("No networks found."); //If n = 0, Return with the 
      else
      {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("");
        for (int i = 0; i < n; ++i)
        {
          // Print SSID and RSSI for each network found
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.print(WiFi.SSID(i));
          Serial.print(" (");
          Serial.print(WiFi.RSSI(i));
          Serial.print(")");
          Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
          delay(10);
        }
      }
      Serial.println("");
      delay(5000);
    }
