<?php
 /**
 * @file
 * Helper functiions for Registration Iframe.
 */

use MailjetTools\MailjetApi;

/**
 * Regisration iframe block code.
  */
function mailjet_register_iframe() {
  global $base_url;
  global $language;

  $lang_codes_map = array(
    'en' => 'en_US',
    'fr' => 'fr_FR',
    'de' => 'de_DE',
    'es' => 'es_ES',
  );

  $default_lang = 'en';
  $locale = isset($lang_codes_map[$language->language]) ? $lang_codes_map[$language->language] : $lang_codes_map[$default_lang];

//  $default_url = IFRAME_URL . "reseller/signup?r=commerceguys&sp=display&aff=drupal-3.0&locale={$locale}&cb=";
  $default_url = IFRAME_URL . "reseller/signup?r=commerceguys&sp=display&aff=drupal-3.0&locale={$locale}";
  $callbackurl = urlencode($base_url . '?q=/admin/config/system/mailjet/api/register/alter_callback');
  return '<iframe src="' . $default_url . '" width="100%" height="1080px" frameborder="0"></iframe>';
//  return '<iframe src="' . $default_url . $callbackurl . '" width="100%" height="1080px" frameborder="0"></iframe>';
}

/**
 * Check for user api.
 */
function mailjet_plans_iframe() {
  if (!variable_get('mailjet_username') && !variable_get('mailjet_password')) {
    drupal_set_message(t('You need to add your Mailjet API details before you can continue'), 'warning');
    drupal_goto('admin/config/system/mailjet/api');
  }
    global $language;

    $lang_codes_map = array('en' => 'en_US', 'fr' => 'fr_FR', 'de' => 'de_DE', 'es' => 'es_ES');
    $default_lang = 'en';
    $locale = isset($lang_codes_map[$language->language]) ? $lang_codes_map[$language->language] : $lang_codes_map[$default_lang];

    //$callbackurl = 't=' . $token . '&show_menu=none&sp=display&locale=' . $locale;
//    $callbackurl = 'r=commerceguys&t=' . $token . '&show_menu=none&sp=display&locale=' . $locale;
    $callbackurl = '';
    $default_url = IFRAME_URL . "reseller/pricing/?";

    return '<iframe src="' . $default_url . $callbackurl . '" width="100%" height="1080px" frameborder="0"></iframe>';
 }
