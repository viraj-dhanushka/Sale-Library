<?php

/**
 * @file
 * Administrative functions of the revenue_sharing_basic module.
 *
 * This file is included by the basic revenue sharing  module, and includes
 * the settings form.
 */

/**
 * Menu callback for the revenue_sharing_basic module settings form.
 *
 * @ingroup forms
 */
function revenue_sharing_basic_settings($form, &$form_state) {
  module_load_include('inc', 'revenue_sharing_basic', 'help/revenue_sharing_basic.help');

  $form['help'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Help and instructions'),
  );

  $form['help']['help'] = array(
    '#type' => 'markup',
    '#markup' => revenue_sharing_basic_help_text(),
  );

  $form['required'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Required parameters'),
  );

  $form['required']['revenue_sharing_basic_client_id_profile_field'] = array(
    '#type' => 'select',
    '#title' => t('Google AdSense client ID profile field'),
    '#default_value' => variable_get('revenue_sharing_basic_client_id_profile_field', REVENUE_SHARING_BASIC_CLIENT_ID_PROFILE_FIELD_DEFAULT),
    '#options' => revenue_sharing_basic_get_profile_fields(),
    '#required' => TRUE,
    '#description' => t('This is the profile field that holds the AdSense Client ID for the site owner as well as (optionally) for site users who participate in revenue sharing. You must enabled the profile module and create a new field for this.'),
  );

  $form['percentage'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Revenue sharing percentage'),
  );

  $options = drupal_map_assoc(range(0, 100, 5));

  $form['percentage']['revenue_sharing_basic_percentage_author'] = array(
    '#type' => 'select',
    '#title' => t('Percentage of node views going to author'),
    '#default_value' => variable_get('revenue_sharing_basic_percentage_author', REVENUE_SHARING_BASIC_PERCENTAGE_AUTHOR_DEFAULT),
    '#options' => $options,
  );

  $form['percentage']['role'] = array(
    '#type' => 'fieldset',
    '#title' => t('Percentage of node views going to author with the following roles'),
    '#description' => t('When the author belongs to one or more roles, the percentage of node views using his AdSense Client ID will be the maximum between the author value and the following settings for each role.'),
    '#theme' => 'revenue_sharing_basic_author_percentage_role',
  );

  $roles = user_roles(TRUE);
  unset($roles[array_search('authenticated user', $roles)]);
  foreach ($roles as $role => $role_desc) {
    $form['percentage']['role']['revenue_sharing_basic_percentage_role_' . $role] = array(
      '#type' => 'select',
      '#title' => t($role_desc),
      '#default_value' => variable_get('revenue_sharing_basic_percentage_role_' . $role, REVENUE_SHARING_BASIC_PERCENTAGE_ROLE_DEFAULT),
      '#options' => $options,
    );
  }

  if (module_exists('referral')) {
    $form['percentage']['revenue_sharing_basic_percentage_refer'] = array(
      '#type' => 'select',
      '#title' => t('Percentage of node views going to user who referred the author'),
      '#default_value' => variable_get('revenue_sharing_basic_percentage_refer', REVENUE_SHARING_BASIC_PERCENTAGE_REFER_DEFAULT),
      '#options' => $options,
    );
  }

  $form['content_types'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Content types'),
  );

  $types = node_type_get_types();
  foreach ($types as $type => $name) {
    $form['content_types']['revenue_sharing_basic_node_type_' . $type] = array(
      '#type' => 'checkbox',
      '#title' => $name->name,
      '#default_value' => variable_get('revenue_sharing_basic_node_type_' . $type, REVENUE_SHARING_BASIC_NODE_TYPE_DEFAULT),
    );
  }

  $form_state['#redirect'] = 'admin/config/services/adsense/publisher';

  return system_settings_form($form);
}

/**
 * Theme the author percentage part of the settings page.
 *
 * @ingroup themeable
 */
function theme_revenue_sharing_basic_author_percentage_role($variables) {
  $form = $variables['form'];
  $output = '';
  $elements = element_children($form);
  $rows = array();
  if (!empty($elements)) {
    $header = array(t('Role'), t('Percentage'));
    foreach ($elements as $key) {
      $row = array();
      $row[] = $form[$key]['#title'];
      unset($form[$key]['#title']);
      $row[] = drupal_render($form[$key]);
      $rows[] = $row;
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Create the list for the revenue_sharing_basic_client_id_profile_field field.
 *
 * @return array
 *   array of fields with the field IDs as keys and the field titles as values
 *
 * @ingroup forms
 */
function revenue_sharing_basic_get_profile_fields() {
  $profile_list = array(0 => 'None');

  // Start with the Field module fields.
  if (module_exists('field')) {
    $fields = field_read_instances(array('entity_type' => 'user'));
    foreach ($fields as $field) {
      $profile_list['field:' . $field['field_name']] = $field['label'];
    }
  }

  // Get fields from the Profile module.
  if (module_exists('profile')) {
    $result = db_query("SELECT fid, name, title FROM {profile_field} WHERE type='textfield' ORDER BY fid");
    foreach ($result as $row) {
      $profile_list['profile:' . $row->name] = $row->title;
    }
  }

  // Also display Profile2 fields.
  if (module_exists('profile2')) {
    $bundles = field_info_instances('profile2');
    foreach ($bundles as $bundle) {
      foreach ($bundle as $field) {
        if ($field['display']['default']['module'] == 'text') {
          $profile_list['profile2:' . $field['bundle'] . ':' . $field['field_name']] = $field['label'];
        }
      }
    }
  }

  return $profile_list;
}
