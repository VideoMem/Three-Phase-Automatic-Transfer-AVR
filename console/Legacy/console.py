import serial
import sys

ser = serial.Serial('/dev/ttyUSB0', 9600)
while True:
    sys.stdout.write(ser.readline())
    sys.stdout.flush

