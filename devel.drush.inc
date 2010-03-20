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
  $items['fn-hook'] = array(
    'description' => 'List implementations of a given hook and explore source of specified one.',
    'arguments' => array(
      'hook' => 'The name of the hook to explore.'
    ),
    'aliases' => array('fnh', 'hook'),
  );
  $items['fn-view'] = array(
    'description' => 'Show the source of specified function.',
    'arguments' => array(
      'function' => 'The name of the function to view.',
    ),
    'options' => array(
      '--pipe' => 'Output just the filename of the function',
    ),
    'examples' => array(
      'drush fn-view book_perm' => 'View the function "book_perm"',
      'vi `drush --pipe fn-view user_access`' => 'Edit the file that contains the function "user_access"',
    ),
    'aliases' => array('fnv'),
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

/**
 * Command handler. Show hook implementations
 */
function drush_devel_fn_hook($hook) {
  $hook_implementations = module_implements($hook);
  if ($choice = drush_choice(array_combine($hook_implementations, $hook_implementations), 'Enter the number of the hook implementation you wish to view.')) {
    return drush_devel_fn_view($choice . "_$hook");
  }
}

/**
 * Command handler.  Show source code of specified function.
 */
function drush_devel_fn_view($function_name) {
  $func = new ReflectionFunction($function_name);
  $func_info = array('!file' => $func->getFileName(), '!startline' => $func->getStartLine(), '!endline' => $func->getEndLine());
  //drush_print_pipe(dt("!file -line !startline", $func_info));
  drush_print_pipe($func->getFileName());
  drush_print(dt("// file: !file, lines !startline-!endline", $func_info));

  _drush_devel_print_function($func->getFileName(), $func->getStartLine(), $func->getEndLine());
}

/**
 * Print the specified function, including any
 * doxygen-style comments that come before it.
 */
function _drush_devel_print_function($file, $start_line, $end_line) {
  $line_num = 0;
  $doxygen = NULL;
  $fp = fopen( $file, 'r' );

  while (!feof($fp) && ($line_num < ($start_line - 1))) {
    $line = fgets($fp);
    ++$line_num;

    if (substr($line,0,3) == '/**') {
      $doxygen = $line;
    }
    elseif (isset($doxygen)) {
      $doxygen .= $line;
      if ($line_num + 1 == $start_line) {
        drush_print(rtrim($doxygen));
      }
      if (strstr($line, '*/') !== FALSE) {
        $doxygen = NULL;
      }
    }
  }
  while (!feof($fp) && ($line_num < $end_line)) {
    $line = fgets($fp);
    ++$line_num;
    drush_print(rtrim($line));
  }
}
