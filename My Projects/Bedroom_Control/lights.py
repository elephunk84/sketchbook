#!/bin/python
# -*- coding: utf-8 -*-

from time import sleep
import serial
import os
from flask import Flask, render_template

ser = serial.Serial('/dev/ttyACM0', 9600)

app = Flask(__name__)

@app.route('/')
def home():
   return render_template("index.html")
   
@app.route('/hello')
def hello_world():
   return 'hello world'

if __name__ == '__main__':
   app.static_folder = 'static'
   app.run('0.0.0.0', '8080')
