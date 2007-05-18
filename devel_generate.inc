<?php
// $Id$
// If not in 'safe mode', increase the maximum execution time:
if (!ini_get('safe_mode')) {
  set_time_limit(240);
}

/**
 * Generate some random users.
 *
 * @param $num
 *  Number of users to generate.
 * @param $kill
 *  Boolean that indicates if existing users should be removed first.
 */
function devel_create_users($num, $kill) {
  $url = parse_url($GLOBALS['base_url']);
  if ($kill) {
    db_query('DELETE FROM {users} WHERE uid > 1');
    drupal_set_message(t('Users deleted.'));
  }
  for ($i = 0; $i < $num; $i++) {
    $uid = db_next_id('{users}_uid');
    $length = rand(4, 12);
    $name = devel_generate_word($length);
    $pass = md5(user_password());
    $mail = $name .'@'. $url['host'];
    $now = time();
    db_query("INSERT INTO {users} (uid, name, pass, mail, status, created, access) VALUES (%d, '%s', '%s', '%s', %d, %d, %d)", $uid, $name, $pass, $mail, 1, $now, $now);
  }
  drupal_set_message(t('!num_users created.', array('!num_users' => format_plural($num, '1 user', '@count users'))));
}


function devel_create_nodes($records, $users, $title_length = 8) {

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
      // get the next nid without incrementing it
      $next_nid = db_result(db_query("SELECT id FROM {sequences} WHERE name = '{node}_nid'"))+1;
      
      $title = devel_create_greeking(rand(1, $title_length), TRUE);
      /*
      $title_words = rand(1, 8);
      while($i < $title_words){
      	$title .= devel_generate_word(rand(4, 9));
      	$i;
      }
      */
      $node->title = $title;
      $node->body = "node #$next_nid ($node->type) - ". devel_create_content();
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
      unset($node);
    }
  }
}

function devel_create_comments($records, $users, $nodes, $comments) {
  $users = array_merge($users, array('0'));
  // Insert new data:
  for ($i = 1; $i <= $records; $i++) {
    $comment->cid = db_next_id("{comments}_cid");
    $comment->nid = array_rand($nodes);

    switch ($i % 3) {
      case 1:
        $comment->pid = db_result(db_query("SELECT cid FROM {comments} WHERE pid = 0 AND nid = %d ORDER BY RAND() LIMIT 1", $comment->nid));
        break;
      case 2:
        $comment->pid = db_result(db_query("SELECT cid FROM {comments} WHERE pid > 0 AND nid = %d ORDER BY RAND() LIMIT 1", $comment->nid));
        break;
      default:
        $comment->pid = 0;
    }

    $comment->subject = "comment #". $comment->cid;
    $comment->comment = "body of comment #". $comment->cid;
    $comment->uid = $users[array_rand($users)];

    db_query("INSERT INTO {comments} (cid, nid, pid, uid, subject, comment, status, thread, timestamp) VALUES (%d, %d, %d, %d, '%s', '%s', %d, %d, %d)", $comment->cid, $comment->nid, $comment->pid, $comment->uid, $comment->subject, $comment->comment, 0, 0, time());
  }
}

function devel_create_content() {
  $nparas = rand(1,12);
  $type = rand(0,3);

  $output = "";
  switch($type % 3) {
    case 1: // html
      for ($i = 1; $i <= $nparas; $i++) {
        $output .= devel_create_para(rand(10,60),1);
      }
      break;

    case 2: // brs only
      for ($i = 1; $i <= $nparas; $i++) {
        $output .= devel_create_para(rand(10,60),2);
      }
      break;

    default: // plain text
      for ($i = 1; $i <= $nparas; $i++) {
        $output .= devel_create_para(rand(10,60)) ."\n\n";
      }
  }

  return $output;
}

function devel_create_para($words, $type = 0) {
  $output = "";
  switch ($type) {
    case 1:
      $output .= "<p>";
      $output .= devel_create_greeking($words);
      $output = trim($output) ."</p>";
      break;

    case 2:
      $output .= devel_create_greeking($words);
      $output = trim($output) ."<br />";
      break;

    default:
      $output .= devel_create_greeking($words);
      $output = trim($output);
  }
  return $output;
}

function devel_create_greeking($words, $title = FALSE) {
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

  if (!$title) {
	  while ($words > 0) {
	    $sentence_length = rand(3,10);
	
	    $greeking .= ucfirst($dictionary[array_rand($dictionary)]);
	    for ($i = 1; $i < $sentence_length; $i++) {
	      $greeking .= " " . $dictionary[array_rand($dictionary)];
	    }
	
	    $greeking .= ". ";
	    $words -= $sentence_length;
	  }
  }
  else {
  	// use different method for titles
  	$title_length = $words;
  	$array = array();
  	for ($i = 0; $i < $words; $i++) {
  		$array[] = $dictionary[array_rand($dictionary)];
  	}
  	$greeking = ucwords(implode(' ', $array));
  }
  return $greeking;
}

function devel_add_terms($nodes, $terms) {
  $used = array();
  if(count($terms) > 0){
    foreach($nodes as $nid => $type) {
      $rand = rand(1, 6); // how many terms*
      while ($rand--) {
        $again = TRUE;
        while ($again) {
          $tid = $terms[$type][@array_rand($terms[$type])];
          if (!isset($used[$nid][$tid])) {
            $again = FALSE;
            $used[$nid][$tid] = $tid;
          }
        }
        if ($tid) {
          db_query("INSERT INTO {term_node} (nid, tid) VALUES (%d, %d)", $nid, $tid);
        }
      }
    }
  }
}

function devel_get_users() {
  $users = array();
  $result = db_query("SELECT uid FROM {users}");
  while($user = db_fetch_object($result)){
    $users[] = $user->uid;
  }
  return $users;
}

function devel_get_nodes($nid = 0) {
  $nodes = array();
  $result = db_query("SELECT nid, type FROM {node} WHERE type IN ('story', 'blog', 'forum', 'page') AND comment = 2 AND nid > %d", $nid);
  while($node = db_fetch_object($result)){
    $nodes[$node->nid] = $node->type ;
  }
  return $nodes;
}

function devel_get_comments() {
  $comments = array();
  $result = db_query("SELECT nid, cid FROM {comments}");
  while($comment = db_fetch_object($result)){
    $comments[$comment->nid][] = $comment->cid ;
  }
  return comments;
}

function devel_get_terms() {
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
