<?php

/**
 * PropellerAds config form.
 */
function propellerads_admin_form($form, &$form_state) {
  $form['#type'] = 'POST';
  $form['propellerads_verification_code'] = array(
    '#type' => 'textfield',
    '#title' => t('PropellerAds site verification code'),
    '#description' => t('Create a new site !link and add here the verification code (only the attribute "content" of the "meta" tag).', array('!link' => l('PropellerAds', 'http://propellerads.com'))),
    '#default_value' => variable_get('propellerads_verification_code'),
  );
  $form['propellerads_channel_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Channel ID'),
    '#description' => t('Create a new site at !link <strong>Dashboard > Menu > Sites</strong> and fill the Channel ID here.', array('!link' => l('PropellerAds', 'http://propellerads.com'))),
    '#default_value' => variable_get('propellerads_channel_id'),
  );
  return system_settings_form($form);
}
