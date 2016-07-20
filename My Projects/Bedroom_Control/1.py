#!/bin/python
# -*- coding: utf-8 -*-

from time import sleep
import serial
ser = serial.Serial('/dev/ttyACM0', 9600)
ser.write('1')

