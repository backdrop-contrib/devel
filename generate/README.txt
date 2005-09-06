GENERAL
===========================
To use these scripts, copy them to the root of your Drupal site.
When you are done, delete them since they are not access controlled.

import-taxonomy-terms.php
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


update-teaser.php
=================

Use this script to regenerate the teasers in the node table.

import-users.php
==================

A small script that reads users from a csv formatted file and puts them into your database.
