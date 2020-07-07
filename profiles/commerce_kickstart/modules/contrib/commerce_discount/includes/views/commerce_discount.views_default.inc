<?php

/**
 * @file
 * Views defaults for Commerce discount module.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_discount_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'commerce_discount_overview';
  $view->description = 'Display a list of discounts for store admin.';
  $view->tag = 'default';
  $view->base_table = 'commerce_discount';
  $view->human_name = 'Discounts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'status',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'label' => 'label',
    'type_1' => 'type_1',
    'type' => 'type',
    'commerce_discount_usage' => 'commerce_discount_usage',
    'commerce_discount_date' => 'commerce_discount_date',
    'commerce_discount_date_1' => 'commerce_discount_date_1',
    'status' => 'status',
    'operations_dropbutton' => 'operations_dropbutton',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'label' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_discount_usage' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_discount_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_discount_date_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'operations_dropbutton' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'No discounts found.';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['commerce_discount_offer_target_id']['id'] = 'commerce_discount_offer_target_id';
  $handler->display->display_options['relationships']['commerce_discount_offer_target_id']['table'] = 'field_data_commerce_discount_offer';
  $handler->display->display_options['relationships']['commerce_discount_offer_target_id']['field'] = 'commerce_discount_offer_target_id';
  $handler->display->display_options['relationships']['commerce_discount_offer_target_id']['required'] = TRUE;
  /* Field: Commerce Discount: Admin title */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'commerce_discount';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  /* Field: Commerce Discount Offer: Type */
  $handler->display->display_options['fields']['type_1']['id'] = 'type_1';
  $handler->display->display_options['fields']['type_1']['table'] = 'commerce_discount_offer';
  $handler->display->display_options['fields']['type_1']['field'] = 'type';
  $handler->display->display_options['fields']['type_1']['relationship'] = 'commerce_discount_offer_target_id';
  $handler->display->display_options['fields']['type_1']['label'] = 'Offer type';
  /* Field: Commerce Discount: Discount dates */
  $handler->display->display_options['fields']['commerce_discount_date']['id'] = 'commerce_discount_date';
  $handler->display->display_options['fields']['commerce_discount_date']['table'] = 'field_data_commerce_discount_date';
  $handler->display->display_options['fields']['commerce_discount_date']['field'] = 'commerce_discount_date';
  $handler->display->display_options['fields']['commerce_discount_date']['label'] = 'Valid dates';
  $handler->display->display_options['fields']['commerce_discount_date']['empty'] = 'Any';
  $handler->display->display_options['fields']['commerce_discount_date']['settings'] = array(
    'format_type' => 'short',
    'custom_date_format' => '',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Commerce Discount: Analytics */
  $handler->display->display_options['fields']['commerce_discount_usage']['id'] = 'commerce_discount_usage';
  $handler->display->display_options['fields']['commerce_discount_usage']['table'] = 'commerce_discount';
  $handler->display->display_options['fields']['commerce_discount_usage']['field'] = 'commerce_discount_usage';
  /* Field: Commerce Discount: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_discount';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['status']['type'] = 'active-disabled';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Commerce Discount: Sort order */
  $handler->display->display_options['fields']['sort_order']['id'] = 'sort_order';
  $handler->display->display_options['fields']['sort_order']['table'] = 'commerce_discount';
  $handler->display->display_options['fields']['sort_order']['field'] = 'sort_order';
  /* Field: Commerce Discount: Operations */
  $handler->display->display_options['fields']['operations_dropbutton']['id'] = 'operations_dropbutton';
  $handler->display->display_options['fields']['operations_dropbutton']['table'] = 'commerce_discount';
  $handler->display->display_options['fields']['operations_dropbutton']['field'] = 'operations_dropbutton';
  /* Sort criterion: Commerce Discount: Status */
  $handler->display->display_options['sorts']['status']['id'] = 'status';
  $handler->display->display_options['sorts']['status']['table'] = 'commerce_discount';
  $handler->display->display_options['sorts']['status']['field'] = 'status';
  $handler->display->display_options['sorts']['status']['order'] = 'DESC';
  /* Sort criterion: Commerce Discount: Sort order */
  $handler->display->display_options['sorts']['sort_order']['id'] = 'sort_order';
  $handler->display->display_options['sorts']['sort_order']['table'] = 'commerce_discount';
  $handler->display->display_options['sorts']['sort_order']['field'] = 'sort_order';
  /* Filter criterion: Commerce Discount: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_discount';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'product_discount' => 'product_discount',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Discount type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['required'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Commerce Discount Offer: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'commerce_discount_offer';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'commerce_discount_offer_target_id';
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  $handler->display->display_options['filters']['type_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['operator_id'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['label'] = 'Offer type';
  $handler->display->display_options['filters']['type_1']['expose']['operator'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  $views['commerce_discount_overview'] = $view;

  return $views;
}
