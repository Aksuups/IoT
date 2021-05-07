    /* Tester for the NodeMCU microcontroller.
     * This tool is made to see that basic functionality and boards work. 
     * 
     * Developed by Aleksi Jokinen 2021
     */    
     int n = 0;
     
     void setup() {
      //Delays added to properly setup the program.
      Serial.begin(9600);
      delay(1000);
      Serial.println("ESP8266, NodeMCU test");
      delay(100);
      Serial.println("Starting to iterate n(1)");
      delay(100);
      Serial.println("n, integer, maximum value 32,767");
      delay(100);
      Serial.println("Running at 9600bps");
      delay(100);
      Serial.println("Loop delay 100ms");
      delay(2000);
     }

     void loop() {
      n++;
      Serial.println(n);
      delay(100); 
     }
