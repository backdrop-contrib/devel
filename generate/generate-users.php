<?php
  /**
   * This script generates users for testing purposes.
   */
include_once 'includes/bootstrap.inc';
include_once 'includes/common.inc';

// use one of your own domains. Using somebody else's domain is rude.
$domain = '';

make_users(50, $domain);

function make_users($num, $domain) {
  db_query('DELETE FROM {users} WHERE uid > 1');
  for ($i = 2; $i <= $num; $i++) {
    $uid = $i;
    $name = md5($i);
    $mail = $name .'@'. $domain;
    $status = 1;
    db_query("INSERT INTO {users} (uid, name, mail, status, created, changed) VALUES (%d, '%s', '%s', %d, %d, %d)", $uid, $name, $mail, $status, time(), time());
  }
  db_query("UPDATE {sequences} SET id = %d WHERE name = 'users_uid'", $uid);
}