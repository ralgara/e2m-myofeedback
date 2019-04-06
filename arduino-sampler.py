#!/usr/bin/env python3

#pip3 install pyserial
import serial

with serial.Serial('/dev/cu.usbmodem142301', 19200, timeout=5) as s:
    for line in s:
        print(line)