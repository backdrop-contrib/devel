<?php

/*
 * @file
 *   PHPUnit Tests for devel. This uses Drush's own test framework, based on PHPUnit.
 *   To run the tests, use phpunit --bootstrap=/path/to/drush/tests/drush_testcase.inc.
 *   Note that we are pointing to the drush_testcase.inc file under /tests subdir in drush.
 */
class develCase extends Drush_CommandTestCase {

  public function testFnView() {
    $sites = $this->setUpDrupal(1, TRUE);
    $options = array(
      'root' => $this->webroot(),
      'uri' => key($sites),
    );
    $this->drush('pm-download', array('devel'), $options + array('cache' => NULL));
    $this->drush('pm-enable', array('devel'), $options + array('skip' => NULL, 'yes' => NULL));

    $this->drush('fn-view', array('drush_main'), $options);
    $output = $this->getOutput();
    $this->assertTrue((bool)strpos($output, '@return'), 'Output contain @return Doxygen.');
    $this->assertTrue((bool)strpos($output, 'function drush_main() {'), 'Output contains function drush_main() declaration');
  }
}