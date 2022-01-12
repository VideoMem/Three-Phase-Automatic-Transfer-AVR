import serial
import sys

ser = serial.Serial('/dev/ttyUSB0', 9600)
'''
for the XY-WTH1 Temperature / humidity controller
commands:
start
    starts data logging out mode
stop
    ends data logging
T:ON
    enables temperature relay
T:OFF
    disables temperature relay
H:ON
    enables humidity relay
H:OFF
    disables humidity relay
TP:50.0
    set stop (relay off) temperature at 50.0 degree celsius (range -20.0~60.0)           
HP:99.9
    set stop (relay off) humidity at 99.9 percent (range 0.0~99.9)           
read
    show parameter settings
'''
while True:
    cmd = input(":")
    ser.write(cmd.encode())
    while ser.in_waiting:
        read = ser.readline()
        safe = read.replace(bytes.fromhex('a1e6'), "º".encode())
        print(safe.decode(), end='')
        # print(read.decode(), end='')

