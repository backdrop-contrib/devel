generate-taxonomy-terms.php
===========================

This simple script creates terms out of an array of names which you provide. unlike
the other scripts in this directory, this one is meant for live sites which need to bulk
import data into the term table. See the source code comments for config instructions

generate-taxonomy.php
=====================

With this script you can generate several taxonomy vocabularies and
terms assigned to them.
Put it in the root directory of your Drupal installation, log into your
Drupal as user #1 and then access the script. It will remove the
existing vocabularies and terms and generates 15 vocabularies and 50 terms.
These values can be changed at the bottom of the file.


generate-content.php
====================

The script generate-content.php can be used to generate content to
test a Drupal site.
Put it in the root directory of your Drupal installation, log into your
Drupal as user #1 and then access the script. It will remove the 
existing content and generate 50 nodes and 500 comments. These values can 
be changed at the bottom of the file. If vocabularies and terms exist
terms will get assigned to the new nodes. Therefore you should run 
generate-taxonomy.php before this script.


prefix-database.php
===================

This script can be used to add a prefix to all tables in database.mysql.
To use it, copy it into your drupal root directory, make the database 
directory writable by the www server and access the file through your browser.
You will find a database definitions file with a d_ prefix in the database
directory. You can change the prefix by editing prefix-database.php.

update-teaser.php
=================

Use this script to regenerate the teasers in the node table.

generate-users.php
==================

A small script that reads users from a csv formatted file and puts them into your database.
