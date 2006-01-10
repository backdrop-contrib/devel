<?php
  /**
   * This script generates users for testing purposes.
   */
include_once 'includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);


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
    db_query("INSERT INTO {users} (uid, name, mail, status, created, access) VALUES (%d, '%s', '%s', %d, %d, %d)", $uid, $name, $mail, $status, time(), time());
  }
  db_query("UPDATE {sequences} SET id = %d WHERE name = 'users_uid'", $uid);
}