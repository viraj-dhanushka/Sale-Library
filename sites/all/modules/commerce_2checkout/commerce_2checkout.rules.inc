<?php

/**
 * Rules integration for 2checkout.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_2checkout_rules_action_info() {
  $actions = array();

  // Must enable 2checkout payment before this will work!!
  $actions['commerce_2checkout_redirect'] = array(
    'label' => t('Redirect to 2checkout'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order in checkout'),
      ),
    ),
    'group' => t('Commerce Checkout'),
    'base' => 'commerce_2checkout_redirect_checkout',
  );
  $actions['commerce_2checkout_add_shipping'] = array(
    'label' => t('Add shipping rates from $_REQUEST'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order in checkout'),
      ),
    ),
    'group' => t('Commerce Shipping'),
    'base' => 'commerce_2checkout_add_shipping',
  );
  return $actions;
}
