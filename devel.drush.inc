<?php
// $Id$

/**
 * @file
 * Drush integration for the devel module.
 */

/**
 * Implements hook_drush_command().
 */
function devel_drush_command() {
  $items['devel-download'] = array(
    'description' => dt('Downloads the FirePHP library from http://firephp.org/.'),
    'arguments' => array(
      'path' => dt('Optional. A path to the download folder. If omitted Drush will use the default location (sites/all/libraries/firephp).'),
    ),
  );
  $items['devel-reinstall'] = array(
    'description' => dt('Disable, Uninstall, and Install a list of projects.'),
    'arguments' => array(
      'path' => dt('A space separated list of project names.'),
    ),
    'aliases' => array('dre'),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function devel_drush_help($section) {
  switch ($section) {
    case 'drush:devel-reinstall':
      return dt('Disable, Uninstall, and Install a list of projects.');
    case 'drush:devel-download':
      return dt("Downloads the FirePHP library from http://firephp.org/. Places it in the devel module directory. Skips download if library already present. This all happens automatically if you enable devel using drush.");
  }
}


/**
 * A command callback. This is faster than 3 separate bootstraps.
 */
function drush_devel_reinstall() {
  $projects = func_get_args();

  $args = array_merge(array('pm-disable'), $projects);
  call_user_func_array('drush_invoke', $args);

  $args = array_merge(array('pm-uninstall'), $projects);
  call_user_func_array('drush_invoke', $args);

  $args = array_merge(array('pm-enable'), $projects);
  call_user_func_array('drush_invoke', $args);
}

/**
 * A command callback.
 */
function drush_devel_download() {
  $args = func_get_args();
  if (isset($args[0])) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
    if (module_exists('libraries')) {
      $path .= libraries_get_path('FirePHPCore') . '/FirePHPCore';
    }
    else {
      $path .= '/'. drupal_get_path('module', 'devel') . '/FirePHPCore';
    }
  }

  if (is_dir($path)) {
    drush_log('FirePHP already present. No download required.', 'ok');
  }
  elseif (drush_shell_exec('svn checkout http://firephp.googlecode.com/svn/branches/Library-FirePHPCore-0.3 ' . $path)) {
    drush_log(dt('FirePHP has been downloaded to @path.', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download FirePHP to @path.', array('@path' => $path)), 'error');
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_devel_post_enable() {
  $modules = func_get_args();
  if (in_array('devel', $modules)) {
    drush_devel_download();
  }
}
