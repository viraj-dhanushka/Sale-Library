<?php

/**
 * @file
 * Implementation of rules hooks and actions.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_wishlist_rules_event_info() {
  return array(
    'commerce_wishlist_product_add' => array(
      'label' => t('After adding a product to wishlist'),
      'help' => t('Triggers when a product has been added to wishlist.'),
      'group' => t('Commerce Wishlist'),
      'variables' => array(
        'owner' => array(
          'type' => 'user',
          'label' => t('Wishlist owner'),
        ),
        'product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
        'line_item' => array(
          'type'  => 'line_item',
          'label' => t('Product'),
        ),
      ),
    ),
    'commerce_wishlist_product_add_to_cart' => array(
      'label' => t('After adding a wishlist product to the cart'),
      'help' => t('Triggers when a wishlist product has been added to the shopping cart.'),
      'group' => t('Commerce Wishlist'),
      'variables' => array(
        'commerce_wishlist_owner' => array(
          'type' => 'user',
          'label' => t('Wishlist owner'),
        ),
        'commerce_wishlist_product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
      ),
    ),
    'commerce_wishlist_product_purchased' => array(
      'label' => t('After purchasing a product from a wishlist'),
      'help' => t('Triggers when a wishlist product is purchased.'),
      'group' => t('Commerce Wishlist'),
      'variables' => array(
        'commerce_wishlist_owner' => array(
          'type' => 'user',
          'label' => t('Wishlist owner'),
        ),
        'commerce_wishlist_product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('The order that the product was purchased in.'),
        ),
      ),
    ),
    'commerce_wishlist_product_delete' => array(
      'label' => t('After removing a product from wishlist'),
      'help' => t('Triggers when a product has been removed from a wishlist.'),
      'group' => t('Commerce Wishlist'),
      'variables' => array(
        'commerce_wishlist' => array(
          'type' => 'commerce_wishlist',
          'label' => t('Wishlist'),
        ),
        'commerce_wishlist_product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_wishlist_rules_action_info() {
  return array(
    'commerce_wishlist_action_remove_product_for_user' => array(
      'label' => t('Remove product from wishlist'),
      'group' => t('Commerce Wishlist'),
      'parameter' => array(
        'product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
        'account' => array(
          'type'  => 'user',
          'label' => t('User'),
        ),
      ),
    ),
  );
}

/**
 * Rules action callback to remove a specific product from user's wishlist.
 */
function commerce_wishlist_action_remove_product_for_user($product, $account) {
  commerce_wishlist_product_remove($product, $account);
}
