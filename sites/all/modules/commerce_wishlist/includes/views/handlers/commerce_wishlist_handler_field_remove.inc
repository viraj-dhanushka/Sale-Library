<?php

/**
 * @file
 * Remove item form field handler implementation.
 */

/**
 * Field handler to present a link to remove a product for wishlist.
 */
class commerce_wishlist_handler_field_remove extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function render($values) {
    // Ensure the user has access to remove this product from wishlist.
    global $user;
    $line_item_id = $this->get_value($values);
    $line_item = commerce_line_item_load($line_item_id);

    $account = user_load($user->uid);
    $wishlist_id = $line_item->order_id;
    $wishlist = commerce_order_load($wishlist_id);

    if (commerce_wishlist_user_access($account, 'update', $wishlist)) {
      $text = !empty($this->options['text']) ? $this->options['text'] : t('Remove');

      $destination = drupal_get_destination();

      return l($text, 'user/' . $wishlist->uid . '/wishlist/nojs/remove/' . $line_item_id, (array(
        'query' => $destination,
      )));
    }

    return '';
  }

}
