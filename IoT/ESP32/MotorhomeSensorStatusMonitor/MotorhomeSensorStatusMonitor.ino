/* ------------------------------------------------------------------------ *
 *  Motorhome sensor monitor                                                *
 *  Tracks gas left on tanks, ambient temperature, pressure and             *
 *  relative humidity.                                                      *
 *  ----------------------------------------------------------------------- *
 *  Developed by Aleksi Jokinen © 2021                                      *
 *  https://www.github.com/Aksuups                                          *
 * ------------------------------------------------------------------------ */
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HX711.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <BlynkSimpleEsp32.h>
//#include <Nextion.h>

// HX711 DT and SCK definitions. 
#define LOADCELL_DOUT_PIN 5
#define LOADCELL_SCK_PIN 18

#define CALIBRATION_FACTOR 50500.00 // This value needs to be determined. 
#define SEALEVELPRESSURE_HPA (1013.25)

// I2C definitions.
#define I2C_Freq 100000
// BME280 SDA, SCL 
#define SDA_0 21
#define SCL_0 22
// BMP280 SDA, SCL
#define SDA_1 12
#define SCL_1 14
 
TwoWire I2C_0 = TwoWire(0);
TwoWire I2C_1 = TwoWire(1);

// Define global variables.
float gastank1, gastank2, temperature, humidity, pressure, volts, outsideTemperature;
float data1_var = 7.50;
int gastank1_bar, gastank2_bar, tmpVal;
const int tmp36Pin = 36; 

/*  Nextion parameters: 
    page1
    gastank1_val, id14, set- -> b2, set+ -> b3
    gastank2_val, id15, set- -> b4, set+ -> b5
    Nextion RX on ESP32 -> GPIO 1 / TX0
*/
int gastank1_weight, gastank2_weight;


// Init HX711 LC Amplifier.
HX711 scale;

// Init BME280-sensor.
Adafruit_BME280 bme;

// Network and API connection configuration.
const char* auth = "*****";
const char* ssid = "*****";
const char* passwd = "*****";

// Set static IP address for the ESP32.
IPAddress local_ip(192, 168, 100, 68);
IPAddress gateway(192, 168, 100, 1); // Defaults to the device IP address with the last octet set to 1.
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);         // Google's Public DNS, Secondary: 8.8.4.4.

// Initialize connection.
WiFiMulti WiFiMulti;

// Functions to send data from microcontroller to the nextion display. 
void sendGasTank1ToNextion(){
  String command = "gastank1.txt=\""+String(gastank1,2)+"\"";
  Serial.print(command);
  endNextionCommand();
}

void sendGasTank1BarToNextion(){
  String command = "gastank1_bar.val="+String(gastank1_bar);
  Serial.print(command);
  endNextionCommand();
}

void sendGasTank2ToNextion(){
  String command = "gastank2.txt=\""+String(gastank2,2)+"\"";
  Serial.print(command);
  endNextionCommand();
}

void sendGasTank2BarToNextion(){
  String command = "gastank2_bar.val="+String(gastank2_bar);
  Serial.print(command);
  endNextionCommand();
}

void sendHumidityToNextion(){
  String command = "humidity.txt=\""+String(humidity,2)+"\"";
  Serial.print(command);
  endNextionCommand();
}

void sendINSIDETemperatureToNextion(){
  String command = "INtemperature.txt=\""+String(temperature,2)+"\"";
  Serial.print(command);
  endNextionCommand();
}

void sendOUTSIDETemperatureToNextion(){
  String command = "OUTtemperature.txt=\""+String(outsideTemperature,2)+"\"";
  Serial.print(command);
  endNextionCommand();
}

void receiveGastank1_weight(){
  //TODO: Implement
}

void receiveGastank2_weight(){
  //TODO: Implement
}

void endNextionCommand(){
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
} 

void updateBlynk(){
  Blynk.virtualWrite(V1, gastank1);
  Blynk.virtualWrite(V2, gastank2);
  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V4, humidity);
  Blynk.virtualWrite(V5, outsideTemperature);
}

void getData(){

  gastank2 = scale.get_units()*0.453592, 3;
  if (gastank2 < 0.00){
    gastank2 = 0.00;
  }

  gastank1 = data1_var;
  data1_var = data1_var - 0.01;
  gastank1_bar = gastank1 * 9.0909090909090909090909090909091;
  gastank2_bar = gastank2 * 9.0909090909090909090909090909091;
  Serial.print("\nTank 1: ");
  Serial.print(gastank1);
  Serial.println(" kg");
  Serial.print("Tank 2: ");
  Serial.print(gastank2);
  Serial.println(" kg");

    //Calculate temperature for TMP36-sensor
  tmpVal = analogRead(tmp36Pin);
  volts = tmpVal/1024.0;
  outsideTemperature = (volts - 0.5) * 100;
  

  // Calculate data for BME280-sensor.
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  Serial.print("Temperature IN: "); 
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Temperature OUT: "); 
  Serial.print(outsideTemperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.println();
  Serial.print("Gastank1 set weight: ");
  Serial.println();
  Serial.print("Gastank2 set weight: ");
  Serial.println();

  updateBlynk();
  endNextionCommand();
  sendGasTank1ToNextion();
  sendGasTank1BarToNextion();
  sendGasTank2ToNextion();
  sendGasTank2BarToNextion();
  sendINSIDETemperatureToNextion();
  sendOUTSIDETemperatureToNextion();
  sendHumidityToNextion();
  receiveGastank1_weight();
  receiveGastank2_weight();
  Serial.println();
  delay(10000);
}

void setup(){

  // Initialize serial communication using 9600 baud rate (9600 Required for operating Nextion).
  Serial.begin(9600);

  // Initialize connection to BlynkAPI.
  Blynk.begin(auth, ssid, passwd);

  // Initialize I2C interfaces.
  I2C_0.begin(SDA_0 , SCL_0 , I2C_Freq );
  I2C_1.begin(SDA_1 , SCL_1 , I2C_Freq );

  // Initialize scale (HX711, load cells).
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);
  scale.set_offset(11000);

  // Initiliaze BME sensor. Give error if not found in address 76.
  if (!bme.begin(0x76)){ 
    Serial.println("\nBME280-sensor not found.");
    while(1);
    }

  Serial.println("\nMotorhome sensor status monitor");
  Serial.println("Tracks gas left on tanks, ambient temperature,");
  Serial.println("pressure and relative humidity.\n");
  Serial.println("Software version: 1.1 (beta).");
  Serial.println("Developed by Aleksi Jokinen © 2021\n");
  Serial.println("Initialising program....\n");

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
  Serial.println("\nStatus: OK");
  Serial.println("Connection established.");
  Serial.print("Network: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet mask: ");
  Serial.println(WiFi.subnetMask());
}

void loop(){
  Blynk.run();
  getData();
}