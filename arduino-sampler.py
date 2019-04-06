#!/usr/bin/env python3

#pip3 install pyserial
import serial
import sys

with serial.Serial('/dev/cu.usbmodem14401', 19200, timeout=5) as s:
    for line in s:
        payload  = line.decode('utf-8').rstrip()
        try:
           adc = int(payload)
           print(adc)
           sys.stdout.flush()
        except ValueError:
           pass # print only parsed ints
