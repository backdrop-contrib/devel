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
    case 'drush:generate users':
      return dt('Usage: drush generate users <number_users>. Specify --kill if you want to delete the users first.');
    case 'drush:generate taxonomy':
      return dt('Usage: drush generate tax <number_vocab> <number_terms>. Specify --kill if you want to delete all taxonomy first.');
    case 'drush:generate content':
      return dt('Usage: drush generate content <number_nodes> <number_comments>. Specify --kill if you want to delete all content first.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function devel_generate_drush_command() {
  $items['generate users'] = array(
    'callback' => 'drush_generate_users',
    'description' => 'Creates users.',
    'arguments' => array(
      'number_users' => 'Number of users to generate.',
    ),
    'options' => array(
      'kill' => 'Specify \'kill\' to delete all users before generating new ones.',
      'roles' => 'A comma delimited list of role IDs which should be granted to the new users. No need to specify authenticated user role.',
    ),
  );
  $items['generate taxonomy'] = array(
    'callback' => 'drush_generate_tax',
    'description' => 'Creates taxonomy.',
    'arguments' => array(
      'n' => 'Vocabulary ID into which new terms will be inserted.',
    ),
    'options' => array(
      'kill' => 'Specify \'kill\' to delete all terms in specified vocab before generating.'
    ),
    
  );
  $items['generate content'] = array(
    'callback' => 'drush_generate_nodescontent',
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
      'add-terms' => 'Add terms to generated nodes.',
      'add-upload' => 'Add an attached file to generated nodes.',
    ),
  );
  return $items;
}


/**
 * Command callback. Generate a number of users.
 */
function drush_generate_users($num_users = NULL) {
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
function drush_generate_tax($num_vocab = NULL, $num_terms = NULL) {
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
function drush_generate_nodescontent($num_nodes = NULL, $max_comments = NULL) {
  if (drush_generate_is_number($num_nodes) == FALSE) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of nodes'));
  }
  if (!empty($max_comments) && drush_generate_is_number($max_comments) == FALSE) {
    drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of comments.'));
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
  $values['values']['add_terms'] = drush_get_option('add-terms', FALSE);
  $values['values']['add_upload'] = drush_get_option('add-upload', FALSE);
  $values['values']['node_types'] = drupal_map_assoc(explode(',', drush_get_option('types', 'page,article')));
  drush_generate_include_devel();
  devel_generate_content($values);
  drush_log(t('Generated @num_nodes nodes, @max_comments comments per node', array('@num_nodes' => (int)$num_nodes, '@max_comments' => (int)$max_comments)), 'success');
}

//////////////////////////////////////////////////////////////////////////////
// Helper functions

// verify if param is a number
function drush_generate_is_number($number) {
  if ($number == NULL) return FALSE;
  if (!is_numeric($number)) return FALSE;
  return TRUE;
}

// include devel_generate.inc
function drush_generate_include_devel() {
  $path = drupal_get_path('module', 'devel_generate');
  require_once($path .'/devel_generate.inc');
}