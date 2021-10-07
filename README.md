Developer Tools (devel)
=======================

This module contains helper functions for Backdrop developers. It provides a
wide array of tools for examining the functionality of your site, including:

- Displaying a list of all SQL queries executed per page.
- Integrating with XHProf to show what PHP was executed and identify slow calls.
- Executing arbitrary PHP.
- Switch between different users.
- Report memory usage at the bottom of each page.
- A mail-system class which redirects outbound email to files.
- Utility functions for dumping variables to the page or messages area.

Although the abilities of this module are restricted to a permission
('access development information'), it's a good idea not to have this module
enabled on production servers due to its exceedingly dangerous functionality.

The most common functions for dumping variables include:

- `dpr()`: Prints a variable using `print_r()` to the messages area.
- `dpm()`: Prints a variable using the krumo library to the messages area.
- `dvm()`: Prints a variable using `var_export()` to the messages area.

All of these functions will only show the messages to users that have the
'access development information' permission.

Included in this package is also:

- devel_node_access module which prints out the node_access records for a given node. Also offers hook_node_access_explain for all node access modules to implement. Handy.
- devel_generate.module which bulk creates nodes, users, comment, terms for development.

Requirements
------------

- none

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://docs.backdropcms.org/documentation/extend-with-modules.

- Additional settings are available under Administration > Development
  (admin/devel).

Documentation
-------------

Additional documentation is located in the Wiki:
https://github.com/backdrop-contrib/devel/wiki/Documentation.

Issues
------

Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/devel/issues.

Current Maintainers
-------------------

- [Nate Haug](https://github.com/quicksketch)
- [Alan Mels](https://github.com/alanmels)
- [Jen Lampton](https://github.com/jenlampton)
- Seeking additional maintainers.

Credits
-------

- Originally written for Drupal by [Moshe Weitzman](https://www.drupal.org/u/moshe-weitzman)
- Originally written for Drupal by [Hans Salvisberg](https://www.drupal.org/u/salvis)

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.
