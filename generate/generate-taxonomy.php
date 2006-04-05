<?php
// $Id$
include_once "includes/bootstrap.inc";
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
// If not in 'safe mode', increase the maximum execution time:
if (!ini_get('safe_mode')) {
  set_time_limit(240);
}

function create_vocabularies($records) {
  $types = array("story", "blog", "forum", "page");

  // Insert new data:
  for ($i = 1; $i <= $records; $i++) {
    $voc = array();
    $voc['name'] = "vocabulary #$i";
    $voc['description'] = "description of vocabulary #$i";
    $voc['nodes'] = array_flip(array($types[array_rand($types)]));
    foreach ($voc['nodes'] as $key => $value) {
      $voc['nodes'][$key] = $key;
    }
    $voc['multiple'] = 1;
    $voc['required'] = 0;
    $voc['relations'] = 1;
    $voc['hierarchy'] = 1;
    $voc['weight'] = rand(0,10);    

    taxonomy_save_vocabulary($voc);
    $output .= "created vocabulary #$i<br />";
  }
  return $output;
}

function create_terms($records, $vocs) {

  // Insert new data:
  for ($i = 1; $i <= $records; $i++) {

    switch ($i % 2) {
      case 1:
        $term['vid'] = $vocs[array_rand($vocs)];
        // dont set a parent. handled by taxonomy_save_term()
        // $term->parent = 0;
        break;
      case 2:
      default:
        $parent = db_fetch_object(db_query_range("SELECT t.tid, v.vid FROM {term_data} t INNER JOIN {vocabulary} v ON t.vid = v.vid ORDER BY RAND()", 0, 1));
        $term['parent'] = array($parent->tid);
        $term['vid'] = $parent->vid;
        break;
    }
 
    $term['name'] = "term #$i";
    $term['description'] = "description of term #$i";
    $term['weight'] = rand(0,10);
   
    $status = taxonomy_save_term($term);
    unset($term);
    
    $output .= $status. ": #$i<br />";
  }
  return $output;
}

function get_vocabularies() {
  $vocs = array();
  $result = db_query("SELECT vid FROM {vocabulary}");
  while($voc = db_fetch_object($result)){
    $vocs[] = $voc->vid;
  }
  return $vocs;
}


db_query("DELETE FROM {term_data}");
db_query("DELETE FROM {term_node}");
db_query("DELETE FROM {term_hierarchy}");
db_query("DELETE FROM {term_relation}");
db_query("DELETE FROM {term_synonym}");
db_query("DELETE FROM {vocabulary}");
db_query("DELETE FROM {vocabulary_node_types}");
db_query("UPDATE {sequences} SET id = '0' WHERE name = '{vocabulary_vid}'");
db_query("UPDATE {sequences} SET id = '0' WHERE name = '{term_data_tid}'");

$output = create_vocabularies(15);
$vocs = get_vocabularies();
$output .= create_terms(50, $vocs);
print theme('page', $output);
drupal_page_footer();

?>
