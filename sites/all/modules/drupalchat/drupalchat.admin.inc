<?php
/**
 * @file
 * Administrative functions to configure DrupalChat.
 */


function drupalchat_app_settings(){
  global $user;

  if(isset($_SESSION['token']) && !empty($_SESSION['token'])){
    $token = $_SESSION['token'];
  }else{
    $json = _drupalchat_get_auth(array());
    $token = $json['key'];
  }

  $drupalchat_host = DRUPALCHAT_EXTERNAL_A_HOST;
  $host = explode("/", $drupalchat_host);
  $host_name = $host[2];

  $dashboardUrl = "https://". DRUPALCHAT_EXTERNAL_CDN_HOST ."/apps/dashboard/#/settings/app?sessid=". $token ."&hostName=". $host_name ."&hostPort=".DRUPALCHAT_EXTERNAL_A_PORT;

  //return '<iframe id = "app_settings" style = "width:100%; height:600px; border:none;" src = "'.$dashboardUrl.'"></iframe>';
  $form = array();
  $form['drupalchat_app_dashboard'] = array(
    '#type' => 'button',
    '#attributes' => array('onClick' => 'window.open("'.$dashboardUrl.'","_blank")'),
    '#value' => t('Click here to open App Dashboard'),
  );

  return $form;
}

/**
 * Callback for admin/settings/drupalchat.
 */
function drupalchat_settings_form($form, &$form_state) {
  drupal_add_css('misc/farbtastic/farbtastic.css');
  drupal_add_js('misc/farbtastic/farbtastic.js');
  drupal_add_js(drupal_get_path('module', 'drupalchat') . '/js/drupalchat.admin.js');
  /*
  drupalchat_yui_path();
  if ((variable_get('drupalchat_yui_use_cdn', 1) == 2) && (!variable_get('drupalchat_yui_path', NULL))) {
    drupal_set_message(t("You need to download the latest stable release of the !library and extract the entire folder into sites/all/libraries.", array('!library' => l(t('YUI library'), 'http://developer.yahoo.com/yui/3/', array('attributes' => array('target' => '_blank'))))));
  }
  */
  $seconds = array(1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7=>7, 8 => 8, 9 => 9, 10 => 10, 11 => 11, 12 => 12, 13 => 13, 14 => 14, 15 => 15, 16 => 16, 17 => 17, 18 => 18, 19 => 19, 20 => 20, 30 => 30, 40 => 40, 50 => 50, 60 => 60, 70 => 70, 80 => 80, 90 => 90, 100 => 100, 110 => 110, 120 => 120, 150 => 150, 180 => 180, 240 => 240, 300 => 300);

  $themes = _drupalchat_load_themes(drupal_get_path('module', 'drupalchat') . '/themes', 'css');

  $polling_method = variable_get('drupalchat_polling_method', DRUPALCHAT_AJAX);

  if ($polling_method == DRUPALCHAT_LONGPOLL && ini_get('max_execution_time') < 30) {
    drupal_set_message(t('For DrupalChat Long Polling to be effective, please set max_execution_time to above 30 in your server php.ini file.'), 'warning');
  }

  $form = array();
  /*$form['libraries'] = array(
    '#type' => 'fieldset',
    '#title' => t('Library info'),
    '#collapsible' => TRUE,
  );
  $form['libraries']['yui'] = array(
    '#type' => 'fieldset',
    '#title' => t('YUI'),
    '#collapsible' => TRUE,
  );
  $form['libraries']['yui']['drupalchat_yui_use_cdn'] = array(
    '#type' => 'select',
    '#title' => t('Load YUI library from Yahoo CDN'),
    '#collapsible' => TRUE,
	'#options' => array(1 => 'Yes', 2 => 'No, load from my server'),
    '#default_value' => variable_get('drupalchat_yui_use_cdn', 1),
  );
  $form['libraries']['yui']['drupalchat_yui_path'] = array(
    '#type' => 'textfield',
    '#title' => t('YUI library path on your server'),
    '#description' => t("You need to download the latest stable release of the !library and extract the entire folder into sites/all/libraries. The path will be automatically discovered; you may override the directory path here if you wish to use another version; do not add slashes before or after the path.", array('!library' => l(t('YUI library'), 'http://developer.yahoo.com/yui/3/', array('attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('drupalchat_yui_path', NULL),
  );*/

  $form['drupalchat_general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('DrupalChat General Settings'),
    '#collapsible' => TRUE,
  );
  $form['drupalchat_general_settings']['drupalchat_polling_method'] = array(
		'#type' => 'radios',
		'#title' => t('Choose Polling Method'),
		'#default_value' => variable_get('drupalchat_polling_method', DRUPALCHAT_AJAX),
		'#options' => array(DRUPALCHAT_COMMERCIAL => t('iFlyChat server'), DRUPALCHAT_AJAX => t('Normal AJAX'), DRUPALCHAT_LONGPOLL => t('Long Polling'), DRUPALCHAT_NODEJS => t('Node.js (currently under development)'),),
		'#description' => t('Decide the server backend for Drupal Chat.'),
  );
  $form['drupalchat_general_settings']['drupalchat_app_id'] = array(
  '#type' => 'textfield',
  '#title' => t('iFlyChat APP ID'),
  '#description' => t("Please enter the APP ID by registering at !elink.", array('!elink' => l(t('iFlyChat.com'), 'https://iflychat.com', array('attributes' => array('target' => '_blank'))))),
  '#default_value' => variable_get('drupalchat_app_id', NULL),
  );
  $form['drupalchat_general_settings']['drupalchat_external_api_key'] = array(
	'#type' => 'textfield',
    '#title' => t('iFlyChat API Key'),
    '#description' => t("Please enter the API key by registering at !elink.", array('!elink' => l(t('iFlyChat.com'), 'https://iflychat.com', array('attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('drupalchat_external_api_key', NULL),
  );

  $form['drupalchat_general_settings']['drupalchat_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('All themes from inside the <em>themes</em> folder will be displayed here.'),
    '#options' => $themes,
    '#default_value' => variable_get('drupalchat_theme', 'light'),
  );
  $form['drupalchat_general_settings']['drupalchat_notification_sound'] = array(
    '#type' => 'select',
    '#title' => t('Notification Sound'),
    '#description' => t('Select whether to play notification sound when a new message is received.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_notification_sound', 1),
  );
  $form['drupalchat_general_settings']['drupalchat_user_picture'] = array(
    '#type' => 'select',
    '#title' => t('User Pictures'),
    '#description' => t('Select whether to show user pictures in chat.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_user_picture', 1),
  );
  $form['drupalchat_general_settings']['drupalchat_enable_smiley'] = array(
    '#type' => 'select',
    '#title' => t('Enable Smileys'),
    '#description' => t('Select whether to show smileys.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_enable_smiley', 1),
  );
  $form['drupalchat_general_settings']['drupalchat_log_messages'] = array(
    '#type' => 'select',
    '#title' => t('Log chat messages'),
    '#description' => t('Select whether to log chat messages, which can be later viewed in !inbox_link.', array('!inbox_link' => l(t('message inbox'),'drupalchat/messages/inbox'))),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_log_messages', 1),
  );
  $form['drupalchat_general_settings']['drupalchat_anon_prefix'] = array(
	'#type' => 'textfield',
    '#title' => t('Prefix to be used with anonymous users'),
    '#description' => t("Please specify the prefix to be used with anonymous users. It shouldn't be long. Ideally it should be between 4 to 7 characters."),
    '#default_value' => variable_get('drupalchat_anon_prefix', 'Guest'),
  );
  $form['drupalchat_general_settings']['drupalchat_anon_use_name'] = array(
    '#type' => 'select',
	'#title' => t('Use random name or number for anonymous user'),
	'#description' => t('Select whether to use random generated name or number to assign to a new anonymous user'),
	'#options' => array(1 => 'Name', 2 => 'Number'),
	'#default_value' => variable_get('drupalchat_anon_use_name', 1),
  );
   $form['drupalchat_general_settings']['drupalchat_user_latency'] = array(
    '#type' => 'select',
    '#title' => t('Chat List Latency'),
    '#description' => t('The delay, in seconds, after which the user will be shown offline in the chat list(i.e. removed from the chat list) from the time he/she goes offline. Increase this value if you find the chat list is unstable and keeps on changing a lot (for example - when a user navigates from one page to another he/she goes offline and then comes back online again). Decrease it if you find that the users are shown in the chat list for too long after they have left your website.'),
    '#options' => $seconds,
    '#default_value' => variable_get('drupalchat_user_latency', 30),
  );
  $form['drupalchat_general_settings']['drupalchat_refresh_rate'] = array(
    '#type' => 'select',
    '#title' => t('Normal AJAX Refesh Rate'),
    '#description' => t('The time interval, in seconds, after which DrupalChat checks for new messages.'),
    '#options' => $seconds,
    '#default_value' => variable_get('drupalchat_refresh_rate', 2),
    '#disabled' => variable_get('drupalchat_polling_method', DRUPALCHAT_AJAX) == DRUPALCHAT_LONGPOLL ? TRUE : FALSE,
  );
  $form['drupalchat_pc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Chat Moderation'),
    '#collapsible' => TRUE,
  );
  $form['drupalchat_pc']['drupalchat_enable_chatroom'] = array(
    '#type' => 'radios',
	'#title' => t('Enable Public Chatroom'),
	'#default_value' => variable_get('drupalchat_enable_chatroom', 1),
	'#options' => array(1 => 'Yes', 2 => 'No'),
  );
  $form['drupalchat_path'] = array(
    '#type' => 'fieldset',
    '#title' => t('DrupalChat Visibility'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['drupalchat_path']['drupalchat_show_embed_chat'] = array(
    '#type' => 'item',
    '#title' => t("Show Embed Chat"),
    //'#description' => t("Click on the button to view the tutorial of embedding the chat onto a page."),
    '#description' => t("Click on the !elink to view the tutorial of embedding the chat onto a page.", array('!elink' => l(t('link'), 'https://iflychat.com/docs/integration/drupal/embed-chat/how-embed-chatroom', array('attributes' => array('target' => '_blank'))))),
  );
  $access = user_access('use PHP for settings');
  $options = array(
    BLOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    BLOCK_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  if (module_exists('php') && $access) {
    $options += array(BLOCK_VISIBILITY_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
    $title = t('Pages or PHP code');
    $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  else {
    $title = t('Pages');
  }
  $form['drupalchat_path']['drupalchat_path_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show DrupalChat on specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('drupalchat_path_visibility', BLOCK_VISIBILITY_NOTLISTED),
  );
  $form['drupalchat_path']['drupalchat_path_pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span>' . $title . '</span>',
    '#default_value' => variable_get('drupalchat_path_pages', NULL),
    '#description' => $description,
  );
  $form['drupalchat_chatlist_cont'] = array(
    '#type' => 'fieldset',
    '#title' => t('DrupalChat User Online List Control'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['drupalchat_chatlist_cont']['drupalchat_rel'] = array(
    '#type' => 'radios',
    '#title' => t('Relationship method'),
    '#default_value' => variable_get('drupalchat_rel', DRUPALCHAT_REL_AUTH),
    '#options' => array(
      DRUPALCHAT_REL_AUTH => t('All users'),
    ),
    '#description' => t('This determines the method for creating the chat buddylist.'),
  );
  if (module_exists('user_relationships')) {
    $form['drupalchat_chatlist_cont']['drupalchat_rel']['#options'][DRUPALCHAT_REL_UR] = t('User Relationship module');
  }
  if (module_exists('flag_friend')) {
    $form['drupalchat_chatlist_cont']['drupalchat_rel']['#options'][DRUPALCHAT_REL_FF] = t('Flag Friend module');
  }
  if (module_exists('og')) {
    $form['drupalchat_chatlist_cont']['drupalchat_rel']['#options'][DRUPALCHAT_REL_OG] = t('Organic Groups module');
  }
  $form['drupalchat_chatlist_cont']['drupalchat_ur_name'] = array(
    '#type' => 'textfield',
    '#title' => t('User Relationships Role Names to integrate with'),
    '#description' => t('The singular form of User Relationships Role Names (e.g. buddy, friend, coworker, spouse) separated by comma.'),
    '#default_value' => variable_get('drupalchat_ur_name', NULL),
	  '#autocomplete_path' => 'drupalchat/ur-autocomplete',
  );
  $form['drupalchat_advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['drupalchat_advanced_settings']['drupalchat_session_caching'] = array(
    '#type' => 'select',
    '#title' => t('Enable PHP Session Caching'),
    '#description' => t('Select whether to use PHP session for caching chat authentication token.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_session_caching', 2),
  );
  return system_settings_form($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function drupalchat_settings_form_validate($form, &$form_state) {
  global $user;

  if($form_state['values']['drupalchat_polling_method'] == DRUPALCHAT_COMMERCIAL){

    $form_state['values']['drupalchat_app_id'] = trim($form_state['values']['drupalchat_app_id']);
    $form_state['values']['drupalchat_external_api_key'] = trim($form_state['values']['drupalchat_external_api_key']);
    $formValues = array(
      'api_key' => $form_state['values']['drupalchat_external_api_key'],
      'app_id' => $form_state['values']['drupalchat_app_id']
    );

    $result = _drupalchat_get_auth($formValues);
    if(gettype($result) == 'object'){
      form_set_error('drupalchat_external_api_key', "Unable to connect to iFlyChat server. Error code - " . $result->code . ". Error message - " . $result->error . ".");  
    }

    $app_id = $form_state['values']['drupalchat_app_id'];
    if(!(strlen($app_id) == 36 && $app_id[14] == '4')){
      form_set_error('drupalchat_app_id', "Invalid APP ID.");
    }

    if(!$form_state['values']['drupalchat_app_id']) {
      form_set_error('drupalchat_app_id', t('Please enter APP ID.'));
    }
    if(!$form_state['values']['drupalchat_external_api_key']) {
      form_set_error('drupalchat_external_api_key', t('Please enter API key.'));
    }


  }


  if ($form_state['values']['drupalchat_rel'] == DRUPALCHAT_REL_UR) {
  /*if (!$form['drupalchat_ur_name']['#value']) {
	  form_set_error('drupalchat_ur_name', t('You must provide the user relationship name in the Drupal UR Settings section.));
    }*/
    if ($form_state['values']['drupalchat_ur_name']) {
	  $array = drupal_explode_tags($form_state['values']['drupalchat_ur_name']);
	  $error = array();
	  foreach($array as $key) {
	    if(!db_query("SELECT COUNT(*) FROM {user_relationship_types} WHERE name = :name", array(':name' => $key))->fetchField())
          $error[] = $key;
      }
	  if(!empty($error))
	    form_set_error('drupalchat_ur_name', t('User Relationship type %type was not found.', array('%type' => drupal_implode_tags($error))));
    }
  }

}

function drupalchat_settings_form_submit($form, &$form_state) {
  //$form_state['redirect'] = array('admin/config/drupalchat/configuration');
  //drupal_goto('admin/config/drupalchat/configuration');
}

function _drupalchat_load_themes($outerDir, $x) {
  $dirs = array_diff(scandir($outerDir), array('.', '..'));

  $dir_array = array();
  foreach ($dirs as $d) {
    if (is_dir($outerDir . "/" . $d)) {
      if ($innerDir = _drupalchat_load_themes($outerDir . '/' . $d, $x)) {
        $dir_array[$d] = $innerDir;
      }
    }
    elseif (($x) ? preg_match('/' . $x . '$/', $d) : 1) {
      $name = _drupalchat_remove_extension($d);
      $dir_array[$name] = $name;
    }
  }
  return $dir_array;
}

function _drupalchat_remove_extension($strName) {
  $ext = strrchr($strName, '.');

  if ($ext !== false) {
    $strName = substr($strName, 0, -strlen($ext));
  }
  return $strName;
}
