/* Network connectivity test tool for the ESP32 microcontroller boards.
 * This tool is made to see that basic functionality and boards work. 
 * 
 * Developed by Aleksi Jokinen 2021
 * --------------------------------------------------------------------
 *  CIRCUIT WIRING
 *  OUTPUT (HX711 => ESP32) 
 *  Vcc pin  => 3.3V 
 *  GND pin  => GND 
 *  SCK pin  => GPIO 25 (D25)
 *  DOUT pin => GPIO 26 (D26) 
 * --------------------------------------------------------------------
*/

#include <HX711.h>
#include "soc/rtc.h"

// HX711 DT and SCK definitions. 
#define DOUT_PIN 26
#define SCK_PIN 25

// Init HX711 sensor/module
HX711 scale;
void setup(){
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M); // Adjust ESP32 clock rate frequency to the timing of the HX711 (80MHz).
  scale.begin(DOUT_PIN, SCK_PIN);
}

void loop(){
  if (scale.is_ready()){
    long reading = scale.read();
    Serial.print("HX711 sensor reading : ");
    Serial.println(reading);
  } 
  else 
  {
  Serial.println("HX711 sensor not found.");
  }
  delay(1000);
}