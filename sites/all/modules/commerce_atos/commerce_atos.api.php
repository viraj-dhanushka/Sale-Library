<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Allows modules to alter the payment request sent to ATOS.
 *
 * You can send additional settings refering to the technical documentation.
 */
function hook_commerce_atos_payment_request_alter(&$data, $order) {
  // Example code.
  $data['foo'] = 'bar';
}
