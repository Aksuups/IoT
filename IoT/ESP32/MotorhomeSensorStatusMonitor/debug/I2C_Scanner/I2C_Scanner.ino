#include <Wire.h>

#define I2C_Freq 100000

//BMP280 SDA, SCL 
#define SDA_0 21
#define SCL_0 22
//BME280 SDA, SCL
//#define SDA_1 39
//#define SCL_1 36
 
TwoWire I2C_0 = TwoWire(0);
//TwoWire I2C_1 = TwoWire(1);

int counter = 0;

void setup(){
    Serial.begin(9600); // The baudrate of Serial monitor is set in 9600

    //Initialize I2C interfaces.
    I2C_0.begin(SDA_0 , SCL_0 , I2C_Freq );
    //I2C_1.begin(SDA_1 , SCL_1 , I2C_Freq );

    //Wire.begin(); // Wire communication begin
    while (!Serial); // Waiting for Serial Monitor
    Serial.println("\nI2C Scanner");
}

void I2C_0_Scan(){
    byte error, address; //variable for error and I2C address
    int nDevices;

    Serial.print("[");
    Serial.print(counter);
    Serial.print("]");
    Serial.println(" Scanning I2C_0 interface...");
    counter++;

    nDevices = 0;
    for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    I2C_0.beginTransmission(address);
    error = I2C_0.endTransmission();

    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error == 4){
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}

/*void I2C_1_Scan(){
    byte error, address; //variable for error and I2C address
    int nDevices;

    Serial.print("[");
    Serial.print(counter);
    Serial.print("]");
    Serial.println(" Scanning I2C_1 interface...");
    counter++;

    nDevices = 0;
    for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    I2C_1.beginTransmission(address);
    error = I2C_1.endTransmission();

    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error == 4){
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}*/

void loop(){
    delay(5000);
    I2C_0_Scan();
    //I2C_1_Scan();

}