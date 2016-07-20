<html>

<head>
<meta http-equiv="refresh" content="10">
<link rel="stylesheet" href="/resources/css/style.css">
<link href='https://fonts.googleapis.com/css?family=Open+Sans' rel='stylesheet' type='text/css'>
<title>IBedroom Lighting Controll</title>
</head>

<body>
<h2 align="center">
Manual Override is - 

    <?php
    if (filesize("resources/status") <= 1 )
    {
    echo "OFF";
    }
    echo file_get_contents( "./resources/status" );
    ?>

</h2>

    <?php
    if (isset($_POST['advance'])){
	    exec('sh ./webadvance.sh');
	}
    ?>
    <form action="" method="post" align="center">
    <button type="submit" name="advance" id="submit">Advance</button>
    </form>
    <h2 align="center">
    Summer Mode is - 
    
    <?php
    echo file_get_contents( "./resources/summer" );
    if (isset($_POST['summer'])){
	    exec('sh ./summermode.sh');
	}
    ?>
    <form action="" method="post" align="center">
    <button type="submit" name="summer" id="summer">Summer Mode</button>
    </form>

</div>
</body>

</html>
