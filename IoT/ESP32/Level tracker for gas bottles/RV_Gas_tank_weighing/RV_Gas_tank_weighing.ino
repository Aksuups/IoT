#include "HX711.h"

//Circuit wiring 
const int LOADCELL_DOUT_PIN = 7;
const int LOADCELL_SCK_PIN = 6;

//Init HX711 sensor/module
HX711 scale;
void setup(){
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
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
