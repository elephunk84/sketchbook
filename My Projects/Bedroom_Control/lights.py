#!/usr/bin/python
import cgi
print "Content-type: text/html"
print
form = cgi.FieldStorage()
laptops = form.getvalue('laptops','0')
desktops = form.getvalue('desktops','0')
print """
<html>

<head>
<link rel="stylesheet" href="style.css">
<link href='https://fonts.googleapis.com/css?family=Open+Sans' rel='stylesheet' type='text/css'>
<title>Bedroom Lighting Control</title>
</head>

<body>
<h2 align="center">
Main Lights - 
</h2>

    <form action="python ./1.py" method="post" align="center">
    <button type="submit" name="advance" id="submit">Advance</button>
    </form>
    <h2 align="center">
    Iains Lights - 
    
    <form action="./2.py" method="post" align="center">
    <button type="submit" name="iainslights" id="iainslights">Iains Lights</button>
    </form>
    <h2 align="center">
    Eloras Lights - 
    
    <form action="./3.py" method="post" align="center">
    <button type="submit" name="eloraslights" id="eloraslights">Eloras Lights</button>
    </form>

</div>
</body>

</html>
"""
