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
<title>IBedroom Lighting Controll</title>
</head>

<body>
<h2 align="center">
Main Lights - 
</h2>

    <?php
    if (isset($_POST['advance'])){
	    exec('1.py');
	}
    ?>
    <form action="" method="post" align="center">
    <button type="submit" name="advance" id="submit">Advance</button>
    </form>
    <h2 align="center">
    Iains Lights - 
    
    <?php
    if (isset($_POST['iainslights'])){
	    exec('2.py');
	}
    ?>
    <form action="" method="post" align="center">
    <button type="submit" name="iainslights" id="iainslights">Iains Lights</button>
    </form>
    <h2 align="center">
    Eloras Lights - 
    
    <?php
    if (isset($_POST['eloraslights'])){
	    exec('3,py');
	}
    ?>
    <form action="" method="post" align="center">
    <button type="submit" name="eloraslights" id="eloraslights">Eloras Lights</button>
    </form>

</div>
</body>

</html>
"""
