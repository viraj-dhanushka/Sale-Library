<?php

/**
 * @file
 * Lingotek Module + Views
 */

/**
 * Implements hook_views_data().
 */
function lingotek_views_data() {
  $data['node']['lingotek_workbench_link'] = array(
    'group' => 'Lingotek',
    'title' => t('Workbench Link'),
    'help' => t('Lingotek Integration'),
    'real field' => 'nid',
    'field' => array(
      'help' => t('Provide a link to the Lingotek Workbench so that a user may translate the node.'),
      'handler' => 'lingotek_views_handler_workbench_link',
    ),
  );
  $data['node']['lingotek_source_status'] = array(
    'group' => 'Lingotek',
    'title' => t('Source Status (Unformatted)'),
    'help' => t('Lingotek Integration'),
    'real field' => 'nid',
    'field' => array(
      'help' => t('Provide the translation source status for a node.'),
      'handler' => 'lingotek_views_handler_source_status',
    ),
  );
  $data['node']['lingotek_source_status_formatted'] = array(
    'group' => 'Lingotek',
    'title' => t('Source Status (Formatted)'),
    'help' => t('Lingotek Integration'),
    'real field' => 'nid',
    'field' => array(
      'help' => t('Provide the translation source status for a node.'),
      'handler' => 'lingotek_views_handler_source_status_formatted',
    ),
  );
  $data['node']['lingotek_target_status'] = array(
    'group' => 'Lingotek',
    'title' => t('Translation Status (Unformatted)'),
    'help' => t('Lingotek Integration'),
    'real field' => 'nid',
    'field' => array(
      'help' => t('Provide the translation target status for a node.'),
      'handler' => 'lingotek_views_handler_target_status',
    ),
  );
  $data['node']['lingotek_target_status_formatted'] = array(
    'group' => 'Lingotek',
    'title' => t('Translation Status (Formatted)'),
    'help' => t('Lingotek Integration'),
    'real field' => 'nid',
    'field' => array(
      'help' => t('Provide the translation target status for a node.'),
      'handler' => 'lingotek_views_handler_target_status_formatted',
    ),
  );
  $data['comments']['lingotek_workbench_link'] = array(
    'group' => 'Lingotek',
    'title' => t('Workbench Link'),
    'help' => t('Lingotek Integration'),
    'real field' => 'nid',
    'field' => array(
      'help' => t('Provide a link to the Lingotek Workbench so that a user may translate the node.'),
      'handler' => 'lingotek_views_handler_workbench_link',
    ),
  );

  return $data;
}
