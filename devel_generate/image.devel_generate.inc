<?php
// $Id$

function image_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_image_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _image_devel_generate($object, $field, $instance, $bundle);
  }
}

function _image_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();

  if ($path = devel_generate_file($instance)) {
    $source->uri = $path;
    $destination = $field['settings']['uri_scheme'] . '://' . $instance['settings']['file_directory'] . '/' . basename($path);
    $file = file_move($source, $destination);

    $object_field['fid'] = $file->fid;
    $object_field['alt'] = devel_create_greeking(4);
    $object_field['title'] = devel_create_greeking(10);
  }

  return $object_field;
}
