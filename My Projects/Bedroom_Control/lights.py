#!/bin/python
# -*- coding: utf-8 -*-

from time import sleep
import serial
import os
from bottle import route, run, template

ser = serial.Serial('/dev/ttyACM0', 9600)

@route('/')
def index():
    return template(index)
     
@route('/name/<name>')
def name(name):
    return template(index_html, author=name)


if __name__ == '__main__':
    port = int(os.environ.get('PORT', 8080))
    run(host='192.168.0.131', port=port, debug=True)
