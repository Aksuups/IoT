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

Adafruit_BME280 bme;
Adafruit_BMP280 bmp;

int counter = 1;

void setup(){
  Serial.begin(9600);
  Serial.println("BME280 and BMP280 sensor testing tool.\n");
  Serial.println("Software version: 1.0");
  Serial.println("Developed by Aleksi Jokinen © 2021\n");

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
    delay(20000);
}
