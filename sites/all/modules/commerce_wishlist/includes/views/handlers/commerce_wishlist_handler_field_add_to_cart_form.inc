<?php

/**
 * @file
 * Add to cart form field handler implementation.
 */

/**
 * Class commerce_wishlist_handler_field_add_to_cart_form.
 */
class commerce_wishlist_handler_field_add_to_cart_form extends commerce_cart_handler_field_add_to_cart_form {

  function option_definition() {
    $options = parent::option_definition();
    $options['remove_from_wishlist'] = array('default' => TRUE);
    return $options;
  }

  /**
   * Provide the add to cart display options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['remove_from_wishlist'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove this item from the wishlist'),
      '#default_value' => $this->options['remove_from_wishlist'],
    );
  }

  function render($values) {
    // Attempt to load the specified product.
    $wish_list_line_item = $this->get_value($values);

    if (!empty($wish_list_line_item)) {
      $product = commerce_product_load($wish_list_line_item->commerce_product[LANGUAGE_NONE][0]['product_id']);
      // Extract a default quantity for the Add to Cart form line item.
      $default_quantity = $this->options['default_quantity'] <= 0 ? 1 : $this->options['default_quantity'];
      $product_ids = array($product->product_id);

      // Build the line item we'll pass to the Add to Cart form.
      $line_item = commerce_product_line_item_new($product, $default_quantity, 0, array(), $this->options['line_item_type']);
      $line_item->data['context']['product_ids'] = $product_ids;
      $line_item->data['context']['add_to_cart_combine'] = $this->options['combine'];

      // Generate a form ID for this add to cart form.
      $form_id = commerce_cart_add_to_cart_form_id($product_ids);

      // Add the display path to the line item's context data array if specified.
      if ($this->options['display_path']) {
        if ($this->view->display[$this->view->current_display]->display_plugin == 'page') {
          $line_item->data['context']['display_path'] = $this->view->display[$this->view->current_display]->display_options['path'];
        }
        else {
          $line_item->data['context']['display_path'] = current_path();
        }
      }

      // Store the View data in the context data array as well.
      $line_item->data['context']['view'] = array(
        'view_name' => $this->view->name,
        'display_name' => $this->view->current_display,
        'human_name' => $this->view->human_name,
        'page' => $this->view->current_page,
        'remove_from_wishlist' => $this->options['remove_from_wishlist'],
      );

      // Build the Add to Cart form using the prepared values.
      $form = drupal_get_form($form_id, $line_item, $this->options['show_quantity'], array('wish_list_line_item' => $wish_list_line_item));

      return drupal_render($form);
    }
  }

}
