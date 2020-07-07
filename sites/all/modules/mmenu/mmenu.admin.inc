<?php

/**
 * @file
 * Mmenu administration menu items.
 */

/**
 * Callback function for the Mmenu admin page.
 */
function mmenu_admin_settings($mmenu_name = '') {
  if (!empty($mmenu_name)) {
    drupal_goto('admin/config/mmenu/' . $mmenu_name);
  }
  return '';
}

/**
 * Form constructor for the Mmenu settings form.
 */
function mmenu_admin_settings_form($form, &$form_state, $mmenu_name = '') {
  $mmenu = mmenu_list($mmenu_name);
  $site_name = variable_get('site_name', t('Drupal'));
  $bool_options = array(
    'true' => t('Yes'),
    'false' => t('No'),
  );

  $form['general'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#weight' => -5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['general']['enabled'] = array(
    '#title' => t('Enabled?'),
    '#type' => 'select',
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => $mmenu['enabled'] ? 1 : 0,
    '#required' => TRUE,
    '#weight' => -3,
    '#description' => t('Enable or disable the mmenu.'),
  );
  $form['general']['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => $mmenu['title'],
    '#required' => TRUE,
    '#weight' => -1,
    '#description' => t('The administrator title of the mmenu.'),
  );

  $form['general']['name'] = array(
    '#type' => 'hidden',
    '#value' => $mmenu_name,
  );

  $drupal_blocks = mmenu_get_blocks();
  $options = array();
  $options[] = t('--- Please select a block ---');
  foreach ($drupal_blocks as $module => $drupal_block) {
    foreach ($drupal_block as $delta => $info) {
      $options[$module . '|' . $delta] = drupal_ucfirst($module) . ' - ' . $info['info'];
    }
  }

  $form['blocks'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Blocks'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $blocks = array();
  foreach ($mmenu['blocks'] as $k => $block) {
    $blocks[] = $block;
  }
  $mmenu_allowed_blocks_nums = variable_get('mmenu_allowed_blocks_nums', 5);
  for ($i = count($blocks); $i < $mmenu_allowed_blocks_nums; $i++) {
    $blocks[$i]['title'] = '';
    $blocks[$i]['module'] = '';
    $blocks[$i]['delta'] = '';
    $blocks[$i]['collapsed'] = TRUE;
    $blocks[$i]['wrap'] = FALSE;
  }

  foreach ($blocks as $k => $block) {
    $form['blocks'][$k] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Block'),
      '#collapsible' => TRUE,
      '#collapsed' => !empty($block['module']) && !empty($block['delta']) ? FALSE : TRUE,
    );
    $form['blocks'][$k]['module_delta'] = array(
      '#title' => t('Select a block'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => !empty($block['module']) && !empty($block['delta']) ? $block['module'] . '|' . $block['delta'] : '',
      '#description' => t('Select a block to display on the mmenu.'),
    );
    $form['blocks'][$k]['menu_parameters'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Menu parameters'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['blocks'][$k]['menu_parameters']['min_depth'] = array(
      '#title' => t('Min depth'),
      '#type' => 'select',
      '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9)),
      '#default_value' => (isset($block['menu_parameters']) && isset($block['menu_parameters']['min_depth'])) ? $block['menu_parameters']['min_depth'] : 1,
      '#description' => t('The minimum depth of menu links in the resulting tree. Defaults to 1, which is the default to build a whole tree for a menu (excluding menu container itself).'),
    );

    $form['blocks'][$k]['title'] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#default_value' => $block['title'],
      '#description' => t('Override the default title for the block. Use <em>!placeholder</em> to display no title, or leave blank to use the default block title.', array('!placeholder' => '&lt;none&gt;')),
    );
    $form['blocks'][$k]['collapsed'] = array(
      '#title' => t('Collapsed'),
      '#type' => 'select',
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
      '#default_value' => $block['collapsed'] ? 1 : 0,
      '#description' => t('Collapse or expand the block content by default.'),
    );
    $form['blocks'][$k]['wrap'] = array(
      '#title' => t('Wrap'),
      '#type' => 'select',
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
      '#default_value' => $block['wrap'] ? 1 : 0,
      '#description' => t('Determine if needs to wrap the block content. Usually to set it to true if the block is not a system menu.'),
    );
  }

  $options = $mmenu['options'];
  $form['options'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Mmenu options'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/options')),
  );

  $form['options']['slidingSubmenus'] = array(
    '#title' => t('slidingSubmenus'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['slidingSubmenus']) && $options['slidingSubmenus'] ? 'true' : 'false',
    '#description' => t('Whether or not the submenus should come sliding in from the right. If false, the submenus expand below their parent.'),
  );

  // Get predefined mmenu extenion list.
  $extensions = mmenu_extension_list();

  foreach ($extensions as $extension) {
    $form['options'][$extension['name']] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => $extension['title'] . t(' (extension)'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => $extension['url'])),
    );
    $extension_options = array();
    foreach ($extension['options'] as $option) {
      $extension_options[$option['name']] = $option['name'] . ': ' . $option['title'];
    }
    $default_option = $extension['multiple'] ? array() : current(array_keys($extension_options));
    $form['options'][$extension['name']]['options'] = array(
      '#type' => $extension['multiple'] ? 'checkboxes': 'radios',
      '#options' => $extension_options,
      '#default_value' => empty($options[$extension['name']]['options']) ? $default_option : $options[$extension['name']]['options'],
    );
  }

  $form['options']['autoHeight'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Auto height (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/auto-height.html')),
  );
  $form['options']['autoHeight']['height'] = array(
    '#title' => t('height'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array('default', 'auto', 'hightest')),
    '#default_value' => isset($options['autoHeight']['height']) ? $options['autoHeight']['height'] : 'default',
    '#description' => t('What type of height to use.Possible values: "default", "auto" or "highest".'),
  );

  $form['options']['backButton'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Back button (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/back-button.html')),
  );
  $form['options']['backButton']['close'] = array(
    '#title' => t('close'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['backButton']['close']) && $options['backButton']['close'] ? 'true' : 'false',
    '#description' => t('Whether or not the menu should close when pressing the browser back button.'),
  );

  $form['options']['columns'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Columns (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/columns.html')),
  );
  $form['options']['columns']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['columns']['add']) && $options['columns']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not a to split up the panels in multiple columns.'),
  );
  $form['options']['columns']['hideNavbars'] = array(
    '#title' => t('hideNavbars'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['columns']['hideNavbars']) && $options['columns']['hideNavbars'] ? 'true' : 'false',
    '#description' => t('Whether or not to hide the navbar in panels.'),
  );
  $form['options']['columns']['visible'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('visible'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('A map of the columns.visible options.'),
  );
  $form['options']['columns']['visible']['max'] = array(
    '#title' => t('max'),
    '#type' => 'textfield',
    '#default_value' => isset($options['columns']['visible']['max']) ? $options['columns']['visible']['max'] : 3,
    '#description' => t('The maximum number of visible columns.'),
  );
  $form['options']['columns']['visible']['min'] = array(
    '#title' => t('min'),
    '#type' => 'textfield',
    '#default_value' => isset($options['columns']['visible']['min']) ? $options['columns']['visible']['min'] : 1,
    '#description' => t('The minimum number of visible columns.'),
  );

  $form['options']['counters'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('counters (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/counters.html')),
  );
  $form['options']['counters']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['counters']['add']) && $options['counters']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically append a counter to each menu item that has a submenu.'),
  );
  $form['options']['counters']['addTo'] = array(
    '#title' => t('addTo'),
    '#type' => 'textfield',
    '#default_value' => empty($options['counters']['addTo']) ? 'panels' : $options['counters']['addTo'],
    '#description' => t('Where to add the counters. Possible values: "panels" and a valid jQuery selector.'),
  );
  $form['options']['counters']['update'] = array(
    '#title' => t('update'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['counters']['update']) && $options['counters']['update'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically count the number of items in the submenu.'),
  );

  $form['options']['dividers'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dividers (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/dividers.html')),
  );
  $form['options']['dividers']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['dividers']['add']) && $options['dividers']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically add dividers to the menu (dividing the list items alphabetically).'),
  );
  $form['options']['dividers']['addTo'] = array(
    '#title' => t('addTo'),
    '#type' => 'textfield',
    '#default_value' => empty($options['dividers']['addTo']) ? 'panels' : $options['dividers']['addTo'],
    '#description' => t('Where to add the dividers. Possible values: "panels" and a valid jQuery selector.'),
  );
  $form['options']['dividers']['collapse'] = array(
    '#title' => t('collapse'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['dividers']['collapse']) && $options['dividers']['collapse'] ? 'true' : 'false',
    '#description' => t('Whether or not to collapse all subsequent list-items that have the classname specified in classNames.dividers.collapsed in the configuration object.'),
  );
  $form['options']['dividers']['fixed'] = array(
    '#title' => t('fixed'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['dividers']['fixed']) && $options['dividers']['fixed'] ? 'true' : 'false',
    '#description' => t('Whether or not to keep the divider of the currently viewed section fixed at the top.'),
  );

  $form['options']['dragOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dragOpen (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/drag-open.html')),
  );
  $form['options']['dragOpen']['open'] = array(
    '#title' => t('open'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['dragOpen']['open']) && $options['dragOpen']['open'] ? 'true' : 'false',
    '#description' => t('Whether or not the menu should open when dragging the page.') . '<br />' . t('Tip: The jQuery.hammer plugin enables dragging on touch and desktop devices. If you only want to enable dragging the menu open on touchscreens, test for touch support using $.fn.mmenu.support.touch.'),
  );
  $form['options']['dragOpen']['pageNode'] = array(
    '#title' => t('pageNode'),
    '#type' => 'textfield',
    '#default_value' => isset($options['dragOpen']['pageNode']) ? $options['dragOpen']['pageNode'] : 'body',
    '#description' => t('The node on which the user can drag to open the menu. If omitted, the entire page is used.'),
  );
  $form['options']['dragOpen']['threshold'] = array(
    '#title' => t('threshold'),
    '#type' => 'textfield',
    '#default_value' => isset($options['dragOpen']['threshold']) ? $options['dragOpen']['threshold'] : 100,
    '#description' => t('The minimum amount of pixels to drag before actually opening the menu, less than 50 is not advised.'),
  );
  $form['options']['dragOpen']['maxStartPos'] = array(
    '#title' => t('maxStartPos'),
    '#type' => 'textfield',
    '#default_value' => isset($options['dragOpen']['maxStartPos']) ? $options['dragOpen']['maxStartPos'] : 50,
    '#description' => t('The maximum x-position to start dragging the page.For a menu with a position set to "top" or "bottom", the default value is 50.'),
  );

  $form['options']['dropdown'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dropdown (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/dropdown.html')),
  );
  $form['options']['dropdown']['drop'] = array(
    '#title' => t('drop'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['dropdown']['drop']) && $options['dropdown']['drop'] ? 'true' : 'false',
    '#description' => t('Whether or not to open the menu as a dropdown from the menu-button.'),
  );
  $form['options']['dropdown']['event'] = array(
    '#title' => t('event'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array('click', 'hover', 'click hover', 'hover click')),
    '#default_value' => isset($options['dropdown']['event']) ? $options['dropdown']['event'] : 'click',
    '#description' => t('The event to open and close the menu. Possible values: "click", "hover", "click hover" and "hover click".'),
  );
  $form['options']['dropdown']['position'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('position'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('A map of the dropdown.position options.'),
  );
  $form['options']['dropdown']['position']['of'] = array(
    '#title' => t('of'),
    '#type' => 'textfield',
    '#default_value' => isset($options['dropdown']['position']['of']) ? $options['dropdown']['position']['of'] : 'body',
    '#description' => t('A valid jQuery selector for the position.of option.'),
  );
  $form['options']['dropdown']['position']['x'] = array(
    '#title' => t('x'),
    '#type' => 'radios',
    '#options' => drupal_map_assoc(array('left', 'right')),
    '#description' => t('How to horizontally position the menu relative to the button. Possible values: "left" and "right". If omitted, the menu will automatically be positioned.'),
  );
  if (isset($options['dropdown']['position']['x'])) {
    $form['options']['dropdown']['position']['x']['#default_value'] = $options['dropdown']['position']['x'];
  }
  $form['options']['dropdown']['position']['y'] = array(
    '#title' => t('y'),
    '#type' => 'radios',
    '#options' => drupal_map_assoc(array('top', 'bottom')),
    '#description' => t('How to vertically position the menu relative to the button. Possible values: "top" and "bottom". If omitted, the menu will automatically be positioned.'),
  );
  if (isset($options['dropdown']['position']['y'])) {
    $form['options']['dropdown']['position']['y']['#default_value'] = $options['dropdown']['position']['y'];
  }
  $form['options']['dropdown']['tip'] = array(
    '#title' => t('tip'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['dropdown']['tip']) && $options['dropdown']['tip'] ? 'true' : 'false',
    '#description' => t('Whether or not to prepend the menu with a tip pointing to the menu-button.'),
  );

  $form['options']['fixedElements'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('fixedElements (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/fixed-elements.html')),
  );
  $form['options']['fixedElements']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no options.'),
  );

  $form['options']['iconPanels'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Icon panels (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/icon-panels.html')),
  );
  $form['options']['iconPanels']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['iconPanels']['add']) && $options['iconPanels']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not a small part of parent panels should be visible.'),
  );
  $form['options']['iconPanels']['hideNavbars'] = array(
    '#title' => t('hideNavbars'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['iconPanels']['hideNavbars']) && $options['iconPanels']['hideNavbars'] ? 'true' : 'false',
    '#description' => t('Whether or not to hide the navbar in panels.'),
  );
  $form['options']['iconPanels']['visible'] = array(
    '#title' => t('visible'),
    '#type' => 'textfield',
    '#default_value' => isset($options['iconPanels']['visible']) ? $options['iconPanels']['visible'] : 3,
    '#description' => t('The number of visible parent panels.'),
  );

  $form['options']['navbarElements'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Navbars (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/navbars.html')),
  );

  $form['options']['navbarElements']['header'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('header'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['options']['navbarElements']['header']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['navbarElements']['header']['add']) && $options['navbarElements']['header']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically prepend a fixed header to the menu.'),
  );
  $form['options']['navbarElements']['header']['title'] = array(
    '#title' => t('title'),
    '#type' => 'textfield',
    '#default_value' => isset($options['navbarElements']['header']['title']) ? $options['navbarElements']['header']['title'] : $site_name,
    '#description' => t('The text above the main menu.'),
  );
  $form['options']['navbarElements']['header']['height'] = array(
    '#title' => t('height'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5)),
    '#default_value' => isset($options['navbarElements']['header']['height']) ? $options['navbarElements']['header']['height'] : 1,
    '#description' => t('The size of the navbar, will be multiplied by 40px.'),
  );

  $form['options']['navbarElements']['footer'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('footer'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['options']['navbarElements']['footer']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['navbarElements']['footer']['add']) && $options['navbarElements']['footer']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically append a fixed footer to the menu.'),
  );
  $form['options']['navbarElements']['footer']['title'] = array(
    '#title' => t('title'),
    '#type' => 'textfield',
    '#default_value' => isset($options['navbarElements']['footer']['title']) ? $options['navbarElements']['footer']['title'] : 'Copyright ©' . date('Y'),
    '#description' => t('The text of the footer if the submenu has no footer text.'),
  );
  $form['options']['navbarElements']['footer']['height'] = array(
    '#title' => t('height'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5)),
    '#default_value' => isset($options['navbarElements']['footer']['height']) ? $options['navbarElements']['footer']['height'] : 1,
    '#description' => t('The size of the navbar, will be multiplied by 40px.'),
  );

  $form['options']['offCanvas'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('offCanvas (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/off-canvas.html')),
  );
  $form['options']['offCanvas']['enabled'] = array(
    '#title' => t('flag'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['offCanvas']) && isset($options['offCanvas']['enabled']) && $options['offCanvas']['enabled'] ? 'true' : 'false',
    '#description' => t('Whether or not to enable the add-on.'),
  );
  $form['options']['offCanvas']['blockUI'] = array(
    '#title' => t('blockUI'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['offCanvas']['blockUI']) && $options['offCanvas']['blockUI'] ? 'true' : 'false',
    '#description' => t('Whether or not to block the user from using the page while the menu is opened. If set to "modal", clicking outside the menu does not close it.'),
  );
  $form['options']['offCanvas']['moveBackground'] = array(
    '#title' => t('moveBackground'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['offCanvas']['moveBackground']) && $options['offCanvas']['moveBackground'] ? 'true' : 'false',
    '#description' => t('Whether or not the page should inherit the background of the body when the menu opens.'),
  );
  $form['options']['offCanvas']['position'] = array(
    '#title' => t('position'),
    '#type' => 'textfield',
    '#default_value' => $mmenu['position'],
    '#description' => t('The position of the menu relative to the page. Possible values: "top", "right", "bottom" or "left".'),
    '#attributes' => array(
      'readonly' => 'readonly',
    ),
  );
  $form['options']['offCanvas']['zposition'] = array(
    '#title' => t('zposition'),
    '#type' => 'textfield',
    '#default_value' => isset($options['offCanvas']['zposition']) ? $options['offCanvas']['zposition'] : 'back',
    '#description' => t('The z-position of the menu relative to the page. Possible values: "back", "front" or "next".'),
  );

  $form['options']['screenReader'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Screen reader (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['options']['screenReader']['aria'] = array(
    '#title' => t('aria'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['screenReader']) && isset($options['screenReader']['aria']) && $options['screenReader']['aria'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically add and update the aria-hidden and aria-haspopup attributes.'),
  );
  $form['options']['screenReader']['text'] = array(
    '#title' => t('text'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['screenReader']) && isset($options['screenReader']['text']) && $options['screenReader']['text'] ? 'true' : 'false',
    '#description' => t('Whether or not to add a "screen reader only" text for anchors that normally don\'t have text.'),
  );

  $form['options']['scrollBugFix'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Scroll bug fix (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/scroll-bug-fix.html')),
  );
  $form['options']['scrollBugFix']['fix'] = array(
    '#title' => t('fix'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['scrollBugFix']['fix']) && $options['scrollBugFix']['fix'] ? 'true' : 'false',
    '#description' => t('Whether or not to fix the scroll bug on touchscreens.'),
  );

  $form['options']['searchfield'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('searchfield (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/searchfield.html')),
  );
  $form['options']['searchfield']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['searchfield']['add']) && $options['searchfield']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically prepend a search field to the menu.'),
  );
  $form['options']['searchfield']['addTo'] = array(
    '#title' => t('addTo'),
    '#type' => 'textfield',
    '#default_value' => isset($options['searchfield']['addTo']) ? $options['searchfield']['addTo'] : t('panels'),
    '#description' => t('Where to add the searchfield(s). Possible values: "menu", "panels" and a valid jQuery selector.'),
  );
  $form['options']['searchfield']['search'] = array(
    '#title' => t('search'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['searchfield']['search']) && $options['searchfield']['search'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically search when typing.'),
  );
  $form['options']['searchfield']['placeholder'] = array(
    '#title' => t('title'),
    '#type' => 'textfield',
    '#default_value' => isset($options['searchfield']['placeholder']) ? $options['searchfield']['placeholder'] : t('Search'),
    '#description' => t('The placeholder text in the search field.'),
  );
  $form['options']['searchfield']['noResults'] = array(
    '#title' => t('noResults'),
    '#type' => 'textfield',
    '#default_value' => isset($options['searchfield']['noResults']) ? $options['searchfield']['noResults'] : t('No results found.'),
    '#description' => t('The text to show when no results are found. If false no message will be shown.'),
  );
  $form['options']['searchfield']['showLinksOnly'] = array(
    '#title' => t('showLinksOnly'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['searchfield']['showLinksOnly']) && $options['searchfield']['showLinksOnly'] ? 'true' : 'false',
    '#description' => t('Whether or not to only show links (A elements) in the search results. If false, also SPAN elements will be shown.'),
  );
  $form['options']['searchfield']['showTextItems'] = array(
    '#title' => t('showTextItems'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['searchfield']['showTextItems']) && $options['searchfield']['showTextItems'] ? 'true' : 'false',
    '#description' => t('Whether or not to show list items without an anchor (an A element) in the results. If true, list items with a SPAN elements will also be shown.'),
  );
  $form['options']['searchfield']['resultsPanel'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('resultsPanel'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['options']['searchfield']['resultsPanel']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['searchfield']['resultsPanel']['add']) && $options['searchfield']['resultsPanel']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to add a new panel for showing all results. If false, results will be shown in their actual panel.'),
  );
  $form['options']['searchfield']['resultsPanel']['dividers'] = array(
    '#title' => t('dividers'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['searchfield']['resultsPanel']['dividers']) && $options['searchfield']['resultsPanel']['dividers'] ? 'true' : 'false',
    '#description' => t('Whether or not to add a divider to divide the results per panel.'),
  );
  $form['options']['searchfield']['resultsPanel']['title'] = array(
    '#title' => t('title'),
    '#type' => 'textfield',
    '#default_value' => isset($options['options']['searchfield']['resultsPanel']['title']) ? $options['options']['searchfield']['resultsPanel']['title'] : t('Search results'),
    '#description' => t('The title above the results panel.'),
  );

  $form['options']['sectionIndexer'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Section indexer (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/section-indexer.html')),
  );
  $form['options']['sectionIndexer']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['sectionIndexer']['add']) && $options['sectionIndexer']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically append a section indexer to the menu.'),
  );
  $form['options']['sectionIndexer']['addTo'] = array(
    '#title' => t('addTo'),
    '#type' => 'textfield',
    '#default_value' => isset($options['sectionIndexer']['addTo']) ? $options['sectionIndexer']['addTo'] : t('panels'),
    '#description' => t('Where to add the section indexer(s). Possible values: "panels" and a valid jQuery selector.'),
  );

  $form['options']['setSelected'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Set selected (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/set-selected.html')),
  );
  $form['options']['setSelected']['current'] = array(
    '#title' => t('current'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['setSelected']['current']) && $options['setSelected']['current'] ? 'true' : 'false',
    '#description' => t('Whether or not to keep the current menu item selected.'),
  );
  $form['options']['setSelected']['hover'] = array(
    '#title' => t('hover'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['setSelected']['hover']) && $options['setSelected']['hover'] ? 'true' : 'false',
    '#description' => t('Whether or not to make list items visibly "selected" onMouseOver.'),
  );
  $form['options']['setSelected']['parent'] = array(
    '#title' => t('parent'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['setSelected']['parent']) && $options['setSelected']['parent'] ? 'true' : 'false',
    '#description' => t('Whether or not to make list items visibly "selected" when opening its subpanel.'),
  );

  $form['options']['screenReader'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Screen reader (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['options']['screenReader']['aria'] = array(
    '#title' => t('aria'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['screenReader']) && isset($options['screenReader']['aria']) && $options['screenReader']['aria'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically add and update the aria-hidden and aria-haspopup attributes.'),
  );
  $form['options']['screenReader']['text'] = array(
    '#title' => t('text'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['screenReader']) && isset($options['screenReader']['text']) && $options['screenReader']['text'] ? 'true' : 'false',
    '#description' => t('Whether or not to add a "screen reader only" text for anchors that normally don\'t have text.'),
  );

  $form['options']['toggles'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('toggles (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/toggles.html')),
  );
  $form['options']['toggles']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no options.'),
  );

  $form['options']['clickOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('clickOpen (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['options']['clickOpen']['open'] = array(
    '#title' => t('open'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['clickOpen']['open']) && $options['clickOpen']['open'] ? 'true' : 'false',
    '#description' => t('Whether or not the menu should open when clicking on the selector.'),
  );
  $form['options']['clickOpen']['selector'] = array(
    '#title' => t('selector'),
    '#type' => 'textfield',
    '#default_value' => isset($options['clickOpen']['selector']) ? $options['clickOpen']['selector'] : '',
    '#description' => t("Clicks the selector to open the mmenu. e.g. #logo or a[id=logo]"),
  );

  $configurations = $mmenu['configurations'];
  $form['configurations'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Mmenu configurations'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('For more information about the configurations, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/options/configuration.html')),
  );
  $form['configurations']['clone'] = array(
    '#title' => t('clone'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($configurations['clone']) && $configurations['clone'] ? 'true' : 'false',
    '#description' => t('Whether or not to clone the menu before prepending it to the BODY. If true, the ID on the menu and every ID inside it will be prepended with "mm-" to prevent using double IDs.'),
  );

  $form['configurations']['openingInterval'] = array(
    '#title' => t('openingInterval'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['openingInterval']) ? $configurations['openingInterval'] : 25,
    '#description' => t('The number of milliseconds between opening/closing the menu and panels, needed to force CSS transitions.'),
  );
  $form['configurations']['panelNodetype'] = array(
    '#title' => t('panelNodetype'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['panelNodetype']) ? $configurations['panelNodetype'] : 'div, ul, ol',
    '#description' => t('The node-type of panels.'),
  );
  $form['configurations']['transitionDuration'] = array(
    '#title' => t('transitionDuration'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['transitionDuration']) ? $configurations['transitionDuration'] : 400,
    '#description' => t('The number of milliseconds used in the CSS transitions.'),
  );

  $form['configurations']['classNames'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('classNames'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['label'] = array(
    '#title' => t('label'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['label']) ? $configurations['classNames']['label'] : 'Label',
    '#description' => t('The classname on a LI that should be displayed as a label.'),
  );
  $form['configurations']['classNames']['panel'] = array(
    '#title' => t('panel'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['panel']) ? $configurations['classNames']['panel'] : 'Panel',
    '#description' => t('The classname on an element (for example a DIV) that should be considered to be a panel. Only applies if the "isMenu" option is set to false.'),
  );
  $form['configurations']['classNames']['selected'] = array(
    '#title' => t('selected'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['selected']) ? $configurations['classNames']['selected'] : 'Selected',
    '#description' => t('The classname on the LI that should be displayed as selected.'),
  );

  $form['configurations']['counters'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('counters (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/counters.html')),
  );
  $form['configurations']['counters']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options.'),
  );
  $form['configurations']['classNames']['counters'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('counters'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['counters']['counter'] = array(
    '#title' => t('counter'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['counters']['counter']) ? $configurations['classNames']['counters']['counter'] : 'Counter',
  );

  $form['configurations']['dividers'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dividers (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/dividers.html')),
  );
  $form['configurations']['dividers']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options.'),
  );
  $form['configurations']['classNames']['dividers'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dividers'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['dividers']['collapsed'] = array(
    '#title' => t('collapsed'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['dividers']['collapsed']) ? $configurations['classNames']['dividers']['collapsed'] : 'Collapsed',
  );

  $form['configurations']['dragOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dragOpen (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/drag-open.html')),
  );
  $form['configurations']['classNames']['dragOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dragOpen'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['dragOpen']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no classNames options.'),
  );
  $form['configurations']['dragOpen']['width'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('width'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['dragOpen']['width']['perc'] = array(
    '#title' => t('perc'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['width']['perc']) ? $configurations['dragOpen']['width']['perc'] : 0.8,
    '#description' => t('The width of the menu as a percentage. From 0.0 (fully hidden) to 1.0 (fully opened).'),
  );
  $form['configurations']['dragOpen']['width']['min'] = array(
    '#title' => t('min'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['width']['min']) ? $configurations['dragOpen']['width']['min'] : 140,
    '#description' => t('The minimum width of the menu.'),
  );
  $form['configurations']['dragOpen']['width']['max'] = array(
    '#title' => t('max'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['width']['max']) ? $configurations['dragOpen']['width']['max'] : 440,
    '#description' => t('The maximum width of the menu.'),
  );
  $form['configurations']['dragOpen']['height'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('height'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['dragOpen']['height']['perc'] = array(
    '#title' => t('perc'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['height']['perc']) ? $configurations['dragOpen']['height']['perc'] : 0.8,
    '#description' => t('The height of the menu as a percentage. From 0.0 (fully hidden) to 1.0 (fully opened).'),
  );
  $form['configurations']['dragOpen']['height']['min'] = array(
    '#title' => t('min'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['height']['min']) ? $configurations['dragOpen']['height']['min'] : 140,
    '#description' => t('The minimum height of the menu.'),
  );
  $form['configurations']['dragOpen']['height']['max'] = array(
    '#title' => t('max'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['height']['max']) ? $configurations['dragOpen']['height']['max'] : 440,
    '#description' => t('The maximum height of the menu.'),
  );

  $form['configurations']['dropdown'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dropdown (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/dropdown.html')),
  );
  $form['configurations']['dropdown']['offset'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('offset'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['dropdown']['offset']['button'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('button'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['dropdown']['offset']['button']['x'] = array(
    '#title' => t('x'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dropdown']['offset']['button']['x']) ? $configurations['dropdown']['offset']['button']['x'] : -10,
    '#description' => t('The horizontal offset for the menu relative to the button.'),
  );
  $form['configurations']['dropdown']['offset']['button']['y'] = array(
    '#title' => t('y'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dropdown']['offset']['button']['y']) ? $configurations['dropdown']['offset']['button']['y'] : 10,
    '#description' => t('The horizontal offset for the menu relative to the button.'),
  );
  $form['configurations']['dropdown']['offset']['viewport'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('viewport'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['dropdown']['offset']['viewport']['x'] = array(
    '#title' => t('x'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dropdown']['offset']['viewport']['x']) ? $configurations['dropdown']['offset']['viewport']['x'] : 20,
    '#description' => t('The horizontal offset for the menu relative to the viewport.'),
  );
  $form['configurations']['dropdown']['offset']['viewport']['y'] = array(
    '#title' => t('y'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dropdown']['offset']['viewport']['y']) ? $configurations['dropdown']['offset']['viewport']['y'] : 20,
    '#description' => t('The vertical offset for the menu relative to the viewport.'),
  );
  $form['configurations']['dropdown']['height'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('height'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['dropdown']['height']['max'] = array(
    '#title' => t('max'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dropdown']['height']['max']) ? $configurations['dropdown']['height']['max'] : 880,
    '#description' => t('The maximum height of the menu.'),
  );
  $form['configurations']['dropdown']['width'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('width'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['dropdown']['width']['max'] = array(
    '#title' => t('max'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dropdown']['width']['max']) ? $configurations['dropdown']['width']['max'] : 440,
    '#description' => t('The maximum width of the menu.'),
  );

  $form['configurations']['fixedElements'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('fixedElements (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/fixed-elements.html')),
  );
  $form['configurations']['fixedElements']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options, it does add an object to the classNames option.'),
  );
  $form['configurations']['classNames']['fixedElements'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('fixedElements'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['fixedElements']['fixed'] = array(
    '#title' => t('fixedTop'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['fixedElements']['fixed']) ? $configurations['classNames']['fixedElements']['fixed'] : 'header',
  );

  $form['configurations']['navbars'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('navbars (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/navbars.html')),
  );
  $form['configurations']['navbars']['breadcrumbSeparator'] = array(
    '#type' => 'textfield',
    '#title' => t('breadcrumbSeparator'),
    '#default_value' => isset($configurations['navbars']['fixedElements']['breadcrumbSeparator']) ? $configurations['navbars']['fixedElements']['breadcrumbSeparator'] : '/',
    '#description' => t('The separator between two breadcrumbs.'),
  );
  $form['configurations']['classNames']['navbars'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('navbars'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['navbars']['panelTitle'] = array(
    '#title' => t('panelTitle'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['navbars']['panelTitle']) ? $configurations['classNames']['navbars']['panelTitle'] : 'Title',
  );
  $form['configurations']['classNames']['navbars']['panelNext'] = array(
    '#title' => t('panelNext'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['navbars']['panelNext']) ? $configurations['classNames']['navbars']['panelNext'] : 'Next',
  );
  $form['configurations']['classNames']['navbars']['panelPrev'] = array(
    '#title' => t('panelPrev'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['navbars']['panelPrev']) ? $configurations['classNames']['navbars']['panelPrev'] : 'Prev',
  );

  $form['configurations']['offCanvas'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('offCanvas (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/off-canvas.html')),
  );
  $form['configurations']['classNames']['offCanvas'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('offCanvas'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['offCanvas']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no classNames options.'),
  );
  $form['configurations']['offCanvas']['menuInjectMethod'] = array(
    '#title' => t('menuInjectMethod'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['offCanvas']['menuInjectMethod']) ? $configurations['offCanvas']['menuInjectMethod'] : 'prepend',
    '#description' => t('How to inject the menu to the DOM. Possible values: "prepend" or "append".'),
  );
  $form['configurations']['offCanvas']['menuWrapperSelector'] = array(
    '#title' => t('menuWrapperSelector'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['offCanvas']['menuWrapperSelector']) ? $configurations['offCanvas']['menuWrapperSelector'] : 'body',
    '#description' => t('jQuery selector for the node the menu should be injected in.'),
  );
  $form['configurations']['offCanvas']['pageNodetype'] = array(
    '#title' => t('pageNodetype'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['offCanvas']['pageNodetype']) ? $configurations['offCanvas']['pageNodetype'] : 'div',
    '#description' => t('The node-type of the page.'),
  );
  $form['configurations']['offCanvas']['pageSelector'] = array(
    '#title' => t('pageSelector'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['offCanvas']['pageSelector']) ? $configurations['offCanvas']['pageSelector'] : 'body > div',
    '#description' => t('jQuery selector for the page. e.g. "body > " + offCanvas.pageNodetype'),
  );
  $form['configurations']['offCanvas']['wrapPageIfNeeded'] = array(
    '#title' => t('wrapPageIfNeeded'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($configurations['offCanvas']['wrapPageIfNeeded']) && $configurations['offCanvas']['wrapPageIfNeeded'] ? 'true' : 'false',
    '#description' => t('Whether or not multiple nodes targeted by the pageSelector should be wrapped in a single node.'),
  );

  $form['configurations']['screenReader'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('screenReader (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/screen-reader.html')),
  );
  $form['configurations']['screenReader']['text'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('text'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['screenReader']['text']['closeMenu'] = array(
    '#type' => 'textfield',
    '#title' => t('closeMenu'),
    '#default_value' => isset($configurations['configurations']['screenReader']['text']['closeMenu']) ? $configurations['configurations']['screenReader']['text']['closeMenu'] : 'Close menu',
    '#description' => t('The screen reader text for anchors that close the menu.'),
  );
  $form['configurations']['screenReader']['text']['closeSubmenu'] = array(
    '#type' => 'textfield',
    '#title' => t('closeSubmenu'),
    '#default_value' => isset($configurations['configurations']['screenReader']['text']['closeSubmenu']) ? $configurations['configurations']['screenReader']['text']['closeSubmenu'] : 'Close submenu',
    '#description' => t('The screen reader text for anchors that close a submenu.'),
  );
  $form['configurations']['screenReader']['text']['openSubmenu'] = array(
    '#type' => 'textfield',
    '#title' => t('openSubmenu'),
    '#default_value' => isset($configurations['configurations']['screenReader']['text']['openSubmenu']) ? $configurations['configurations']['screenReader']['text']['openSubmenu'] : 'Open submenu',
    '#description' => t('The screen reader text for anchors that open a submenu.'),
  );
  $form['configurations']['screenReader']['text']['toggleSubmenu'] = array(
    '#type' => 'textfield',
    '#title' => t('toggleSubmenu'),
    '#default_value' => isset($configurations['configurations']['screenReader']['text']['toggleSubmenu']) ? $configurations['configurations']['screenReader']['text']['toggleSubmenu'] : 'Toggle submenu',
    '#description' => t('The screen reader text for anchors that toggle a submenu.'),
  );

  $form['configurations']['searchfield'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('searchfield (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/searchfield.html')),
  );
  $form['configurations']['searchfield']['clear'] = array(
    '#title' => t('clear'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($configurations['searchfield']['clear']) && $configurations['searchfield']['clear'] ? 'true' : 'false',
    '#description' => t('Whether or not to add a clear button to the searchfield.'),
  );
  $form['configurations']['searchfield']['submit'] = array(
    '#title' => t('submit'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($configurations['searchfield']['submit']) && $configurations['searchfield']['submit'] ? 'true' : 'false',
    '#description' => t('Whether or not to add a submit button to the searchfield. Requires the searchfield.form configuration option to be set.'),
  );

  $form['configurations']['toggles'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('toggles (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/toggles.html')),
  );
  $form['configurations']['toggles']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options, it does add an object to the classNames option.'),
  );
  $form['configurations']['classNames']['toggles'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('toggles'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['toggles']['toggle'] = array(
    '#title' => t('toggle'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['toggles']['toggle']) ? $configurations['classNames']['toggles']['toggle'] : 'Toggle',
  );
  $form['configurations']['classNames']['toggles']['check'] = array(
    '#title' => t('check'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['toggles']['check']) ? $configurations['classNames']['toggles']['check'] : 'Check',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 0,
  );
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#weight' => 1,
  );

  return $form;
}

/**
 * Form submission handler for mmenu_admin_settings_form().
 */
function mmenu_admin_settings_form_submit($form_id, &$form_state) {
  $values = $form_state['values'];

  switch ($values['op']) {
    case t('Save'):
      $blocks = array();

      // Updates the blocks.
      foreach ($values['blocks'] as $k => $block) {
        if (!empty($block['module_delta'])) {
          $v = explode('|', $block['module_delta']);
          $blocks[$k] = $block;
          $blocks[$k] += array(
            'module' => $v[0],
            'delta' => $v[1],
          );
        }
      }

      // Add extension options.
      $extensions = mmenu_extension_list();
      $extension_values = array();
      foreach ($extensions as $extension) {
        if (!empty($values['options'][$extension['name']]['options'])) {
          if (is_array($values['options'][$extension['name']]['options'])) {
            $options = array_filter($values['options'][$extension['name']]['options']);
            $extension_values = array_merge($extension_values, array_values($options));
          }
          else {
            $extension_values[] = $values['options'][$extension['name']]['options'];
          }
        }
      }
      $values['options']['extensions'] = $extension_values;

      $navbars = array();
      // Add header.
      if ($values['options']['navbarElements']['header']['add'] === 'true') {
        $navbars[] = array(
          'position' => 'top',
          'content' => array('prev', 'title', 'next'),
          'height' => $values['options']['navbarElements']['header']['height'],
        );
        $values['options']['navbar']['title'] = $values['options']['navbarElements']['header']['title'];
      }
      else {
        $navbars[] = array(
          'position' => 'top',
          'content' => array('prev', 'next'),
        );
      }

      // Add footer.
      if ($values['options']['navbarElements']['footer']['add'] === 'true') {
        $navbars[] = array(
          'position' => 'bottom',
          'content' => ['<div class="mm-title">' . $values['options']['navbarElements']['footer']['title']  . '</div>'],
          'height' => $values['options']['navbarElements']['footer']['height'],
        );
      }
      $values['options']['navbars'] = $navbars;

      $mmenu = array(
        'enabled' => $values['general']['enabled'],
        'title' => $values['general']['title'],
        'name' => $values['general']['name'],
        'blocks' => $blocks,
        'options' => mmenu_convert_settings('options', $values['options']),
        'configurations' => mmenu_convert_settings('configurations', $values['configurations']),
      );
      variable_set('mmenu_item_' . $values['general']['name'], $mmenu);

      // Clears mmenus cache.
      cache_clear_all('mmenus', 'cache');

      drupal_set_message(t('The settings have been saved.'));
      break;

    case t('Reset'):
      // Deletes the mmenu settings from database.
      variable_del('mmenu_item_' . $values['general']['name']);
      // Clears mmenus cache.
      cache_clear_all('mmenus', 'cache');
      drupal_set_message(t('The settings have been reset.'));
      break;
  }
}
