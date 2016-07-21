#!/usr/bin/python
# -*- coding: utf-8 -*-

from time import sleep
import serial
ser = serial.Serial('/dev/ttyACM0', 9600)
while True:
    while True:
        print ser.readline()
    num = raw_input('Number:')
    if num == '1':
        ser.write('1')
    if num == '2':
        ser.write('2')
    if num == '3':
        ser.write('3')
