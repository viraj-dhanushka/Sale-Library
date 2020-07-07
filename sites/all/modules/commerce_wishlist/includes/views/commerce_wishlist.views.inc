<?php

/**
 * @file
 * Wish list views.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_wishlist_views_data_alter(&$data) {
  // Expose links to operate on the product.
  $data['commerce_line_item']['wishlist_remove'] = array(
    'field' => array(
      'title' => t('Remove'),
      'help' => t('Provide a simple link to remove a product from the wishlist.'),
      'real field' => 'line_item_id',
      'handler' => 'commerce_wishlist_handler_field_remove',
    ),
  );
  $data['commerce_line_item']['wishlist_add_to_cart'] = array(
    'field' => array(
      'title' => t('Add to Cart'),
      'help' => t('Provide a button to a product to the cart from the wishlist.'),
      'real field' => 'line_item_id',
      'handler' => 'commerce_wishlist_handler_field_add_to_cart_form',
    ),
  );
}
