<?php

 /**
 * @file
 * Helper function for generate Campaign Iframes.
 */

use MailjetTools\MailjetApi;
/**
  * Iframe for the campaigns.
  */
function mailjet_campaign_iframe() {
  if (!variable_get('mailjet_username') && !variable_get('mailjet_password')) {
    drupal_set_message(t('You need to add your Mailjet API details before you can continue'), 'warning');
    drupal_goto('admin/config/system/mailjet');
  }

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

    $callbackurl = urlencode($base_url . '?q=/admin/mailjet/campaign/alter_callback');

    $mailjetIframe = MailjetApi::getMailjetIframe(variable_get('mailjet_username'), variable_get('mailjet_password'));
    $mailjetIframe->setInitialPage(\MailjetIframe\MailjetIframe::PAGE_CAMPAIGNS);
    $mailjetIframe->setCallback($callbackurl);
    $mailjetIframe->setLocale($locale);

    return '<div id="iframe-main-container" class="iframe-main-container" style="width:100%; height: 1300px;">' . $mailjetIframe->getHtml() . '</div>';

}
