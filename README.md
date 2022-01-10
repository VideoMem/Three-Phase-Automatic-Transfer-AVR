# Three-Phase-Automatic-Transfer-AVR

An Arduino 328p firmware for two phase power supply automatic transfer control.

It features generator start/stop and automatic transfer when it warms. 

Also selects single phase input from the best voltage value of three phase power.

It provides with basic serial console for setup, and manchester line coding support for minimal/remote control wireless 433MHz RF modules.

## Prerequisites
Install the required AVR+Arduino stack, you also will need Python 3 for the console plus the serial module.

On Ubuntu 20.04:
```
    sudo apt install arduino-core arduino-mk python3 python3-pip
    sudo pip3 install pyserial
``` 

### Upload firmware
Connect nano trough usb serial port and execute

```
make upload
```

### Console

```
python3 console/console.py
```
