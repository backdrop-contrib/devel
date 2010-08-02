<?php
// $Id$

/**
 * @file
 *  Generate content, taxonomy, and users via drush framework.
 */

/**
 * Implementation of hook_drush_help().
 */
function devel_generate_drush_help($section) {
  switch ($section) {
    case 'drush:generate-users':
      return dt('Usage: drush generate-users <number_users>. Specify --kill if you want to delete the users first.');
    case 'drush:generate-taxonomy':
      return dt('Usage: drush generate-taxonomy <number_vocab> <number_terms>. Specify --kill if you want to delete all taxonomy first.');
    case 'drush:generate-content':
      return dt('Usage: drush generate-content <number_nodes> <number_comments>. Specify --kill if you want to delete all content first.');
    case 'drush:generate-menus':
      return dt('Usage: drush generate-menus <number_menus> <number_links> <max_depth> <max_width>. Specify --kill if you want to delete all menus and links first.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function devel_generate_drush_command() {
  $items['generate-users'] = array(
    'description' => 'Creates users.',
    'arguments' => array(
      'number_users' => 'Number of users to generate.',
    ),
    'options' => array(
      'kill' => 'Specify \'kill\' to delete all users before generating new ones.',
      'roles' => 'A comma delimited list of role IDs which should be granted to the new users. No need to specify authenticated user role.',
    ),
    'aliases' => array('genu'),
  );
  $items['generate-taxonomy'] = array(
    'description' => 'Creates taxonomy.',
    'arguments' => array(
      'n' => 'Vocabulary ID into which new terms will be inserted.',
    ),
    'options' => array(
      'kill' => 'Specify \'kill\' to delete all terms in specified vocab before generating.',
      'feedback' => 'An integer representing interval for insertion rate logging. Defaults to 500',
    ),
    'aliases' => array('gent'),

  );
  $items['generate-content'] = array(
    'description' => 'Creates content.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN, // Various D7 code assumes we have a uid.
    'drupal dependencies' => array('devel_generate'),
    'arguments' => array(
      'number_nodes' => 'Number of nodes to generate.',
      'maximum_comments' => 'Maximum number of comments to generate.',
    ),
    'options' => array(
      'kill' => 'Specify \'kill\' to delete all content before generating new content.',
      'types' => 'A comma delimited list of content types to create. Defaults to page,article.',
      'feedback' => 'An integer representing interval for insertion rate logging. Defaults to 500',
      'skip-fields' => 'A comma delimited list of fields to omit when generating random values',
      'languages' => 'A comma-separated list of language codes',
    ),
    'aliases' => array('genc'),
  );
  $items['generate-menus'] = array(
    'description' => 'Creates menus and menu items.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN, // Various D7 code assumes we have a uid.
    'drupal dependencies' => array('devel_generate'),
    'arguments' => array(
      'number_menus' => 'Number of menus to generate. Defaults to 2.',
      'number_links' => 'Number of links to generate. Defaults to 50.',
      'max_depth' => 'Max link depth. Defaults to 3',
      'max_width' => 'Max width of first level of links. Defaults to 8.',
    ),
    'options' => array(
      'kill' => 'Specify \'kill\' to delete all previously generated menus and links before generating new menus and links.',
    ),
    'aliases' => array('genm'),
  );
  return $items;
}


/**
 * Command callback. Generate a number of users.
 */
function drush_devel_generate_users($num_users = NULL) {
  if (drush_generate_is_number($num_users) == FALSE) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of users.'));
  }
  drush_generate_include_devel();
  $roles = drush_get_option('roles') ? explode(',', drush_get_option('roles')) : array();
  devel_create_users($num_users, drush_get_option('kill'), 0, $roles);
  drush_log(t('Generated @number users.', array('@number' => $num_users)), 'success');
}

/**
 * Command callback. Generate a number of taxonomy.
 */
function drush_devel_generate_taxonomy($num_vocab = NULL, $num_terms = NULL) {
  if (drush_generate_is_number($num_vocab) == FALSE) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid vocabulary ID.'));
  }
  if (drush_generate_is_number($num_terms) == FALSE) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of terms.'));
  }
  drush_generate_include_devel();
  devel_generate_taxonomy_data($num_vocab, $num_terms, '12', drush_get_option('kill'));
  drush_log(t('Generated @num_vocab vocabulary, @num_terms terms', array('@num_vocab' => $num_vocab, '@num_terms' => $num_terms)), 'success');
}

/**
 * Command callback. Generate a number of content.
 */
function drush_devel_generate_content($num_nodes = NULL, $max_comments = NULL) {
  if (drush_generate_is_number($num_nodes) == FALSE) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of nodes'));
  }
  if (!empty($max_comments) && drush_generate_is_number($max_comments) == FALSE) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of comments.'));
  }

  $add_language = drush_get_option('languages');
  if (!empty($add_language)) {
    $add_language = explode(',', str_replace(' ', '', $add_language));
    // Intersect with the enabled languages to make sure the language args 
    // passed are actually enabled.
    $values['values']['add_language'] = array_flip(array_intersect($add_language, array_keys(locale_language_list())));
  }

  // Let's load user 1, seems to be needed for creating comments.
  global $user;
  $user_one = user_load(1);
  $user = $user_one;
  drupal_save_session(FALSE);

  if (drush_get_option('kill')) {
    $values['values']['kill_content'] = 1;
  }
  $values['values']['title_length'] = '8';
  $values['values']['num_nodes'] = $num_nodes;
  $values['values']['max_comments'] = $max_comments;
  $values['values']['node_types'] = drupal_map_assoc(explode(',', drush_get_option('types', 'page,article')));
  drush_generate_include_devel();
  devel_generate_content($values);
  drush_log(t('Generated @num_nodes nodes, @max_comments comments (or less) per node.', array('@num_nodes' => (int)$num_nodes, '@max_comments' => (int)$max_comments)), 'success');
}

/**
 * Command callback. Generate a number of menus and menu links.
 */
function drush_devel_generate_menus($number_menus = 2, $number_links = 50, $max_depth = 3, $max_width = 8) {
  if (drush_generate_is_number($number_menus) == FALSE) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of menus'));
  }
  if (drush_generate_is_number($number_links) == FALSE) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of links'));
  }
  if (drush_generate_is_number($max_depth) == FALSE || $max_depth > 9 || $max_depth < 1) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid maximum link depth. Use a value between 1 and 9'));
  }  
  if (drush_generate_is_number($max_width) == FALSE || $max_width < 1) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid maximum menu width. Use a positive numeric value.'));
  }

  global $user;
  $user_one = user_load(1);
  $user = $user_one;
  drupal_save_session(FALSE);

  $kill = drush_get_option('kill');
  drush_generate_include_devel();
  $link_types = drupal_map_assoc(array('node', 'front', 'external'));
  devel_generate_menu_data($number_menus, array(), $number_links, 12, $link_types, $max_depth, $max_width, $kill);
  drush_log(t('Generated @number_menus menus, @number_links links.', array('@number_menus' => (int)$number_menus, '@number_links' => (int)$number_links)), 'success');
}
//////////////////////////////////////////////////////////////////////////////
// Helper functions

// Verify if param is a number.
function drush_generate_is_number($number) {
  if ($number == NULL) return FALSE;
  if (!is_numeric($number)) return FALSE;
  return TRUE;
}

// Include devel_generate.inc.
function drush_generate_include_devel() {
  $path = drupal_get_path('module', 'devel_generate');
  require_once($path .'/devel_generate.inc');
}
