#!/bin/python
# -*- coding: utf-8 -*-

from time import sleep
import serial
ser = serial.Serial('/dev/ttyACM0', 9600) # Establish the connection on a specific port
ser.write('2')

