#!/bin/python
# -*- coding: utf-8 -*-

from time import sleep
import serial
import os
from bottle import route, run, template

ser = serial.Serial('/dev/ttyACM0', 9600)

index_html = '''<html>

<head>
<link rel="stylesheet" href="style.css">
<link href='https://fonts.googleapis.com/css?family=Open+Sans' rel='stylesheet' type='text/css'>
<title>IBedroom Lighting Controll</title>
</head>

<body>
<h2 align="center">
Main Lights - 
</h2>

    <?php
    if (isset($_POST['advance'])){
	    exec('/usr/bin/python /var/www/html/1.py');
	}
    ?>
    <form action="" method="post" align="center">
    <button type="submit" name="advance" id="submit">Advance</button>
    </form>
    <h2 align="center">
    Iains Lights - 
    
    <?php
    if (isset($_POST['iainslights'])){
	    exec('/usr/bin/python /var/www/html/2.py');
	}
    ?>
    <form action="" method="post" align="center">
    <button type="submit" name="iainslights" id="iainslights">Iains Lights</button>
    </form>
    <h2 align="center">
    Eloras Lights - 
    
    <?php
    if (isset($_POST['eloraslights'])){
	    exec('/usr/bin/python /var/www/html/3.py');
	}
    ?>
    <form action="" method="post" align="center">
    <button type="submit" name="eloraslights" id="eloraslights">Eloras Lights</button>
    </form>

</div>
</body>

</html>
.'''


@route('/')
def index():
    return template(index_html, author='Real Python')


@route('/name/<name>')
def name(name):
    return template(index_html, author=name)


if __name__ == '__main__':
    port = int(os.environ.get('PORT', 8080))
    run(host='192.168.0.131', port=port, debug=True)
