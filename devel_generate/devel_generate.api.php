<?php
/**
 * @file
 *
 * Documented API for interacting with the Devel Generate submodule.
 */

/**
 * Alter the list of words used to generate text.
 *
 * The existing dictionary array from devel_create_greeking is passed in by
 * reference. It is possible to alter this array or to replace it entirely.
 *
 * $dictionary is a flat array of quoted words.
 */
function hook_devel_generate_dictionary_alter(array &$dictionary) {
  $dictionary = mymodule_custom_dictionary();
}

/**
 * Alter individual words.
 *
 * The existing word string is passed in by reference. It is possible to alter
 * the word or replace it entirely.
 */
function hook_devel_generate_word_alter(&$word) {
  $dictionary = mymodule_custom_dictionary();
  shuffle($dictionary);
  $word = ucfirst($dictionary[array_rand($dictionary)]);
}

/**
 * Helper to define dictionary.
 */
function mymodule_custom_dictionary() {
  $dictionary = array(
    "Backdrop", "CMS", "module", "theme", "layout", "core", "contributed",
    "custom", "PHP", "GitHub", "Drupal", "fork", "appearance", "functionality",
    "admin", "configuration", "blocks", "regions", "sub-theme", "installation",
  );
  return $dictionary;
}
