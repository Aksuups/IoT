/* Tester for the NodeMCU microcontroller.
 * This tool is made to see that basic functionality and boards work. 
 * 
 * Developed by Aleksi Jokinen 2021
*/    
int n = 0;
     
void setup() {
//Delays added to properly setup the program.
Serial.begin(9600);  //Baud rate needs to be 9600 or NodeMCU doesn't work correctly.
Serial.println("ESP8266, NodeMCU test");
Serial.println("Starting to iterate n(1)");
Serial.println("n, integer, maximum value 32,767");
Serial.println("Running at 9600bps");
Serial.println("Loop delay 100ms");
}

//Iteration tester. 
void loop() {
  if(n < 32768) {
    n++;
    Serial.println(n);
  }
  else {
    Serial.println("Iteration test finished with 32,767 cycles");
    Serial.println("Test passed.");
    return;
  }
  delay(100);
}
