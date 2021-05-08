Project for tracking the gas bottle levels using load cells.

Plans for the solution is to use HX711 module and/or pressure plates (or similar)
to be put physically under the gas bottles, to calculate remaining gas. Code needs to account
for the small variation between each gas bottle weight, to measure actual amount of gas. (Accurately or within +- of
reasonable difference)

ESP32 is the microcontroller chosen for this project, since it has capability of using either Wifi or
bluetooth as a communication protocol.

At the moment project is leaning more towards using wifi. This is due possibility to connect automatically
to RV's internal wifi network and host a webserver.

Simple UI is provided at given IP address which is run locally on a webserver inside ESP32.

User can see the status of the gas bottles (amount of gas left per bottle), by navigating to given IP address.

Additionally software should include process for calibrating the sensor for accurate measurements.

For connecting the load cells/sensors in various different ways:
https://circuitjournal.com/50kg-load-cells-with-HX711#2x50

Link above contains instructions for connecting 4, 2 and 1 cells to HX711 AMP.
NOTE: These load sensors, have three wires.
