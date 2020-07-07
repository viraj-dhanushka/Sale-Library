<?php

/**
 * @file
 * A handler to supply source statuses.
 */
class lingotek_views_handler_source_status_formatted extends views_handler_field {
  /**
   * Constructor; calls to base object constructor.
   */
  function construct() {
    parent::construct();
  }

  /**
   * Implements views_handler#options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['custom_label']['#default_value'] = FALSE;
    $form['label']['#default_value'] = '';
  }

  function render($values) {
    $value = $this->get_value($values);
    $entity_type = NULL;
    $entity_id = NULL;
    global $language;

    $entity_type = $this->view->base_table;

    if ($entity_type == 'node') {
      $entity_id = $values->nid;
      $entity = node_load($entity_id);
      if ($this->options['relationship'] != 'NONE') {

      }
    }

    if (!user_access('manage projects') && !user_access('translation')) {
      return; //do not show if the user does not have correct permissions
    }

    $upload_status = LingotekSync::getUploadStatus($entity_type, $value);

    $upload_status_lowercase = strtolower($upload_status);

    $source_text = '<span class="ltk-source-icon source-' . $upload_status_lowercase . '" title="' . $upload_status . '">' . $entity->language . '</span>';

    return $source_text;
  }

  function element_type($none_supported = FALSE, $default_empty = FALSE, $inline = FALSE) {
    if ($inline) {
      return 'span';
    }

    if (isset($this->definition['element type'])) {
      return $this->definition['element type'];
    }

    return 'div';
  }
}
