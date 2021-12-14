<?php

/**
 * @file
 * Functions for Devel administrative pages.
 */

/**
 * Form constructor for the settings form.
 *
 * @ingroup forms
 */

function devel_admin_settings() {
  $config = config('devel.settings');

  $form['redirect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Redirect Page'),
    '#collapsible' => FALSE,
  );
  $form['redirect']['redirect_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display redirection page'),
    '#default_value' => $config->get('redirect_page'),
    '#description' => t('When a module executes backdrop_goto(), the query log and other developer information is lost. Enabling this setting presents an intermediate page to developers so that the log can be examined before continuing to the destination page.'),
  );

  $form['theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theming tools'),
    '#collapsible' => FALSE,
  );
  $form['theme']['theme_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable theme debug'),
    '#default_value' => config_get('system.core', 'theme_debug'),
    '#description' => t('Adds HTML comments containing information about the template files used to render each part of the page. Useful for finding template suggestions when building a custom theme.'),
  );
  $form['theme']['rebuild_theme_registry'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild the theme registry on every page load'),
    '#description' => t('While creating new templates and theme_ overrides the theme registry needs to be rebuilt.'),
    '#default_value' => $config->get('rebuild_theme_registry'),
  );

  $form['errors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Error handling'),
    '#collapsible' => FALSE,
  );

  $enabled_levels = config_get('system.core', 'watchdog_enabled_severity_levels');
  if (is_array($enabled_levels)) {
    $form['errors']['watchdog_enabled_severity_levels'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Enabled watchdog severity levels'),
      '#options' => watchdog_severity_levels(),
      '#default_value' => $enabled_levels,
      '#description' => t('Only messages with the enabled severity levels will be logged. The "debug" and "deprecated" levels are recommended for developer environments, but not on production sites.'),
    );
  }

  $error_handlers = devel_get_handlers();
  $form['errors']['devel_error_handlers'] = array(
    '#type' => 'select',
    '#title' => t('Error handlers'),
    '#options' => array(
      DEVEL_ERROR_HANDLER_NONE => t('None'),
      DEVEL_ERROR_HANDLER_STANDARD => t('Standard Backdrop'),
      DEVEL_ERROR_HANDLER_BACKTRACE_DPM => t('Krumo backtrace in the message area'),
      DEVEL_ERROR_HANDLER_BACKTRACE_KRUMO => t('Krumo backtrace above the rendered page'),
    ),
    '#multiple' => TRUE,
    '#default_value' => empty($error_handlers) ? DEVEL_ERROR_HANDLER_STANDARD : $error_handlers,
    '#description' => t('Select the error handler(s) to use, in case you <a href="@choose">choose to show errors on screen</a>.', array('@choose' => url('admin/config/development/logging'))) . '<ul>' .
                      '<li>' . t('<em>None</em> is a good option when stepping through the site in your debugger.') . '</li>' .
                      '<li>' . t('<em>Standard Backdrop</em> does not display all the information that is often needed to resolve an issue.') . '</li>' .
                      '<li>' . t('<em>Krumo backtrace</em> displays nice debug information when any type of error is noticed, but only to users with the %perm permission.', array('%perm' => t('Access developer information'))) . '</li></ul>' .
                      t('Depending on the situation, the theme, the size of the call stack and the arguments, etc., some handlers may not display their messages, or display them on the subsequent page. Select <em>Standard Backdrop</em> <strong>and</strong> <em>Krumo backtrace</em> above the rendered page to maximize your chances of not missing any messages.') . '<br />' .
                      t('Demonstrate the current error handler(s):') . ' ' . l('notice+warning', $_GET['q'], array('query' => array('demo' => 'warning'))) . ', ' . l('notice+warning+error', $_GET['q'], array('query' => array('demo' => 'error'))) . ' ' . t('(The presentation of the @error is determined by PHP.)', array('@error' => 'error')),
  );
  $form['errors']['devel_error_handlers']['#size'] = count($form['errors']['devel_error_handlers']['#options']);
  if (empty($_POST) && !empty($_GET['demo'])) {
    devel_demonstrate_error_handlers($_GET['demo']);
  }

  $form['misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Miscellaneous'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['misc']['permission_raw_names'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display machine names of permissions and modules'),
    '#default_value' => $config->get('permission_raw_names'),
    '#description' => t('Display the language-independent machine names of the permissions in mouse-over hints on the !Permissions page and the module base file names on the @Permissions and !Modules pages.', array('!Permissions' => l(t('Permissions'), 'admin/people/permissions'), '@Permissions' => t('Permissions'), '!Modules' => l(t('Modules'), 'admin/modules'))),
  );
  $form['misc']['api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API Site'),
    '#default_value' => $config->get('api_url'),
    '#description' => t('The base URL for your developer documentation links. You might change this if you run <a href="!url">api.module</a> locally.', array('!url' => url('http://drupal.org/project/api')))
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit handler for devel_admin_settings().
 */
function devel_admin_settings_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Save the hidden core development settings
  $config = config('system.core');
  $config->set('theme_debug', (bool) $values['theme_debug']);
  $config->set('watchdog_enabled_severity_levels', array_map('intval', array_values(array_filter($values['watchdog_enabled_severity_levels'], 'is_string'))));

  $config->save();

  // And now our own devel.settings.
  $config = config('devel.settings');

  // Save value of system variable 'theme_debug'
  config_set('system.core', 'theme_debug', (bool) $values['theme_debug']);

  $config->set('api_url', $values['api_url']);

  $config->set('redirect_page', (bool) $values['redirect_page']);
  $config->set('permission_raw_names', (bool) $values['permission_raw_names']);
  $config->set('devel_error_handlers', array_values(array_filter($values['devel_error_handlers'])));
  $config->set('rebuild_theme_registry', (bool) $values['rebuild_theme_registry']);

  $config->save();

  backdrop_set_message('The configuration options have been saved.');
}

/**
 * Demonstrates the active error handler.
 *
 * Generates some warnings or errors, so that the reaction of the current error
 * handler can be observed.
 *
 * @param $severity
 *   The severity level that the error handler should demonstrate.
 */
function devel_demonstrate_error_handlers($severity) {
  switch ($severity) {
    case 'warning':
      trigger_error('Notice', E_USER_NOTICE);
      trigger_error('Warning', E_USER_WARNING);
      break;
    case 'error':
      trigger_error('Notice', E_USER_NOTICE);
      trigger_error('Warning', E_USER_WARNING);
      trigger_error('Error', E_USER_ERROR);
      break;
  }
}
