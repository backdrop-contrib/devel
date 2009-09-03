<?php
// $Id$

/**
 * @file
 * Functions needed for Devel module integration.
 */

/**
 * Enrich the $object that is about to be saved with arbitrary
 * information in each of its fields.
 **/
function cck_generate_fields(&$object, $bundle) {
  $field_types = field_info_field_types();
  $instances = field_info_instances($bundle);
  foreach ($instances as $instance) {
    $field_name = $instance['field_name'];
    $table_name = _field_sql_storage_tablename($field_name);
    $revision_name = _field_sql_storage_revision_tablename($field_name);
    $field = field_read_field($field_name);

    $object_field = array();
    // If module handles own multiples, then only call its hook once.
    if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
      $max = 0;
    }
    else {
      switch ($field['cardinality']) {
        case FIELD_CARDINALITY_UNLIMITED:
          $max = rand(0, 3); //just an arbitrary number for 'unlimited'
          break;
        default:
          $max = $field['cardinality'] - 1;
          break;
      }
    }
    for ($i = 0; $i <= $max; $i++) {
      $module = $field_types[$field['type']]['module'];
      $function = $module .'_cck_generate';
      if (function_exists($function)) {
        $result = $function($object, $field, $instance, $bundle); // $items, $teaser, $page
        if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
          // Fields that handle their own multiples will add their own deltas.
          $object_field = $result;
        }
        else {
          // When multiples are handled by the content module, add a delta for each result.
          $object_field[$i] = $result;
        }
      }
    }
    // TODO Come back later and see if we need 
    // a valid language code or if 'none' is OK.
    $object->{$field['field_name']}[FIELD_LANGUAGE_NONE] = $object_field;
  }
}

/**
 * A simple function to return multiple values for fields that use
 * custom multiple value widgets but don't need any other special multiple
 * values handling. This will call the field generation function
 * a random number of times and compile the results into a node array.
 */
function cck_devel_multiple($function, $object, $field, $instance, $bundle) {
  $object_field = array();
  if (function_exists($function)) {
    switch ($field['cardinality']) {
      case FIELD_CARDINALITY_UNLIMITED:
        $max = rand(0, 3); //just an arbitrary number for 'unlimited'
        break;
      default:
        $max = $field['cardinality'] - 1;
        break;
    }
    for ($i = 0; $i <= $max; $i++) {
      $object_field[$i] = $function($object, $field, $instance, $bundle);
    }
  }
  return $object_field;
}

if (module_exists('text')) {
  function text_cck_generate($object, $field, $instance, $bundle) {
    if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
      return cck_devel_multiple('_text_cck_generate', $object, $field, $instance, $bundle);
    }
    else {
      return _text_cck_generate($object, $field, $instance, $bundle);
    }
  }

  function _text_cck_generate($object, $field, $instance, $bundle) {
    $object_field = array();
    $format = isset($field['text_processing']) ? rand(0, 3) : 0;
    if ($instance['widget']['type'] != 'text_textfield') {
      $object_field['value'] = devel_create_content($format);
      if ($instance['widget']['type'] == 'text_textarea_with_summary' && !empty($instance['display_summary'])) {
        $object_field['summary'] = devel_create_content($format);
      }
    }
    else {
      // Generate a value that respects max_length.
      if (empty($field['max_length'])) {
        $field['max_length'] = 12;
      }
      $object_field['value'] = user_password($field['max_length']);
    }
    $object_field['format'] = $format;
    return $object_field;
  }
}

if (module_exists('number')) {
  function number_cck_generate($object, $field, $instance, $bundle) {
    if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
      return cck_devel_multiple('_number_cck_generate', $object, $field, $instance, $bundle);
    }
    else {
      return _number_cck_generate($object, $field, $instance, $bundle);
    }
  }

  function _number_cck_generate($object, $field, $instance, $bundle) {
    $object_field = array();
    // Make sure the field settings are all set.
    foreach (array('min', 'max', 'precision', 'scale') as $key) {
      if (empty($field[$key])) {
        $field[$key] = NULL;
      }
    }
    $min = is_numeric($field['min']) ? $field['min'] : 0;
    switch ($field['type']) {
      case 'number_integer':
        $max = is_numeric($field['max']) ? $field['max'] : 10000;
        $decimal = 0;
        $scale = 0;
        break;

      case 'number_decimal':
        $precision = is_numeric($field['precision']) ? $field['precision'] : 10;
        $scale = is_numeric($field['scale']) ? $field['scale'] : 2;
        $max = is_numeric($field['max']) ? $field['max'] : pow(10, ($precision - $scale));
        $decimal = rand(0, (10 * $scale)) / 100;
        break;

      case 'number_float':
        $precision = rand(10, 32);
        $scale = rand(0, 2);
        $decimal = rand(0, (10 * $scale)) / 100;
        $max = is_numeric($field['max']) ? $field['max'] : pow(10, ($precision - $scale));
        break;
    }
    $object_field['value'] = round((rand($min, $max) + $decimal), $scale);
    return $object_field;
  }
}

if (module_exists('nodereference')) {
  function nodereference_cck_generate($object, $field, $instance, $bundle) {
    if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
      return cck_devel_multiple('_nodereference_cck_generate', $object, $field, $instance, $bundle);
    }
    else {
      return _nodereference_cck_generate($object, $field, $instance, $bundle);
    }
  }

  function _nodereference_cck_generate($object, $field, $instance, $bundle) {
    $object_field = array();
    $allowed_values = nodereference_allowed_values($field);
    unset($allowed_values[0]);
    if (!empty($allowed_values)) {
      // Just pick one of the specified allowed values.
      $object_field['nid'] = array_rand($allowed_values);
    }
    return $object_field;
  }
}

if (module_exists('userreference')) {
  function userreference_cck_generate($object, $field) {
    if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
      return cck_devel_multiple('_userreference_cck_generate', $object, $field);
    }
    else {
      return _userreference_cck_generate($object, $field);
    }
  }

  function _userreference_cck_generate($object, $field) {
    $object_field = array();
    $allowed_values = userreference_allowed_values($field);
    if (isset($allowed_values['none'])) {
      unset($allowed_values['none']);
    }
    if (!empty($allowed_values)) {
      // Just pick one of the specified allowed values.
      $object_field['uid'] = array_rand($allowed_values);
    }
    return $object_field;
  }
}