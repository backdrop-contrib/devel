<?php
// $Id$
include_once "includes/bootstrap.inc";
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
// If not in 'safe mode', increase the maximum execution time:
if (!ini_get('safe_mode')) {
  set_time_limit(240);
}

function create_nodes($records, $users) {

  $possible_types = array("story", "blog", "forum", "page");
  // Only use types that exist.
  $installed_types = array_keys(node_get_types('types'));
  foreach ($possible_types as $type) {
    if (in_array($type, $installed_types)) {
      $types[] = $type;
    }
  }
  $users = array_merge($users, array('0'));

  if (is_array($types)) {
    // Insert new data:
    for ($i = 1; $i <= $records; $i++) {
      $node->uid = $users[array_rand($users)];
      $node->type = $types[array_rand($types)];
      $next_nid = db_result(db_query("SELECT id FROM {sequences} WHERE name = '{node}_nid'"))+1;
      $node->title = "node #$next_nid ($node->type)";
      $node->body = create_content();
      $node->teaser = node_teaser($node->body);
      $node->filter = variable_get('filter_default_format', 1);
      $node->status = 1;
      $node->revision = rand(0,1);
      $node->promote = rand(0, 1);
      $node->comment = 2;
      $node->created = time();
      $node->changed = time();

      // Save the node:
      node_save($node);

      // Setup a path:
      db_query("INSERT INTO {url_alias} (src, dst) VALUES ('%s', '%s')", "node/$node->nid", "node-$node->nid-$node->type");
      print "created node #$node->nid with alias ". url("node-$node->nid-$node->type") ."<br />";
      unset($node);
    }
  }
}

function create_comments($records, $users, $nodes, $comments) {
  $users = array_merge($users, array('0'));
  // Insert new data:
  for ($i = 1; $i <= $records; $i++) {
    $comment = array();

    switch ($i % 3) {
      case 1:
        $result = db_fetch_object(db_query("SELECT nid, cid FROM {comments} WHERE pid = 0 ORDER BY RAND() LIMIT 1"));
        $comment['pid'] = $result->cid;
        $comment['nid'] = $result->nid;
        break;
      case 2:
        $result = db_fetch_object(db_query("SELECT nid, cid FROM {comments} WHERE pid > 0 ORDER BY RAND() LIMIT 1"));
        $comment['pid'] = $result->cid;
	$comment['nid'] = $result->nid;
        break;
      default:
        $comment['nid'] = array_rand($nodes);
        $comment['pid'] = 0;
    }

    $commen['subject'] = "comment #$i";
    $comment['comment'] = "body of comment #$i";
    $comment['uid'] = $users[array_rand($users)];

    if (!$comment['nid']) {
      $comment['nid'] = array_rand($nodes);
      $comment['pid'] = 0;
    }
    comment_save($comment);
    print "created comment #$i<br />";
  }
}

function create_content() {
  $nparas = rand(1,12);
  $type = rand(0,3);

  $output = "";
  switch($type % 3) {
    case 1: // html
      for ($i = 1; $i <= $nparas; $i++) {
        $output .= create_para(rand(10,60),1);
      }
      break;

    case 2: // brs only
      for ($i = 1; $i <= $nparas; $i++) {
        $output .= create_para(rand(10,60),2);
      }
      break;

    default: // plain text
      for ($i = 1; $i <= $nparas; $i++) {
        $output .= create_para(rand(10,60)) ."\n";
      }
  }

  return $output;
}

function create_para($words, $type = 0) {
  $output = "";
  switch ($type) {
    case 1:
      $output .= "<p>";
      $output .= create_greeking($words);
      $output = trim($output) ."</p>";
      break;

    case 2:
      $output .= create_greeking($words);
      $output = trim($output) ."<br />";
      break;

    default:
      $output .= create_greeking($words);
      $output = trim($output);
  }
  return $output;
}

function create_greeking($words) {
  $dictionary = array("abbas", "abdo", "abico", "abigo", "abluo", "accumsan",
    "acsi", "ad", "adipiscing", "aliquam", "aliquip", "amet", "antehabeo",
    "appellatio", "aptent", "at", "augue", "autem", "bene", "blandit",
    "brevitas", "caecus", "camur", "capto", "causa", "cogo", "comis",
    "commodo", "commoveo", "consectetuer", "consequat", "conventio", "cui",
    "damnum", "decet", "defui", "diam", "dignissim", "distineo", "dolor",
    "dolore", "dolus", "duis", "ea", "eligo", "elit", "enim", "erat",
    "eros", "esca", "esse", "et", "eu", "euismod", "eum", "ex", "exerci",
    "exputo", "facilisi", "facilisis", "fere", "feugiat", "gemino",
    "genitus", "gilvus", "gravis", "haero", "hendrerit", "hos", "huic",
    "humo", "iaceo", "ibidem", "ideo", "ille", "illum", "immitto",
    "importunus", "imputo", "in", "incassum", "inhibeo", "interdico",
    "iriure", "iusto", "iustum", "jugis", "jumentum", "jus", "laoreet",
    "lenis", "letalis", "lobortis", "loquor", "lucidus", "luctus", "ludus",
    "luptatum", "macto", "magna", "mauris", "melior", "metuo", "meus",
    "minim", "modo", "molior", "mos", "natu", "neo", "neque", "nibh",
    "nimis", "nisl", "nobis", "nostrud", "nulla", "nunc", "nutus", "obruo",
    "occuro", "odio", "olim", "oppeto", "os", "pagus", "pala", "paratus",
    "patria", "paulatim", "pecus", "persto", "pertineo", "plaga", "pneum",
    "populus", "praemitto", "praesent", "premo", "probo", "proprius",
    "quadrum", "quae", "qui", "quia", "quibus", "quidem", "quidne", "quis",
    "ratis", "refero", "refoveo", "roto", "rusticus", "saepius",
    "sagaciter", "saluto", "scisco", "secundum", "sed", "si", "similis",
    "singularis", "sino", "sit", "sudo", "suscipere", "suscipit", "tamen",
    "tation", "te", "tego", "tincidunt", "torqueo", "tum", "turpis",
    "typicus", "ulciscor", "ullamcorper", "usitas", "ut", "utinam",
    "utrum", "uxor", "valde", "valetudo", "validus", "vel", "velit",
    "veniam", "venio", "vereor", "vero", "verto", "vicis", "vindico",
    "virtus", "voco", "volutpat", "vulpes", "vulputate", "wisi", "ymo",
    "zelus");

  $greeking = "";

  while ($words > 0) {
    $sentence_length = rand(3,10);

    $greeking .= ucfirst($dictionary[array_rand($dictionary)]);
    for ($i = 1; $i < $sentence_length; $i++) {
      $greeking .= " " . $dictionary[array_rand($dictionary)];
    }

    $greeking .= ". ";
    $words -= $sentence_length;
  }

  return $greeking;
}

function add_terms($nodes, $terms) {
  if(count($terms) > 0){
    foreach($nodes as $nid => $type) {
     $tid = $terms[$type][@array_rand($terms[$type])];
      if ($tid) {
        db_query("INSERT INTO {term_node} (nid, tid) VALUES (%d, %d)", $nid, $tid);
      }
    }
  }
}

function get_users() {
  $users = array();
  $result = db_query("SELECT uid FROM {users}");
  while($user = db_fetch_object($result)){
    $users[] = $user->uid;
  }
  return $users;
}

function get_nodes() {
  $nodes = array();
  $result = db_query("SELECT nid, type FROM {node} WHERE type IN ('story', 'blog', 'forum', 'page') AND comment = 2");
  while($node = db_fetch_object($result)){
    $nodes[$node->nid] = $node->type ;
  }
  return $nodes;
}

function get_comments() {
  $comments = array();
  $result = db_query("SELECT nid, cid FROM {comments}");
  while($comment = db_fetch_object($result)){
    $comments[$comment->nid][] = $comment->cid ;
  }
  return comments;
}

function get_terms() {
  $terms = array();
  $result = db_query("SELECT d.tid, v.vid FROM {vocabulary} v, {term_data} d WHERE v.vid = d.vid");
  while($term = db_fetch_object($result)){
    $result2 = db_query("SELECT n.type FROM {vocabulary_node_types} n WHERE n.vid = %d", $term->vid);
    while ($nt = db_fetch_object($result2)) {
      $terms[$nt->type][] = $term->tid;
    }
  }
  return $terms;
}

db_query("DELETE FROM {comments}");
db_query("DELETE FROM {node}");
db_query("DELETE FROM {node_revisions}");
db_query("DELETE FROM {node_comment_statistics}");
if (db_table_exists(forum)) { db_query("DELETE FROM {forum}"); }
db_query("DELETE FROM {url_alias}");
db_query("UPDATE {sequences} SET id = '0' WHERE name = 'node_nid'");
db_query("UPDATE {sequences} SET id = '0' WHERE name = 'comments_cid'");
db_query("ALTER TABLE {node} AUTO_INCREMENT = 1");
db_query("ALTER TABLE {comments} AUTO_INCREMENT = 1");

// get user id
$users = get_users();

$terms = get_terms();

// create 100 pseudo-random nodes:
create_nodes(50, $users);

$nodes = get_nodes();

add_terms($nodes, $terms);

$comments = get_comments();

create_comments(500, $users, $nodes, $comments);

?>
