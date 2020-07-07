<?php

/**
 * @file
 * Default imported rules of this module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_wishlist_default_rules_configuration() {
  $rules = array();

  // Rule triggered when a wishlist product has been purchased.
  $rules['rules_commerce_wishlist_product_purchased'] = entity_import('rules_config', '{"rules_commerce_wishlist_product_purchased":{"LABEL":"Remove product from wishlist after purchase","PLUGIN":"reaction rule","OWNER":"rules","TAGS":["Commerce Wishlist"],"REQUIRES":["rules","commerce_wishlist"],"ON":{"commerce_wishlist_event_product_purchased":[]},"IF":[{"data_is":{"data":["commerce-wishlist-owner:uid"],"value":["commerce-order:uid"]}}],"DO":[{"commerce_wishlist_action_remove_product_for_user":{"product":["commerce-wishlist-product"],"account":["commerce-wishlist-owner"]}}]}}');

  // Rule triggered when a wishlist product has been added to the shopping cart.
  // In order to keep this compatible with previous version of the module, this
  // will be imported only if the website has old variable for removing the
  // wishlist products immediately after adding them to the shopping cart.
  if (variable_get('commerce_wishlist_remove_product', FALSE)) {
    $rules[] = entity_import('rules_config', '{"rules_adding_a_wishlist_product_to_the_cart":{"LABEL":"Remove product from wishlist after adding it to the shopping cart","PLUGIN":"reaction rule","OWNER":"rules","TAGS":["Commerce Wishlist"],"REQUIRES":["commerce_wishlist"],"ON":{"commerce_wishlist_event_product_added_to_cart":[]},"DO":[{"commerce_wishlist_action_remove_product_for_user":{"product":["commerce-wishlist-product"],"account":["commerce-wishlist-owner"]}}]}}');
    // Show a message to the admin so they know what is going on.
    drupal_set_message(t('The old setting for removing products from wishlist once they have been added to the cart is replaced by a rule. You can configure that rule <a href="@link">here</a>.', array(
      '@link' => url('admin/config/workflow/rules/reaction/manage/rules_adding_a_wishlist_product_to_the_cart'),
    )));
    // Remove the old variable as it's not necessary anymore.
    variable_del('commerce_wishlist_remove_product');
  }

  return $rules;
}
