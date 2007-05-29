README.txt
==========

A module containing helper functions for Drupal developers and
inquisitive admins. This module can print a log of
all database queries for each page request at the bottom of each page. The
summary includes how many times each query was executed on a page
(shouldn't run same query multiple times), and how long each query
 took (short is good - use cache for complex queries).
 
 It also
 - writes a log of template files which may be used on current page
 - a block for running custom PHP on a pge
 - a block for quickly accessing devel pages
 - a block for masquerading as other users (useful for testing)
 - reports memory usage at bottom of page
 
 This module is safe to use on a production site. Just be sure to only grant
 'access development information' permission to developers.

Also a dpr() function is provided, which pretty prints arrays and strings. Useful during
development. Many other nice functions like dsm(), dvm(), ...

Included in this package is also: 
- devel_node_access module which prints out the node_access records for a given node._
- devel_generate.module which bulk creates nodes, users, comment, terms for development
- macro.module which records form submissions and can pay them back later or on another site. More
information available at http://drupal.org/node/79900.


AUTHOR/MAINTAINER
======================
-moshe weitzman
weitzman at tejasa DOT com
