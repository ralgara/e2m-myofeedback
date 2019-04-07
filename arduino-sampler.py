#!/usr/bin/env python3

#pip3 install pyserial
import serial
import sys
import os
import time

#/dev/cu.usbmodem14301'
dev = os.environ['E2MWEAR_ARDUINO_DEV']
baud = os.environ['E2MWEAR_ARDUINO_BAUD']
print({"dev": dev, "baud": baud, "ts": time.time()})
with serial.Serial(dev, baud, timeout=5) as s:
    for line in s:
        payload  = line.decode('utf-8').rstrip()
        try:
           adc = int(payload)
           sample = {
               "ts": time.time(),
               "adc": {
                   0: adc
               }
           }
           print(sample)
           sys.stdout.flush()
        except ValueError:
           pass # print only parsed ints
