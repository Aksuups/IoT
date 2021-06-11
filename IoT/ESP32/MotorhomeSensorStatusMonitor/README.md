## Project for tracking motorhome gas tank levels using ESP32 microcontroller and HX711 Amp with load cells.

Additionally ESP32 utilizes BME280-sensor to track relative humidity and temperature inside the motorhome, and
TMP36-sensor to track outside temperature.
  
Values from load cells are amplified using HX711 amplifier connected to ESP32.  
~~Webserver is created locally inside ESP32.
ESP32 is connected to motorhome's internal network wirelessly.~~

 **Powering microcontroller and components**
    
    Microcontroller gets power from motorhomes 12V power supply with   
    Texas Instruments LM2596 Step-Down Voltage Regulator in between to   
    regulate voltage presicely. 12V is stepped down to 6-7V 1A to meet  
    power consumption demands, but to also reduce amount of heat dissipated  
    from the microcontroller.  
    
 **Two ways to access UI and display data**  
    
    1) Physical 3.5″ TFT LCD touch screen to display sensor data quickly when inside the motorhome.  
       
    2) Smartphone application with custom dashboard as UI to see data on the go.
       Project will consists the usage of Blynk application and API, to give easiest access 
       to the sensor data when not near the motorhome.

 
 **HX711 Specifications** 

    Operation Voltage: 2.7V--5V
    Operation Current: < 1.5mA
    Selectable 10SPS or 80SPS output data rate
    Simultaneous 50 and 60Hz supply rejection
   
 **Load cell Specifications** 

    Wiring (from manufacturer):
	Red   -> Power  (+) -> HX711 E+  
	Green -> Power  (-) -> HX711 E-
	White -> Output (+) -> HX711 A+
	Black -> Output (-) -> HX711 A-

    ~~Type: Strain gauge
    Accuracy: ±0.05% (50kg x 0.0005 = 0.025kg -> ±25g)
    Wires: Three wires (Red, white, green)
    Maximum weight: 50.00kg (per cell, 200.00kg total)
    Wiring: Can be wired to Amp using 1, 2, or 4 cell configuration
            Quarter Wheatstone bridge = 1 cell
              Half Wheatstone bridge = 2 cells
              Full Wheatstone bridge = 4 cells~~
 
 **BME280 Specifications**
  
    Operation range (full accuracy):
        Pressure: 300...1100 hPa
        Temperature: -40…85°C
        
    Interface: I2C and SPI
    
    Average current consumption (1Hz data refresh rate):
        1.8 μA @ 1 Hz (H, T)
        2.8 μA @ 1 Hz (P, T)
        3.6 μA @ 1 Hz (H, P, T)
      
    Average current consumption in sleep mode: 0.1 μA
    
    Humidity sensor:
        Response time (τ63%): 1 s
        Accuracy tolerance: ± 3 % relative humidity
        Hysteresis: ≤ 2 % relative humidity
        
    Pressure sensor:
        RMS Noise: 0.2 Pa (equiv. to 1.7 cm)
        Sensitivity Error: ± 0.25 % (equiv. to 1 m at 400 m height change)
        Temperature coefficient offset: ±1.5 Pa/K (equiv. to ±12.6 cm at 1°C temperature change)
 
 
**Links:**  
[Circuitjournal loadcell hookup](https://circuitjournal.com/50kg-load-cells-with-HX711)  
[Schematic by adamfk](https://easyeda.com/adamfk/hx711-module)  
[HX711 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf)  
[Display and BME280 sensor guide](https://www.youtube.com/watch?v=eI-4_QyVenw)  
[BME280 technical specifications](https://nettigo.eu/products/module-pressure-humidity-and-temperature-sensor-bosch-bme280)  
[LM2596 technical specifications and datasheet](https://www.ti.com/lit/ds/symlink/lm2596.pdf?ts=1620647119293&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLM2596%253Futm_source%253Dgoogle%2526utm_medium%253Dcpc%2526utm_campaign%253Dapp-null-null-GPN_EN-cpc-pf-google-wwe%2526utm_content%253DLM2596%2526ds_k%253DLM2596%2526DCM%253Dyes%2526gclid%253DCj0KCQjws-OEBhCkARIsAPhOkIatRMRxvpGvEbnHPK70zLzYH-hPvPnoEBu_keLb09jnaif96hzVTssaAlA8EALw_wcB%2526gclsrc%253Daw.ds)   
[Sending and storing values from nextion to arduino](https://seithan.com/nextion-projects/send-values-store-them-on-arduino/)
[Circuitjournal loadcell hookup2](https://circuitjournal.com/four-wire-load-cell-with-HX711)

**Future of the project**  
At this stage, project is still in the development phase, but there is plans to include gps-tracking  
capabilities to see vehicles location in real-time.
