<?php


class AddThisTestHelper {

  public static function stringContains($string, $contains) {
    return strpos($string, $contains) !== FALSE;
  }

  public static function generateRandomLowercaseString() {
    return drupal_strtolower(DrupalWebTestCase::randomName());
  }
}
