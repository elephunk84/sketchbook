#!/usr/bin/python
# -*- coding: utf-8 -*-

from time import sleep
import serial
ser = serial.Serial('/dev/ttyACM0', 9600)
num = raw_input('Number:')
while True:
    if num == '1':
        ser.write('1')
    if num == '2':
        ser.write('2')
    if num == '3':
        ser.write('3')
