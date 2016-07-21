#!/bin/python
# -*- coding: utf-8 -*-

from time import sleep
import serial
import os
from bottle import route, run, template

ser = serial.Serial('/dev/ttyACM0', 9600)

@app.route('/')
def index():
    """Home page"""
    info = {'title': 'Welcome Home!',
        'content': 'Hello World'
    }
    return template('index_html.tpl', info)
    
if __name__ == '__main__':
    port = int(os.environ.get('PORT', 8080))
    run(host='192.168.0.131', port=port, debug=True)
