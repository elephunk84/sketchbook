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
    form = ContactForm()
    if form.validate_on_submit():
        if 'advance' in request.form:
            ser.write('1')
        elif 'iainslights' in request.form:
            ser.write('2')
        elif 'eloraslights' in request.form:
            ser.write('3')
        else:
            pass 
    elif request.method == 'GET':
        return render_template("index.html", form=form)

@app.route('/hello')
def hello_world():
   return 'hello world'

if __name__ == '__main__':
   app.static_folder = 'static'
   app.run('0.0.0.0', '8080')
