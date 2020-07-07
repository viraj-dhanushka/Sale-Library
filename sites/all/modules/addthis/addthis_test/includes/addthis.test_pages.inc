<?php
/**
 * @file
 * Callbacks for test pages that include AddThis functionality.
 */

/**
 * Page callback to test a page load without any js loaded.
 *
 * @return array
 *   Render array for a page.
 */
function addthis_test_no_js() {
  $content = array();

  $content['explanation'] = array(
    '#markup' => 'No widget js is loaded on this page. Unless specified in the settings.',
  );

  return $content;
}

/**
 * Page callback to test a page load with js loaded.
 *
 * @return array
 *   Render array for this page.
 */
function addthis_test_with_js() {
  $content = array();

  $content['explanation'] = array(
    '#markup' => 'Widget js is loaded on this page through hook_page_build().<br>
    <!-- Go to www.addthis.com/dashboard to customize your tools -->
    <div class="addthis_sharing_toolbox"></div>',
  );

  return $content;
}

