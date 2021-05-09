## Project for tracking motorhome gas tank levels using ESP32 microcontroller and HX711 Amp with load cells.  

Values from load cells are amplified using HX711 amplifier connected to ESP32.  
Webserver is created locally inside ESP32.    
ESP32 is connected to motorhome's internal network wirelessly.  

User interface is accessed using browser and navigating to 192.168.0.68/ which is static IP address 
assigned to the ESP32 microcontroller. 
 
 **HX711 Specifications** 

    Operation Voltage: 2.7V--5V
    Operation Current: < 1.5mA
    Selectable 10SPS or 80SPS output data rate
    Simultaneous 50 and 60Hz supply rejection
   
 **Load cell Specifications** 

    Type: Strain gauge
    Accuracy: ±0.05% (50kg x 0.0005 = 0.025kg -> ±25g)
    Wires: Three wires (Red, white, green)
    Maximum weight: 50.00kg (per cell, 200.00kg total)
    Wiring: Can be wired to Amp using 1, 2, or 4 cell configuration
            Quarter Wheatstone bridge = 1 cell
              Half Wheatstone bridge = 2 cells
              Full Wheatstone bridge = 4 cells
    
**Links:**  
[Circuitjournal tutorial](https://circuitjournal.com/50kg-load-cells-with-HX711)  
[Schematic by adamfk](https://easyeda.com/adamfk/hx711-module)  
[HX711 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf)
