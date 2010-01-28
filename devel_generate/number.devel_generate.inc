<?php
// Id$

function number_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_number_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _number_devel_generate($object, $field, $instance, $bundle);
  }
}

function _number_devel_generate($object, $field, $instance, $bundle) {
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
