<?php
// Reads a csv formatted file and creates users.
// $Id$
include "includes/bootstrap.inc";
include "includes/common.inc";

$handle = fopen("daten.csv", "r");
while ($data = fgetcsv ($handle, 1000, ";")) {
  $array = array("name" => $data[0], "pass" => $data[1], "mail" => $data[2], "status" => 1, "rid" => _user_authenticated_id());
  user_save($account, $array);
}
fclose ($handle);

?>
