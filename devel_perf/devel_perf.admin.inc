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
function devel_perf_settings_form($form, &$form_state) {
  $config = config('devel_perf.settings');
  $form['#config'] = 'devel_perf.settings';

  $form['queries'] = array(
    '#type' => 'fieldset',
    '#title' => t('Query logging'),
  );

  $description = t('Display a log of the database queries needed to generate the current page, and the execution time for each. Also, queries which are repeated during a single page view are summed in the # column, and printed in red since they are candidates for caching.');
  $form['queries']['query_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display query log'),
    '#default_value' => $config->get('query_display', 0),
    '#description' => $description,
  );
  $form['queries']['settings'] = array(
    '#type' => 'container',
      '#states' => array(
        // Hide the query log settings when not displaying query log.
        'invisible' => array(
          'input[name="query_display"]' => array('checked' => FALSE),
        ),
      ),
  );
  $form['queries']['settings']['show_query_args_first'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show query arguments'),
    '#default_value' => $config->get('show_query_args_first', FALSE),
    '#description' => t('Show arguments rather than the query placeholders in the logged query.'),
  );
  $form['queries']['settings']['query_sort'] = array(
    '#type' => 'radios',
    '#title' => t('Sort query log'),
    '#default_value' => $config->get('query_sort', DEVEL_PERF_QUERY_SORT_BY_SOURCE),
    '#options' => array(t('by source'), t('by duration')),
    '#description' => t('The query table can be sorted in the order that the queries were executed or by descending duration.'),
  );
  $form['queries']['settings']['query_slow_limit'] = array('#type' => 'textfield',
    '#title' => t('Slow query highlighting'),
    '#default_value' => $config->get('query_slow_limit'),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Enter an integer in milliseconds. Any query which takes longer than this many milliseconds will be highlighted in the query log. This indicates a possibly inefficient query, or a candidate for caching.'),
  );

  $form['performance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Performance logging'),
    '#collapsible' => FALSE,
  );
  $form['performance']['timer_display'] = array('#type' => 'checkbox',
    '#title' => t('Display page timer'),
    '#default_value' => $config->get('timer_display'),
    '#description' => t('Display page execution time in the query log box.'),
  );
  $form['performance']['memory_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display memory usage'),
    '#default_value' => $config->get('memory_display'),
    '#description' => t('Display how much memory is used to generate the current page. This will show memory usage when devel_init() is called and when devel_exit() is called.'),
  );

  return system_settings_form($form);
}
