README.txt
==========

A module containing helper functions for Drupal developers and
inquisitive admins. Initially, this module prints out a summary of
all database queries for each page request at the bottom of each page. The
summary includes how many times each query was executed on a page
(shouldn't run same query multiple times), and how long each query
 took (short is good - use cache for complex queries).

Also a dprint_r($array) function is provided, which pretty prints arrays. Useful during
development.

Also prints stack trace and profileing info when the xdebug extension is active.
See http://www.xdebug.org/index.php

-moshe weitzman
weitzman at tejasa.com

NOTE
====

The subdirectory generate/ includes some scripts that can be used to help testing
on a Drupal site. See generate/README.txt for details.
