<?php

/**
 * @file
 * Provides the wishlist for use in Drupal Commerce.
 */

/**
 * Form callback: settings form.
 */
function commerce_wishlist_admin_form($form, &$form_state) {
  $form['commerce_wishlist_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Button/link weight'),
    '#description' => t('Set the position of the button/link "Add to wishlist".'),
    '#default_value' => variable_get('commerce_wishlist_weight', 0),
    '#required' => TRUE,
  );
  $form['commerce_wishlist_element'] = array(
    '#type' => 'select',
    '#title' => t('"Add to wishlist" element'),
    '#options' => array('link' => t('Ajax link'), 'button' => t('Button')),
    '#description' => t('The way "Add to wishlist" element will be displayed. Either as a button or link.'),
    '#default_value' => variable_get('commerce_wishlist_element', 'button'),
  );
  $form['commerce_wishlist_product_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Product types'),
    '#options' => commerce_product_type_options_list(),
    '#description' => t('Enable "Add to wishlist" element for these product types.'),
    '#default_value' => variable_get('commerce_wishlist_product_types', array()),
  );

  return system_settings_form($form);
}
