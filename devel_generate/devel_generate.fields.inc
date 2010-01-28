<?php
// $Id$

/**
 * @file
 * Functions needed for devel_generate Fields API integration.
 */

/**
 * Enrich the $object that is about to be saved with arbitrary
 * information in each of its fields.
 **/
function devel_generate_fields(&$object, $obj_type, $bundle) {
  $field_types = field_info_field_types();
  $instances = field_info_instances($obj_type, $bundle);
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

      // Include any support file that might exist for this field.
      if (in_array($module, array('file', 'image', 'taxonomy', 'number', 'text', 'userreference', 'nodereference', 'comment'))) {
        // devel_generate implements on behalf of core and special friends.
        module_load_include('inc', 'devel_generate', "$module.devel_generate");
      }
      else {
        module_load_include('inc', $module, "$module.devel_generate");
      }
      $function = $module . '_devel_generate';
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
    // TODO: Completely overriding any existing $object->{$field['field_name']}
    // is necessary here because the forum module has a bug where it
    // initializes the property with incorrect data.
    // @see http://drupal.org/node/652176
    $object->{$field['field_name']} = array(
      LANGUAGE_NONE => $object_field,
    );
  }
}

/**
 * A simple function to return multiple values for fields that use
 * custom multiple value widgets but don't need any other special multiple
 * values handling. This will call the field generation function
 * a random number of times and compile the results into a node array.
 */
function devel_generate_multiple($function, $object, $field, $instance, $bundle) {
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

/**
 * Generate an image based on the properties of a field.
 *
 * Inspect the minimum and maximum image sizes and makes sure the generated
 * image matches the requirements.
 *
 * @return
 *   The path to the new file, in the temporary directory.
 */
function devel_generate_file($instance) {
  if (empty($instance['settings']['file_extensions'])) {
    $instance['settings']['file_extensions'] = 'png jpg jpeg txt';
  }

  $extensions = array_intersect(explode(' ', $instance['settings']['file_extensions']), array('png', 'jpg', 'jpeg', 'txt'));
  $extension = array_rand(drupal_map_assoc($extensions));

  if ($extension == 'txt') {
    $filesize = empty($instance['settings']['max_filesize']) ? 1024 : parse_size($instance['settings']['max_filesize']);
    return devel_generate_textfile($filesize);
  }
  elseif (in_array($extension, array('png', 'jpg', 'jpeg')) && function_exists('imagecreate')) {
    $min_resolution = empty($instance['settings']['min_resolution']) ? '100x100' : $instance['settings']['min_resolution'];
    $max_resolution = empty($instance['settings']['max_resolution']) ? '600x600' : $instance['settings']['max_resolution'];
    return devel_generate_image($extension, $min_resolution, $max_resolution);
  }

  return FALSE;
}

/**
 * Private function for generating a random text file.
 */
function devel_generate_textfile($filesize = 1024) {
  static $filesizes = array();

  $temp_file = FALSE;
  if (isset($filesizes[$filesize])) {
    $temp_file = $filesizes[$filesize];
  }
  elseif ($tmp_file = drupal_tempnam('temporary://', 'filefield_')) {
    $destination = $tmp_file . '.txt';
    file_unmanaged_move($tmp_file, $destination);

    $fp = fopen($destination, 'w');
    fwrite($fp, str_repeat('01', $filesize/2));
    fclose($fp);
    $filesizes[$filesize] = $destination;
  }

  return $destination;
}

/**
 * Private function for creating a random image.
 *
 * This function only works with the GD toolkit. ImageMagick is not supported.
 */
function devel_generate_image($extension = 'png', $min_resolution, $max_resolution) {
  static $images = array();

  // Generate a max of 5 different images.
  if (!isset($images[$extension][$min_resolution][$max_resolution]) || count($images[$extension][$min_resolution][$max_resolution]) < 5) {
    if ($tmp_file = drupal_tempnam('temporary://', 'imagefield_')) {
      $destination = $tmp_file . '.' . $extension;
      file_unmanaged_move($tmp_file, $destination);

      $min = explode('x', $min_resolution);
      $max = explode('x', $max_resolution);

      $width = rand((int)$min[0], (int)$max[0]);
      $height = rand((int)$min[0], (int)$max[0]);

      // Make a image split into 4 sections with random colors.
      $im = imagecreate($width, $height);
      for ($n = 0; $n < 4; $n++) {
        $color = imagecolorallocate($im, rand(0, 255), rand(0, 255), rand(0, 255));
        $x = $width/2 * ($n % 2);
        $y = $height/2 * (int) ($n >= 2);
        imagefilledrectangle($im, $x, $y, $x + $width/2, $y + $height/2, $color);
      }

      // Make a perfect circle in the image middle.
      $color = imagecolorallocate($im, rand(0, 255), rand(0, 255), rand(0, 255));
      $smaller_dimension = min($width, $height);
      $smaller_dimension = ($smaller_dimension % 2) ? $smaller_dimension : $smaller_dimension;
      imageellipse($im, $width/2, $height/2, $smaller_dimension, $smaller_dimension, $color);

      $save_function = 'image'. ($extension == 'jpg' ? 'jpeg' : $extension);
      $save_function($im, drupal_realpath($destination));

      $images[$extension][$min_resolution][$max_resolution][$destination] = $destination;
    }
  }
  // Select one of the images we've already generated for this field.
  else {
    $destination = array_rand($images[$extension][$min_resolution][$max_resolution]);
  }
  return $destination;
}
