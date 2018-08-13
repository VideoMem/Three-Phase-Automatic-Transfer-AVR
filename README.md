# Three-Phase-Automatic-Transfer-AVR

An Arduino 328p firmware for two phase power supply automatic transfer control.

It features generator start/stop and automatic transfer when it warms. 

Also selects single phase input from the best voltage value of three phase power.

It provides with basic serial console for setup, and manchester line coding support for cheap wireless 433MHz RF modules.

## Prerequisites
Install the required AVR+Arduino stack, you also will need Python 2.7 for the console.

### Upload firmware
Connect nano trough usb serial port and execute

$ make upload

### Console

$ python console/console.py

