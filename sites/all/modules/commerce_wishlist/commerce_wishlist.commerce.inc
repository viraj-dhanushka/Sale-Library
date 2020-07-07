<?php

/**
 * @file
 * Commerce methods.
 */

/**
 * Implements hook_commerce_order_state_info().
 */
function commerce_wishlist_commerce_order_state_info() {
  $order_states = array();

  $order_states['wishlist'] = array(
    'name' => 'wishlist',
    'title' => t('Wishlist'),
    'description' => t('These are orders which hold products but cannot be ordered without being converted to a shopping cart.'),
    'weight' => 10,
    'default_status' => 'wishlist',
  );

  return $order_states;
}

/**
 * Implements hook_commerce_order_status_info().
 */
function commerce_wishlist_commerce_order_status_info() {
  $order_statuses = array();

  $order_statuses['wishlist'] = array(
    'name' => 'wishlist',
    'title' => t('Wishlist'),
    'state' => 'wishlist',
  );

  return $order_statuses;
}
