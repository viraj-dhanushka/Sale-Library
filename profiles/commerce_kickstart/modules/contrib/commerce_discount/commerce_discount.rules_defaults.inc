<?php

/**
 * @file
 * Default rule configurations for commerce discount.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_discount_default_rules_configuration() {
  $rules = array();

  // Create a rule for each commerce discount entity.
  entity_get_controller('commerce_discount')->resetCache();
  $discounts = entity_load('commerce_discount');

  // Build the discount rules.
  if ($discounts) {
    $rules += commerce_discount_build_discount_rules($discounts);
  }

  // Remove discount price components on product line items, prior to evaluating
  // product discount rules.
  $rule = rules_reaction_rule();
  $rule->active = TRUE;
  $rule->weight = -100;
  $rule
    ->event('commerce_product_calculate_sell_price')
    ->action('commerce_discount_remove_discount_components_on_products', array(
      'commerce_line_item:select' => 'commerce-line-item',
    ));
  $rules['commerce_discount_remove_discount_components_on_products'] = $rule;

  // Create a rule for shipping discounts if shipping is enabled.
  if (module_exists('commerce_shipping')) {
    $rule = rules_reaction_rule();

    $rule->label = t('Discount shipping services');
    $rule->active = TRUE;
    $rule->weight = 10;

    // Discount shipping services once all methods have been collected.
    $rule
      ->event('commerce_shipping_collect_rates')
      ->action('commerce_discount_shipping_services', array(
        'commerce_order:select' => 'commerce-order',
      ));

    $rules['commerce_discount_shipping_services'] = $rule;
  }

  return $rules;
}
