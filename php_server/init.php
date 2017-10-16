<?php defined("DIRECT") OR die("direct access is prohibited.");


require_once "config.php";
require_once "functions.php";


global $pdo;
$pdo = new pdo("mysql:host=" . DATABASE_HOST . ";dbname=" . DATABASE_NAME . ";charset=utf8", DATABASE_NICKNAME, DATABASE_PASSWORD);
$pdo->exec("set names utf8");

