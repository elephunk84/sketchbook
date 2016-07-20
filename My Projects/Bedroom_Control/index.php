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
    if (isset($_GET['advance'])){
	    exec('/usr/bin/python /var/www/html/1.py');
	}
    ?>
    <form action="" method="get" align="center">
    <button type="submit" name="advance" id="submit">Advance</button>
    </form>
    <h2 align="center">
    Iains Lights - 
    
    <?php
    if (isset($_GET['iainslights'])){
	    exec('/usr/bin/python /var/www/html/2.py');
	}
    ?>
    <form action="" method="get" align="center">
    <button type="submit" name="iainslights" id="iainslights">Iains Lights</button>
    </form>
    <h2 align="center">
    Eloras Lights - 
    
    <?php
    if (isset($_GET['eloraslights'])){
	    exec('/usr/bin/python /var/www/html/3.py');
	}
    ?>
    <form action="" method="get" align="center">
    <button type="submit" name="eloraslights" id="eloraslights">Eloras Lights</button>
    </form>

</div>
</body>

</html>
