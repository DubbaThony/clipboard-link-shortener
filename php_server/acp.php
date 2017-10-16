<?php
define("DIRECT", true);
require_once "init.php";
if(!isLoggedIn()){
?>
<html>
<head>
	<title>
		Admin Login Form
	</title>
<style>

</style>
</head>
<body>
please login to the acp:
<form>
	<input name="username">
	<input name="password" type="password">
	<input type="submit">
</form>
</body>
</html>
<?php } else { ?>
<html>
<head>
	<title>
		Admin Login Form
	</title>
<style>

</style>
</head>
<body>
please login to the acp:
<form>
	<input name="username">
	<input name="password" type="password">
	<input type="submit">
</form>
</body>
</html>
<?php }