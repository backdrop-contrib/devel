<?php
// $Id$
include_once "includes/bootstrap.inc";
include_once "includes/common.inc";

function create_vocabularies($records) {
  $types = array("story", "blog", "forum", "page");

  // Insert new data:
  for ($i = 1; $i <= $records; $i++) {
    $voc->name = "vocabulary #$i";
    $voc->description = "description of vocabulary #$i";
    $voc->nodes = array($types[array_rand($types)]);
    $voc->multiple = 1;
    $voc->required = 0;
    $voc->relations = 1;
    $voc->hierarchy = 1;
    $voc->weight = rand(0,10);    

    taxonomy_save_vocabulary(object2array($voc));
    print "created vocabulary #$i<br />";
  }
}

function create_terms($records, $vocs) {

  // Insert new data:
  for ($i = 1; $i <= $records; $i++) {

    switch ($i % 2) {
      case 1:
        $term->vid = $vocs[array_rand($vocs)];
        $term->parent = 0;
        break;
      case 2:
      default:
        $parent = db_fetch_object(db_query("SELECT t.tid, v.vid FROM {term_data} t, {vocabulary} v WHERE v.vid = t.vid ORDER BY RAND() LIMIT 1"));
        $term->parent = $parent->tid;
        $term->vid = $parent->vid;
        break;
    }
 
    $term->name = "term #$i";
    $term->description = "description of term #$i";
    $term->weight = rand(0,10);
   
    taxonomy_save_term(object2array($term)); 
    
    print "created term #$i<br />";
  }
}

function get_vocabularies() {
  $vocs = array();
  $result = db_query("SELECT vid FROM vocabulary");
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
db_query("UPDATE sequences SET id = '0' WHERE name = 'vocabulary_vid'");
db_query("UPDATE sequences SET id = '0' WHERE name = 'term_data_tid'");

create_vocabularies(15);
$vocs = get_vocabularies();
create_terms(50, $vocs);

?>
