#include <Wire.h> //include Wire.h library

int counter = 0;

void setup(){
    Serial.begin(9600); // The baudrate of Serial monitor is set in 9600

    Wire.begin(); // Wire communication begin
    while (!Serial); // Waiting for Serial Monitor
    Serial.println("\nI2C Scanner");
}

void I2C_Scan(){
    byte error, address; //variable for error and I2C address
    int nDevices;

    Serial.print("[");
    Serial.print(counter);
    Serial.print("]");
    Serial.println(" Scanning...");
    counter++;

    nDevices = 0;
    for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

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

void loop(){
    delay(5000);
    I2C_Scan();
}