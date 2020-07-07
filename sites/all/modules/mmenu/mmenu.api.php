<?php

/**
 * @file
 * Hooks provided by mmenu module.
 */

/**
 * Allows modules to add more mmenus.
 */
function hook_mmenu() {
  $module_path = drupal_get_path('module', 'mmenu');

  return array(
    'mmenu_left' => array(
      'enabled' => TRUE,
      'enabled_callback' => array(
        'php' => array(
          'mmenu_enabled_callback',
        ),
        'js' => array(
          'mmenu_enabled_callback',
        ),
      ),
      'name' => 'mmenu_left',
      'title' => t('Left menu'),
      'blocks' => array(
        array(
          'title' => t('Main menu'),
          'module' => 'system',
          'delta' => 'main-menu',
          'collapsed' => FALSE,
          'wrap' => FALSE,
        ),
        array(
          'title' => t('Management'),
          'module' => 'system',
          'delta' => 'management',
          'collapsed' => FALSE,
          'wrap' => FALSE,
          'menu_parameters' => array(
            'min_depth' => 2,
          ),
        ),
      ),
      'options' => array(
        'position' => 'left',
        'classes' => 'mm-light',
      ),
      'configurations' => array(),
      // Adds your own CSS or JS handlers if you want.
      'custom' => array(
        'css' => array(
          $module_path . '/examples/css/mmenu_example.custom.css',
        ),
        'js' => array(
          $module_path . '/examples/js/mmenu_example.custom.js',
        ),
      ),
    ),
  );
}

/**
 * Allows modules to alter mmenu settings.
 */
function hook_mmenu_alter(&$mmenus) {
  $mmenus['mmenu_left']['enabled'] = FALSE;
}

/**
 * Allows modules to add more mmenu extensions.
 *  Refer to: http://mmenu.frebsite.nl/documentation/extensions.
 */
function hook_mmenu_extension() {
  $module_path = drupal_get_path('module', 'mmenu');
  return array(
    'themes' => array(
      'options' => array(
        'theme-custom' => array(
          'name' => 'theme-custom',
          'title' => t('Custom Theme'),
          'css' => array(
            $module_path . '/extensions/themes/theme-custom/styles/theme-custom.css',
          ),
        ),
      ),
    ),
  );
}

/**
 * Allows modules to alter mmenu extension settings.
 */
function hook_mmenu_extension_alter(&$extensions) {
  $module_path = drupal_get_path('module', 'mmenu');
  $extensions['themes']['options']['theme-custom']['css'] = array(
    $module_path . '/extensions/themes/theme-custom/styles/theme-custom-alter.css',
  );
}

/**
 * Allows modules to add more mmenu icon classes.
 */
function hook_mmenu_icon() {
  $icons = array(
    'path' => array(
      'home' => 'icon-home',
      'about' => 'icon-office',
      'contact' => 'icon-envelope',
    ),
    'block' => array(
      array(
        'module' => 'system',
        'delta' => 'main-menu',
        'icon_class' => 'icon-enter',
      ),
    ),
  );
  return $icons;
}

/**
 * Allows modules to alter mmenu icon class settings.
 */
function hook_mmenu_icon_alter(&$icons) {
  $icons['path']['home'] = 'icon-home1';
}
