<?php defined("DIRECT") OR die("direct access is prohibited.");

function isLoggedIn():bool
{
	if(isset($_SESSION["auth"]) && $_SESSION["auth"])
		return true;
	else
		return false;
}
function validateAPIkey($key):bool
{
	return true;
	global $pdo;
	$q = $pdo->prepare("SLECT * FROM `apikeys` WHERE `apikey` = ?");
	$q->execute([$key]);
	if($q->rowCount() == 0)
		return false;
	$r = $q->fetch(PDO::FETCH_ASSOC);
	if($r["max"] >= $r["used"])
		return false;
	$q = $pdo->prepare("UPDATE `apikeys` SET `used` = `used` + 1 WHERE  `apikey` = ?");
	$q->execute([$key]);
	return true;
}
function addURL(string $url):string
{
	global $pdo;
	$hash="";
	$url = base64_decode($url);

	while (true){
		$hash = generateRandomID(4);
		$s = $pdo->query("SELECT * FROM `urls` WHERE `shortcut` = '".$hash."';");
		if($s->rowCount() == 0)
			break;
	}
	$q = $pdo->prepare("INSERT INTO `urls` (`shortcut`, `url`) VALUES (?,?)");
	$q->execute([$hash, $url]);
	return $hash;
}
/** @author unknown, got from user "MT." at StackOverflow. Shamelessly copied and modded. refer: https://stackoverflow.com/questions/3990820/how-can-i-make-short-random-unique-keys-like-youtube-video-ids-in-php */
function generateRandomID(int $required_length = 11):string
{
	// http://mohnish.in
    $limit_one = rand();
    $limit_two = rand();
    return substr(uniqid(sha1(crypt(md5(rand(min($limit_one, $limit_two), max($limit_one, $limit_two))), rand()))), 0, $required_length);
}
function getScriptBaseLocation():string
{
	if(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"])
		$http = "https://";
	else
		$http = "http://";
	$out =  $_SERVER['SERVER_NAME'] . dirname($_SERVER['PHP_SELF']);
	if (strpos($out, "http") !== 0)
		$out = $http.$out;
	$lenght = strlen($out);
	if ($out[$lenght-1]=='/')
		return $out;
	else
		return $out."/";
}
function retriveUrl(string $shortcut){
	global $pdo;
	$q= $pdo->prepare("SELECT `url` FROM `urls` WHERE `shortcut` = ?");
	$q->execute([$shortcut]);
	return $q->fetch(PDO::FETCH_NUM)[0];
}