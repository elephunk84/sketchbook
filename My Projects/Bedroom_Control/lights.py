#!/bin/python
# -*- coding: utf-8 -*-

from time import sleep
import serial
import os
from flask import Flask, render_template, request

ser = serial.Serial('/dev/ttyACM0', 9600)

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def home():
    return render_template("index.html")
    
@app.route('/iainslights', methods=['GET', 'POST'])
def iainslights():
    ser.write('3')
    return render_template("index.html")

@app.route('/eloraslights', methods=['GET', 'POST'])
def iainslights():
    ser.write('2')
    return render_template("index.html")

@app.route('/bedlights', methods=['GET', 'POST'])
def iainslights():
    ser.write('1')
    return render_template("index.html")

@app.route('/hello')
def hello_world():
   return 'hello world'

if __name__ == '__main__':
   app.static_folder = 'static'
   app.run('0.0.0.0', '8080')
