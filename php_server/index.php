<?php
define("DIRECT", true);
require_once "init.php";
ob_start();
if(!isset($_GET["add"]) && !isset($_GET["r"])) {?>So you found this script. No worries. Front is closed. You need to use an API ;)<?php }
if(isset($_GET["r"])){
	$location = retriveUrl($_GET["r"]);
	header("HTTP/1.1 301 Moved Permanently");
	header("Location: ".$location);
}
if(isset($_GET["add"])){
	if(!isset($_GET["key"])){
		echo "you need specify API key!";
		die;
	}
	if(!isset($_GET["url"])){
		echo "you need specify url!";
		die;
	}
	if(validateAPIkey($_GET["key"]))
		echo getScriptBaseLocation()."?r=".addURL($_GET["url"]);
	else
		echo "Your key is invalid or it reached its limit";
}