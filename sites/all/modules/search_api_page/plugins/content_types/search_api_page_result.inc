<?php

/*
 * Plugins are described by creating a $plugin array which will be used by the
 * system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Search API page results'),
  'icon' => 'icon_search.png',
  'description' => t('The results of a Search API page search using keywords.'),
  'required context' => new ctools_context_required(t('Keywords'), 'string'),
  'category' => t('Widgets'),
  'edit form' => 'search_api_page_result_content_type_edit_form',
  'render callback' => 'search_api_page_result_content_type_render',
  'admin title' => 'search_api_page_result_content_type_admin_title',
  'defaults' => array(
    'page' => '',
    'per_page' => '',
    'result_page_search_form'  => '',
    'get_per_page' => '',
    'view_mode' => '',
  ),
);

/**
 * Renders the custom content type.
 */
function search_api_page_result_content_type_render($subtype, $conf, $panel_args, $context) {
  if (!search_api_page_load($conf['page'])) {
    return NULL;
  }

  $keys = NULL;
  if (!empty($context) && isset($context->data)) {
    $keys = $context->data;
  }

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'search_api_page';
  $block->delta   = 'result';

  module_load_include('inc', 'search_api_page', 'search_api_page.pages');

  $override_options = array(
    'per_page' => $conf['per_page'],
    'result_page_search_form' => $conf['result_page_search_form'],
    'get_per_page' => $conf['get_per_page'],
    'view_mode' => $conf['view_mode'],
  );

  $block->content = search_api_page_view($conf['page'], $keys, $override_options);

  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function search_api_page_result_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $pages = array();
  foreach (search_api_page_load_multiple(FALSE, array('enabled' => TRUE)) as $info) {
    $pages[$info->machine_name] = $info->name;
  }

  if (!empty($conf['page'])) {
    $info = search_api_page_load($conf['page']);
    $index = search_api_index_load($info->index_id);
  }

  $form['page'] = array(
    '#type' => 'select',
    '#title' => t('Search API Page'),
    '#options' => $pages,
    '#default_value' => $conf['page'],
    '#required' => TRUE,
  );

  $form['per_page'] = array(
    '#type' => 'select',
    '#title' => t('Results per page'),
    '#description' => t('Select how many items will be displayed on one page of the search result.'),
    '#options' => drupal_map_assoc(array(5, 10, 20, 30, 40, 50, 60, 80, 100)),
    '#default_value' => $conf['per_page'],
    '#required' => TRUE,
  );

  $form['result_page_search_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show search form on result page'),
    '#description' => t('Enable or disable the search form on the result page'),
    '#default_value' => isset($conf['result_page_search_form']) ? $conf['result_page_search_form'] : TRUE,
  );

  $form['get_per_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow GET override'),
    '#description' => t('Allow the „Results per page“ setting to be overridden from the URL, using the "per_page" GET parameter.<br />' .
        'Example: <code>http://example.com/search_results?per_page=7</code>'),
    '#default_value' => !empty($conf['get_per_page']),
  );

  $view_modes = array(
    'search_api_page_result' => t('Themed as search results'),
  );
  // For entities, we also add all entity view modes.
  if (isset($index) && $index->getEntityType()) {
    $entity_info = entity_get_info($index->getEntityType());
    foreach ($entity_info['view modes'] as $mode => $mode_info) {
      $view_modes[$mode] = $mode_info['label'];
    }
  }
  if (count($view_modes) > 1) {
    $form['view_mode'] = array(
      '#type' => 'select',
      '#title' => t('View mode'),
      '#options' => $view_modes,
      '#description' => t('Select how search results will be displayed.'),
      '#size' => 1,
      '#default_value' => isset($conf['view_mode']) ? $conf['view_mode'] : 'search_api_page_result',
      '#required' => TRUE,
    );
  }
  else {
    $form['view_mode'] = array(
      '#type' => 'value',
      '#value' => key($view_modes),
    );
  }

  return $form;
}

/**
 * Submit handler for the search form.
 */
function search_api_page_result_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function search_api_page_result_content_type_admin_title($subtype, $conf, $context) {
  $info = search_api_page_load($conf['page']);
  $page = isset($info->name) ? $info->name : t('Missing/broken type');
  return t('%page Search API page search result', array('%page' => $page));
}
