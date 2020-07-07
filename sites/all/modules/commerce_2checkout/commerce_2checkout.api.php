<?php

/**
 * @file
 * Documents hooks provided by the Commerce 2Checkout module.
 */


/**
 * Allows modules to alter the data array used to create the request form.
 *
 * @param &$data
 *   The data array used to create the request form elements.
 * @param $order
 *   The full order object the request form is being generated for.
 *
 * @see commerce_2checkout_purchase_parameters()
 */
function hook_commerce_2checkout_order_form_data_alter(&$data, $order) {
  $USD_BOB = (float) variable_get('currency_usd_2_bob', 6.97);
  $data['currency_code'] = 'USD';

  $i = 0;
  foreach ($order->commerce_line_items[LANGUAGE_NONE] as $li) {
    $data['li_' . $i . '_price'] = number_format((float) $data['li_' . $i . '_price'] / $USD_BOB, 2, '.', '');
    $i++;
  }
}

/**
 * Implements hook_commerce_2checkout_validate_form_data_alter().
 */
function hook_commerce_2checkout_validate_form_data_alter(&$data, $order) {
  $USD_BOB = (float) variable_get('currency_usd_2_bob', 6.97);

  $data['total'] = number_format((float) $data['total'] * $USD_BOB, 2, '.', '');
}
