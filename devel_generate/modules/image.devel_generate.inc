<?php
/**
 * @file
 * Devel generate support for Image module.
 */

define('DEVEL_GENERATE_IMAGE_MAX', 5);

function image_devel_generate($object, $field, $instance, $bundle) {
  if (function_exists('imagejpeg')) {
    $devel_generate_image_function = '_image_devel_generate';
    if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
      return devel_generate_multiple($devel_generate_image_function, $object, $field, $instance, $bundle);
    }
    else {
      return $devel_generate_image_function($object, $field, $instance, $bundle);
    }
  }
}

function _image_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();
  static $images = array();

  $min_size = empty($instance['settings']['min_size']) ? '100x100' : $instance['settings']['min_size'];
  $max_size = empty($instance['settings']['max_resolution']) ? '600x600' : $instance['settings']['max_resolution'];
  $extensions = array_intersect(explode(' ', $instance['settings']['file_extensions']), array('png', 'jpg'));
  $extension = array_rand(backdrop_map_assoc($extensions));

  // Generate a max of 5 different images.
  if (!isset($images[$extension][$min_size][$max_size]) || count($images[$extension][$min_size][$max_size]) <= DEVEL_GENERATE_IMAGE_MAX) {
    if ($path = devel_generate_image($extension, $min_size, $max_size)) {
      $destination_dir = $field['settings']['uri_scheme'] . '://' . $instance['settings']['file_directory'];
      file_prepare_directory($destination_dir, FILE_CREATE_DIRECTORY);
      if ($uri = file_unmanaged_move($path, $destination_dir)) {
        $file = new File();
        $file->fid = NULL;
        $file->uri = $uri;
        $file->filename = backdrop_basename($uri);
        $file->filemime = file_get_mimetype($file->uri);
        // @todo Randomize file owner.
        $file->uid = 1;
        file_save($file);
        $images[$extension][$min_size][$max_size][$file->fid] = $file;
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
  else {
    // Select one of the images we've already generated for this field.
    $file = new File();
    $file->fid = array_rand($images[$extension][$min_size][$max_size]);
  }

  $object_field['fid'] = $file->fid;
  $object_field['alt'] = devel_create_greeking(4);
  $object_field['title'] = devel_create_greeking(4);
  return $object_field;
}

/**
 * Private function for creating a random image.
 *
 * This function only works with the GD toolkit. ImageMagick is not supported.
 */
function devel_generate_image($extension = 'png', $min_size = array(), $max_size = array()) {
  if ($tmp_file = backdrop_tempnam('temporary://', 'imagefield_')) {
    $destination = $tmp_file . '.' . $extension;
    file_unmanaged_move($tmp_file, $destination, FILE_CREATE_DIRECTORY);

    $min = explode('x', $min_size);
    $max = explode('x', $max_size);

    $width = rand((int)$min[0], (int)$max[0]);
    $height = rand((int)$min[1], (int)$max[1]);

    $half_width = (int)($width/2);
    $half_height = (int)($height/2);

    // Make an image split into 4 sections with random colors.
    $im = imagecreate($width, $height);
    for ($n = 0; $n < 4; $n++) {
      $color = imagecolorallocate($im, rand(0, 255), rand(0, 255), rand(0, 255));
      $x = (int)($half_width * ($n % 2));
      $y = (int)($half_height * (int) ($n >= 2));
      imagefilledrectangle($im, $x, $y, $x + $half_width, $y + $half_height, $color);
    }

    // Make a perfect circle in the image middle.
    $color = imagecolorallocate($im, rand(0, 255), rand(0, 255), rand(0, 255));
    $smaller_dimension = min($width, $height);
    $smaller_dimension = (int)(($smaller_dimension % 2) ? $smaller_dimension : $smaller_dimension);
    imageellipse($im, $half_width, $half_height, $smaller_dimension, $smaller_dimension, $color);

    $save_function = 'image'. ($extension == 'jpg' ? 'jpeg' : $extension);
    $save_function($im, backdrop_realpath($destination));

    $images[$extension][$min_size][$max_size][$destination] = $destination;
  }
  return $destination;
}
