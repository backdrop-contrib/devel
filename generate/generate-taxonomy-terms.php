<?php

// written by Moshe Weitzman - weitzman at tejasa.com - July 2003

// BEWARE! this script adds data to your database

// -------------------------
// CONFIGURATION

    // Change the value below to TRUE when you want to run the script After running, immediately
    // change back to FALSE in order to prevent accidentally executing this script twice.
    $active = FALSE;
    
    // Enter the vocabulary ID into which you want to insert terms
    $vid = 88;
    
    // create an array of term names. order doesn't
    // matter since Drupal will present them alphabetically
    $terms = array (
      "Alabama","Alaska","Arizona","Arkansas","California","Colorado","Connecticut","Delaware","Florida","Georgia","Hawaii","Idaho","Illinois","Indiana","Iowa","Kansas","Kentucky","Louisiana","Maine","Maryland","Massachusetts","Michigan","Minnesota","Mississippi","Missouri","Montana","Nebraska","Nevada","New Hampshire","New Jersey","New Mexico","New York","North Carolina","North Dakota","Ohio","Oklahoma","Oregon","Pennsylvania","Rhode Island","South Carolina","South Dakota","Tennessee","Texas","Utah","Vermont","Virginia","Washington","West Virginia","Wisconsin","Wyoming","Washington D.C."
    );
// -------------------------
// CODE

    include_once "includes/bootstrap.inc";
    include_once("includes/common.inc");
    
    if ($active) {
      if (user_access("administer taxonomy")) {
        foreach ($terms as $term) {
          $edit = array ("vid" => $vid, "name" => $term );
          $msg = taxonomy_save_term($edit);
          print $msg;
        }
      }
      else {
        print "You have insufficent permission to  administer taxonomy";
      }  
    }
    else {
      print "You have not activated term_loader. See $active variable at top of the source code";
    }

?>
