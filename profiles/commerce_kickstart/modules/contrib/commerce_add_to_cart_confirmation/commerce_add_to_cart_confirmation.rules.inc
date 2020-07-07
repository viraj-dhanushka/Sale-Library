<?php

/**
 * @file
 * Rules integration for the Commerce add to cart confirmation module.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_add_to_cart_confirmation_rules_action_info() {
  return array(
    'commerce_add_to_cart_confirmation_message' => array(
      'label' => t('Add to Cart message'),
      'parameter' => array(
        'commerce_line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('Line item'),
        ),
      ),
      'group' => t('Commerce (contrib)'),
      'callbacks' => array(
        'execute' => 'commerce_add_to_cart_confirmation_rules_add_to_cart_message',
      ),
    ),
  );
}

/**
 * Rules action: displays a custom Add to Cart message.
 */
function commerce_add_to_cart_confirmation_rules_add_to_cart_message($line_item, $continue) {
  $view = views_embed_view('confirm_message_product_display', 'default', $line_item->line_item_id);
  $message = theme('commerce_add_to_cart_confirmation_message', array('view' => $view));
  drupal_set_message($message, 'commerce-add-to-cart-confirmation');
}
