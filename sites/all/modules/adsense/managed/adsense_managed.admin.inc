<?php

/**
 * @file
 * Contains the administrative functions of the adsense_managed module.
 *
 * This file is included by the adsense_managed module, and includes the
 * settings form.
 */

/**
 * Menu callback for the adsense_managed module settings form.
 *
 * @ingroup forms
 */
function adsense_managed_settings() {
  module_load_include('inc', 'adsense_managed', 'help/adsense_managed.help');

  $form['help'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#title'       => t('Help and instructions'),
  );

  $form['help']['help'] = array(
    '#markup' => adsense_managed_help_text(),
  );

  $form['code'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Code type'),
  );

  $form['code']['adsense_managed_async'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use asynchronous ad code?'),
    '#default_value' => variable_get('adsense_managed_async', ADSENSE_MANAGED_ASYNC_DEFAULT),
    '#description' => t('This will enable the asynchronous ad code type. [!moreinfo]', array('!moreinfo' => l(t('More information'), 'https://support.google.com/adsense/answer/3221666'))),
  );

  $form['page_level'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Auto ads'),
  );

  $form['page_level']['adsense_managed_page_level_ads_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable auto ads?'),
    '#default_value' => variable_get('adsense_managed_page_level_ads_enabled', ADSENSE_MANAGED_PAGE_LEVEL_ADS_ENABLED),
    '#description' => t('This will enable Auto ads. [!moreinfo]', array('!moreinfo' => l(t('More information'), 'https://support.google.com/adsense/answer/7478040'))),
  );

  $form['blocks'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('AdSense Blocks'),
  );

  $form['blocks']['adsense_managed_number_blocks'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of blocks'),
    '#default_value' => variable_get('adsense_managed_number_blocks', ADSENSE_MANAGED_NUMBER_BLOCKS_DEFAULT),
    '#size' => 2,
    '#maxlength' => 2,
  );

  $form['#validate'][] = '_adsense_managed_settings_validate';

  return system_settings_form($form);
}

/**
 * Validate adsense_managed_settings form.
 */
function _adsense_managed_settings_validate($form, &$form_state) {
  $number_blocks = $form_state['values']['adsense_managed_number_blocks'];
  if ($number_blocks < 0) {
    form_set_error('adsense_managed_number_blocks', t("Number of blocks can't be a negative number"));
  }
}
