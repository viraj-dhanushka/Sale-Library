<?php
/**
 * @file
 * Class to manage a WidgetJs url.
 */

class AddThisWidgetJsUrl {

  protected $url;
  protected $attributes = array();

  /**
   * Sepecify the url through the construct.
   */
  public function __construct($url) {
    $this->url = $url;
  }

  /**
   * Add a attribute to the querystring.
   */
  public function addAttribute($name, $value) {
    $this->attributes[$name] = $value;
  }

  /**
   * Remove a attribute from the querystring.
   */
  public function removeAttribute($name) {
    if (isset($attributes[$name])) {
      unset($attributes[$name]);
    }
  }

  /**
   * Get the full url for the widgetjs.
   */
  public function getFullUrl() {
    $querystring_elements = array();
    foreach ($this->attributes as $key => $value) {
      if (!empty($value)) {
        $querystring_elements[] = $key . '=' . $value;
      }
    }

    $querystring = implode('&', $querystring_elements);

    return $this->url . '#' . $querystring;
  }

}
