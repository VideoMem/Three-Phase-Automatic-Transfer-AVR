# -*- coding: utf-8 -*-
import serial
import sys
from manchester import manchester

ser = serial.Serial('/dev/ttyUSB0', 9600)
man = manchester()

while True:
    read = ser.read(2)
    a, b = read
    translate = man.Rtable(ord(a), ord(b))
    if translate > 0:
        sys.stdout.write(chr(translate))
    sys.stdout.flush

