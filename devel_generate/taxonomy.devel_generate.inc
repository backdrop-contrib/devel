<?php
//$Id$

function taxonomy_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_taxonomy_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _taxonomy_devel_generate($object, $field, $instance, $bundle);
  }
}

function _taxonomy_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();
  // TODO: For free tagging vocabularies that do not already have terms, this
  // will not result in any tags being added.
  $allowed_values = taxonomy_allowed_values($field);
  if (!empty($allowed_values)) {
    // Pick one of the allowed values.
    $object_field['tid'] = array_rand($allowed_values);
  }
  else {
    $object_field['tid'] = 0;
  }
  return $object_field;
}
