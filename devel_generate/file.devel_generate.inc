<?php
// $Id$

function file_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_file_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _file_devel_generate($object, $field, $instance, $bundle);
  }
}

function _file_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();

  if ($path = devel_generate_file($instance)) {
    $source->uri = $path;
    $destination = $field['settings']['uri_scheme'] . '://' . $instance['settings']['file_directory'] . '/' . basename($path);
    $file = file_move($source, $destination);
  
    $object_field['fid'] = $file->fid;
    $object_field['display'] = $field['settings']['display_default'];
    $object_field['description'] = devel_create_greeking(10);
  }

  return $object_field;
}
