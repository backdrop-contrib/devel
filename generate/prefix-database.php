<?php
$prefix = "d_";
$file = file("database/database.mysql");

$fp = fopen("database/database-prefixed.mysql", "w");
foreach($file as $line){
  $line = str_replace("CREATE TABLE ", "CREATE TABLE ". $prefix, $line);
  $line = str_replace("INSERT INTO ", "INSERT INTO ". $prefix, $line);
  $line = str_replace("REPLACE ", "REPLACE ". $prefix, $line);
  fwrite($fp, $line);
}
fclose($fp);

?>
