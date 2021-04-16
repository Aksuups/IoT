This is a program made for RV gas tank weighing. 

Plans for the solution is to use HX711 module and/or pressure plates (kinf of)
to be put physically under the gas tanks, to calculate remaining gas. Code needs to be adjusted 
for the different weight of every gas tank, to measure actual amount of gas. (Accurately or within +- of 
reasonable difference)

ESP32 is the microcontroller chosen for this project, since it has capability of using either Wifi or 
bluetooth as a communication protocol. 

At the moment project is leaning more towards using wifi. This is due possibility to connect automatically 
to RV's internal wifi network and host a webserver.

Simple UI is provided at given (yet undetermined) IP address which is run by the microprocsessor. 

User can see the status of the gas tanks (amount of gas left per tank) by navigating to given IP address. 

Additionally software should include process for calibrating the sensor. 


