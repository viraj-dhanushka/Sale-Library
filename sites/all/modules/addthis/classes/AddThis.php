<?php
/**
 * @file
 * An AddThis-class.
 */

class AddThis {

  const BLOCK_NAME = 'addthis_block';
  const DEFAULT_CUSTOM_CONFIGURATION_CODE = 'var addthis_config = {}';
  const DEFAULT_FORMATTER = 'addthis_default_formatter';
  const DEFAULT_NUMBER_OF_PREFERRED_SERVICES = 4;
  const FIELD_TYPE = 'addthis';
  const MODULE_NAME = 'addthis';
  const PERMISSION_ADMINISTER_ADDTHIS = 'administer addthis';
  const PERMISSION_ADMINISTER_ADVANCED_ADDTHIS = 'administer advanced addthis';
  const STYLE_KEY = 'addthis_style';
  const WIDGET_TYPE = 'addthis_button_widget';

  // AddThis attribute and parameter names (as defined in AddThis APIs).
  const PROFILE_ID_QUERY_PARAMETER = 'pubid';
  const TITLE_ATTRIBUTE = 'addthis:title';
  const URL_ATTRIBUTE = 'addthis:url';

  // Persistent variable keys.
  const ADDRESSBOOK_ENABLED_KEY = 'addthis_addressbook_enabled';
  const BLOCK_WIDGET_TYPE_KEY = 'addthis_block_widget_type';
  const BLOCK_WIDGET_SETTINGS_KEY = 'addthis_block_widget_settings';
  const BOOKMARK_URL_KEY = 'addthis_bookmark_url';
  const CLICKBACK_TRACKING_ENABLED_KEY = 'addthis_clickback_tracking_enabled';
  const CLICK_TO_OPEN_COMPACT_MENU_ENABLED_KEY = 'addthis_click_to_open_compact_menu_enabled';
  const CO_BRAND_KEY = 'addthis_co_brand';
  const COMPLIANT_508_KEY = 'addthis_508_compliant';
  const CUSTOM_CONFIGURATION_CODE_ENABLED_KEY = 'addthis_custom_configuration_code_enabled';
  const CUSTOM_CONFIGURATION_CODE_KEY = 'addthis_custom_configuration_code';
  const ENABLED_SERVICES_KEY = 'addthis_enabled_services';
  const EXCLUDED_SERVICES_KEY = 'addthis_excluded_services';
  const GOOGLE_ANALYTICS_TRACKING_ENABLED_KEY = 'addthis_google_analytics_tracking_enabled';
  const GOOGLE_ANALYTICS_SOCIAL_TRACKING_ENABLED_KEY = 'addthis_google_analytics_social_tracking_enabled';
  const FACEBOOK_LIKE_COUNT_SUPPORT_ENABLED = 'addthis_facebook_like_count_support_enabled';
  const OPEN_WINDOWS_ENABLED_KEY = 'addthis_open_windows_enabled';
  const PROFILE_ID_KEY = 'addthis_profile_id';
  const SERVICES_CSS_URL_KEY = 'addthis_services_css_url';
  const SERVICES_JSON_URL_KEY = 'addthis_services_json_url';
  const STANDARD_CSS_ENABLED_KEY = 'addthis_standard_css_enabled';
  const UI_DELAY_KEY = 'addthis_ui_delay';
  const UI_HEADER_BACKGROUND_COLOR_KEY = 'addthis_ui_header_background_color';
  const UI_HEADER_COLOR_KEY = 'addthis_ui_header_color';
  const WIDGET_JS_URL_KEY = 'addthis_widget_js_url';
  const WIDGET_JS_LOAD_DOMREADY = 'addthis_widget_load_domready';
  const WIDGET_JS_LOAD_ASYNC = 'addthis_widget_load_async';
  const WIDGET_JS_INCLUDE = 'addthis_widget_include';

  // Twitter.
  const TWITTER_VIA_KEY = 'addthis_twitter_via';
  const TWITTER_VIA_DEFAULT = 'AddThis';
  const TWITTER_TEMPLATE_KEY = 'addthis_twitter_template';
  const TWITTER_TEMPLATE_DEFAULT = '{{title}} {{url}} via @AddThis';

  // External resources.
  const DEFAULT_BOOKMARK_URL = 'http://www.addthis.com/bookmark.php?v=300';
  const DEFAULT_SERVICES_CSS_URL = 'http://cache.addthiscdn.com/icons/v1/sprites/services.css';
  const DEFAULT_SERVICES_JSON_URL = 'http://cache.addthiscdn.com/services/v1/sharing.en.json';
  const DEFAULT_WIDGET_JS_URL = 'http://s7.addthis.com/js/300/addthis_widget.js';
  const DEFAULT_WIDGET_JS_LOAD_DOMREADY = TRUE;
  const DEFAULT_WIDGET_JS_LOAD_ASYNC = FALSE;

  // Type of inclusion.
  // 0 = don't include, 1 = pages no admin, 2 = on usages only.
  const DEFAULT_WIDGET_JS_INCLUDE = 2;
  const WIDGET_JS_INCLUDE_NONE = 0;
  const WIDGET_JS_INCLUDE_PAGE = 1;
  const WIDGET_JS_INCLUDE_USAGE = 2;


  // Internal resources.
  const ADMIN_CSS_FILE = 'addthis.admin.css';
  const ADMIN_INCLUDE_FILE = 'includes/addthis.admin.inc';

  // Widget types.
  const WIDGET_TYPE_DISABLED = 'addthis_disabled';

  // Styles.
  const CSS_32x32 = 'addthis_32x32_style';
  const CSS_16x16 = 'addthis_16x16_style';

  private static $instance;

  /* @var AddThisJson */
  private $json;

  /**
   * Get the singleton instance of the AddThis class.
   *
   * @return AddThis
   *   Instance of AddThis.
   */
  public static function getInstance() {

    if (!isset(self::$instance)) {
      $add_this = new AddThis();
      $add_this->setJson(new AddThisJson());
      self::$instance = $add_this;
    }

    return self::$instance;
  }

  /**
   * Set the json object.
   */
  public function setJson(AddThisJson $json) {
    $this->json = $json;
  }

  public function getDefaultFormatterTypes() {
    return array(
      self::WIDGET_TYPE_DISABLED => t('Disabled'),
    );
  }

  public function getDisplayTypes() {
    $displays = array();
    foreach ($display_impl = _addthis_field_info_formatter_field_type() as $key => $display) {
      $displays[$key] = t(check_plain($display['label']));
    }
    return $displays;
  }

  /*
   * Get markup for a given display type.
   *
   * When $options does not contain #entity, link to the current URL.
   * When $options does not contain #display, use default settings.
   */
  public function getDisplayMarkup($display, $options = array()) {
    if (empty($display)) {
      return array();
    }

    $formatters = _addthis_field_info_formatter_field_type();

    if (!array_key_exists($display, $formatters)) {
      return array();
    }

    // The display type exists. Now get it and get the markup.
    $display_information = $formatters[$display];

    // Theme function might only give a display name and
    // render on default implementation.
    if (!isset($options['#display']) ||
        (isset($options['#display']['type']) && $options['#display']['type'] != $display)) {

      $options['#display'] = isset($options['#display']) ? $options['#display'] : array();
      $options['#display'] = array_merge($options['#display'], $display_information);
      $options['#display']['type'] = $display;

    }

    // When #entity and #entity_type exist, use the entity's URL.
    if (isset($options['#entity']) && isset($options['#entity_type'])) {
      $uri = entity_uri($options['#entity_type'], $options['#entity']);
      $uri['options'] += array(
        'absolute' => TRUE,
      );

      // @todo Add a hook to alter the uri also based on fields from the
      // entity (such as custom share link). Pass $options and $uri. Return
      // a uri object to which we can reset it. Maybe use the alter structure.

      $options['#url'] = url($uri['path'], $uri['options']);
    }
    // @todo Hash the options array and cache the markup.
    // This will save all the extra calls to modules and alters.

    // Allow other modules to alter markup options.
    drupal_alter('addthis_markup_options', $options);

    $markup = array(
      '#display' => $options['#display'],
    );
    // Get all hook implementation to verify later if we can call it.
    $addthis_display_markup_implementations = module_implements('addthis_display_markup');

    // Look for a targeted implementation to call.
    // This should be the default implementation that is called.
    if (function_exists($display_information['module'] . '_addthis_display_markup__' . $display)) {
      $markup += call_user_func_array($display_information['module'] . '_addthis_display_markup__' . $display, array($options));
    }
    elseif (in_array($display_information['module'], $addthis_display_markup_implementations)) {
      $markup += module_invoke($display_information['module'], 'addthis_display_markup', $display, $options);
    }
    // Allow other modules to alter markup.
    drupal_alter('addthis_markup', $markup);
    return $markup;
  }

  public function getServices() {
    $rows = array();
    $services = $this->json->decode($this->getServicesJsonUrl());
    if (empty($services)) {
      drupal_set_message(t('AddThis services could not be loaded from @service_url', array('@service_url', $this->getServicesJsonUrl())), 'warning');
    }
    else {
      foreach ($services['data'] as $service) {
        $serviceCode = check_plain($service['code']);
        $serviceName = check_plain($service['name']);
        $rows[$serviceCode] = '<span class="addthis_service_icon icon_' . $serviceCode . '"></span> ' . $serviceName;
      }
    }
    return $rows;
  }

  public function getAddThisAttributesMarkup($options) {
    if (isset($options)) {
      $attributes = array();

      if (isset($options['#entity'])) {
        $attributes += $this->getAttributeTitle($options['#entity']);
      }
      $attributes += $this->getAttributeUrl($options);

      return $attributes;
    }
    return array();
  }

  /**
   * Get the type used for the block.
   */
  public function getBlockDisplayType() {
    return variable_get(self::BLOCK_WIDGET_TYPE_KEY, self::WIDGET_TYPE_DISABLED);
  }

  /**
   * Get the settings used by the block display.
   */
  public function getBlockDisplaySettings() {
    $settings = variable_get(self::BLOCK_WIDGET_SETTINGS_KEY, NULL);

    if ($settings == NULL && $this->getBlockDisplayType() != self::WIDGET_TYPE_DISABLED) {
      $settings = field_info_formatter_settings($this->getBlockDisplayType());
    }

    return $settings;
  }

  public function getProfileId() {
    return check_plain(variable_get(AddThis::PROFILE_ID_KEY));
  }

  public function getServicesCssUrl() {
    return check_url(variable_get(AddThis::SERVICES_CSS_URL_KEY, self::DEFAULT_SERVICES_CSS_URL));
  }

  public function getServicesJsonUrl() {
    return check_url(variable_get(AddThis::SERVICES_JSON_URL_KEY, self::DEFAULT_SERVICES_JSON_URL));
  }

  public function getEnabledServices() {
    return variable_get(self::ENABLED_SERVICES_KEY, array());
  }

  public function getExcludedServices() {
    return variable_get(self::EXCLUDED_SERVICES_KEY, array());
  }

  /**
   * Return the type of inclusion.
   *
   * @return string
   *   Retuns domready or async.
   */
  public function getWidgetJsInclude() {
    return variable_get(self::WIDGET_JS_INCLUDE, self::DEFAULT_WIDGET_JS_INCLUDE);
  }

  /**
   * Return if domready loading should be active.
   *
   * @return bool
   *   Returns TRUE if domready is enabled.
   */
  public function getWidgetJsDomReady() {
    return variable_get(self::WIDGET_JS_LOAD_DOMREADY, self::DEFAULT_WIDGET_JS_LOAD_DOMREADY);
  }

  /**
   * Return if async initialization should be active.
   *
   * @return bool
   *   Returns TRUE if async is enabled.
   */
  public function getWidgetJsAsync() {
    return variable_get(self::WIDGET_JS_LOAD_ASYNC, self::DEFAULT_WIDGET_JS_LOAD_ASYNC);
  }

  public function isClickToOpenCompactMenuEnabled() {
    return (boolean) variable_get(self::CLICK_TO_OPEN_COMPACT_MENU_ENABLED_KEY, FALSE);
  }

  public function isOpenWindowsEnabled() {
    return (boolean) variable_get(self::OPEN_WINDOWS_ENABLED_KEY, FALSE);
  }

  public function getUiDelay() {
    return (int) check_plain(variable_get(self::UI_DELAY_KEY));
  }

  public function getUiHeaderColor() {
    return check_plain(variable_get(self::UI_HEADER_COLOR_KEY));
  }

  public function getUiHeaderBackgroundColor() {
    return check_plain(variable_get(self::UI_HEADER_BACKGROUND_COLOR_KEY));
  }

  public function isStandardCssEnabled() {
    return (boolean) variable_get(self::STANDARD_CSS_ENABLED_KEY, TRUE);
  }

  public function getCustomConfigurationCode() {
    return variable_get(self::CUSTOM_CONFIGURATION_CODE_KEY, self::DEFAULT_CUSTOM_CONFIGURATION_CODE);
  }

  public function isCustomConfigurationCodeEnabled() {
    return (boolean) variable_get(self::CUSTOM_CONFIGURATION_CODE_ENABLED_KEY, FALSE);
  }

  public function getBaseWidgetJsUrl() {
    return check_url(variable_get(self::WIDGET_JS_URL_KEY, self::DEFAULT_WIDGET_JS_URL));
  }

  public function getBaseBookmarkUrl() {
    return check_url(variable_get(self::BOOKMARK_URL_KEY, self::DEFAULT_BOOKMARK_URL));
  }

  public function getCoBrand() {
    return variable_get(self::CO_BRAND_KEY, '');
  }

  public function get508Compliant() {
    return (boolean) variable_get(self::COMPLIANT_508_KEY, FALSE);
  }

  public function getTwitterVia() {
    return variable_get(self::TWITTER_VIA_KEY, self::TWITTER_VIA_DEFAULT);
  }

  public function getTwitterTemplate() {
    return variable_get(self::TWITTER_TEMPLATE_KEY, self::TWITTER_TEMPLATE_DEFAULT);
  }

  public function isClickbackTrackingEnabled() {
    return (boolean) variable_get(self::CLICKBACK_TRACKING_ENABLED_KEY, FALSE);
  }

  public function isAddressbookEnabled() {
    return (boolean) variable_get(self::ADDRESSBOOK_ENABLED_KEY, FALSE);
  }

  public function isGoogleAnalyticsTrackingEnabled() {
    return (boolean) variable_get(self::GOOGLE_ANALYTICS_TRACKING_ENABLED_KEY, FALSE);
  }

  public function isGoogleAnalyticsSocialTrackingEnabled() {
    return (boolean) variable_get(self::GOOGLE_ANALYTICS_SOCIAL_TRACKING_ENABLED_KEY, FALSE);
  }

  public function isFacebookLikeCountSupportEnabled() {
    return (boolean) variable_get(self::FACEBOOK_LIKE_COUNT_SUPPORT_ENABLED, TRUE);
  }

  public function addStylesheets() {
    drupal_add_css($this->getServicesCssUrl(), 'external');
    drupal_add_css($this->getAdminCssFilePath(), 'file');
  }

  public function getFullBookmarkUrl() {
    return $this->getBaseBookmarkUrl() . $this->getProfileIdQueryParameterPrefixedWithAmp();
  }

  /**
   * Transform the entity title to a attribute.
   *
   * @remarks
   *   The title of the entity and site can not contain double-qoutes. These are
   *   encoded into html chars.
   */
  private function getAttributeTitle($entity) {
    if (isset($entity->title)) {
      return array(
        self::TITLE_ATTRIBUTE => htmlentities($entity->title . ' - ' . variable_get('site_name'), ENT_COMPAT),
      );
    }
    return array();
  }

  private function getAttributeUrl($options) {
    if (isset($options['#url'])) {
      return array(
        self::URL_ATTRIBUTE => $options['#url'],
      );
    }
    return array();
  }

  public function getServiceNamesAsCommaSeparatedString($services) {
    $serviceNames = array_values($services);
    $servicesAsCommaSeparatedString = '';
    foreach ($serviceNames as $serviceName) {
      if ($serviceName != '0') {
        $servicesAsCommaSeparatedString .= $serviceName . ',';
      }
    }
    return $servicesAsCommaSeparatedString;
  }

  private function getAdminCssFilePath() {
    return drupal_get_path('module', self::MODULE_NAME) . '/' . self::ADMIN_CSS_FILE;
  }

  private function getProfileIdQueryParameter($prefix) {
    $profileId = $this->getProfileId();
    return !empty($profileId) ? $prefix . self::PROFILE_ID_QUERY_PARAMETER . '=' . $profileId : '';
  }

  private function getProfileIdQueryParameterPrefixedWithAmp() {
    return $this->getProfileIdQueryParameter('&');
  }

  private function getProfileIdQueryParameterPrefixedWithHash() {
    return $this->getProfileIdQueryParameter('#');
  }

  /**
   * Get the url for the AddThis Widget.
   */
  private function getWidgetUrl() {
    $url = ($this->currentlyOnHttps() ?
      $this->getBaseWidgetJsUrl() : // Not https url.
      $this->transformToSecureUrl($this->getBaseWidgetJsUrl()) // Transformed to https url.
    );
    return check_url($url);
  }

  /**
   * Request if we are currently on a https connection.
   *
   * @return True if we are currently on a https connection.
   */
  public function currentlyOnHttps() {
    global $base_root;
    return (strpos($base_root, 'https://') !== FALSE) ? TRUE : FALSE;
  }

  /**
   * Transform a url to secure url with https prefix.
   */
  public function transformToSecureUrl($url) {
    if ($this->currentlyOnHttps()) {
      $url = (strpos($url, 'http://') === 0 ? 'https://' . substr($url, 7) : $url);
    }
    return $url;
  }
}
