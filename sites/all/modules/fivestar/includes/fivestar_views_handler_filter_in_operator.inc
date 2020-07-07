<?php

/**
 * @file
 * Provides the view handling functions for fivestar.
 */

/**
 *
 */
class fivestar_views_handler_filter_in_operator extends views_handler_filter_in_operator {

  /**
   * Initialize handler.
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    // We handle empty/not empty ourselves.
    $this->definition['allow empty'] = FALSE;
  }

  /**
   * Get the possible options (the number of stars).
   */
  public function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    // Get field info to figure out the number of stars configured for it.
    $field_name = $this->definition['field_name'];
    $field_info = field_info_field($field_name);
    $bundles = reset($field_info['bundles']);
    $entity_type = key($field_info['bundles']);
    $bundle_name = $bundles[0];
    $field_instance = field_info_instance($entity_type, $field_name, $bundle_name);

    $max_stars = !empty($field_instance['settings']['stars']) ? $field_instance['settings']['stars'] : 5;
    $options = array(0 => t('No stars'));
    for ($i = 1; $i <= $max_stars; $i++) {
      $percentage = ceil($i * 100 / $max_stars);
      $options[$percentage] = format_plural($i, '1 star', '@count stars');
    }
    $this->value_options = $options;
  }

  /**
   * Operator callback.
   */
  public function op_simple() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();

    // Zero needs special handling.
    $has_nul = in_array(0, $this->value);

    $or_statement = db_or();
    $and_statement = db_and();
    // Add regular condition if we have any values other than zero.
    if (count($this->value) > ($has_nul ? 1 : 0)) {
      $or_statement->condition("$this->table_alias.$this->real_field", array_values($this->value), $this->operator);
    }

    $selected_val_count = count(array_values($this->value));
    $first_element = reset($this->value);
    // 'IN' Condition.
    if ($this->operator == 'in') {
      if ($selected_val_count == 1) {
        if ($first_element == 0) {
          $nul_operator = ($has_nul == 0) ? 'IS NOT NULL' : 'IS NULL';
          $or_statement->condition("$this->table_alias.$this->real_field", NULL, $nul_operator);
          $this->query->add_where($this->options['group'], $or_statement);
        }
        elseif ($first_element != 0) {
          $operator = 'IN';
          $and_statement->condition("$this->table_alias.$this->real_field", array_values($this->value), $operator);
          $this->query->add_where($this->options['group'], $and_statement);
        }
      }
      elseif ($selected_val_count > 1) {
        if ($first_element == 0) {
          $nul_operator = ($has_nul == 0) ? 'IS NOT NULL' : 'IS NULL';
          $or_statement->condition("$this->table_alias.$this->real_field", NULL, $nul_operator);
          $this->query->add_where($this->options['group'], $or_statement);
        }
        elseif ($first_element != 0) {
          $operator = 'IN';
          $and_statement->condition("$this->table_alias.$this->real_field", array_values($this->value), $operator);
          $this->query->add_where($this->options['group'], $and_statement);
        }
      }
    }
    // 'NOT IN' Condition.
    elseif ($this->operator == 'not in') {
      if ($has_nul == 1 && $first_element == 0) {
        if ($selected_val_count == 1) {
          $nul_operator = 'IS NOT NULL';
          $and_statement->condition("$this->table_alias.$this->real_field", array_values($this->value), $nul_operator);
          $this->query->add_where($this->options['group'], $and_statement);
        }
        elseif ($selected_val_count > 1) {
          $operator = 'NOT IN';
          $nul_operator = 'IS NOT NULL';
          $and_statement->condition("$this->table_alias.$this->real_field", array_values($this->value), $operator);
          $and_statement->condition("$this->table_alias.$this->real_field", NULL, $nul_operator);
          $this->query->add_where($this->options['group'], $and_statement);
        }
      }
      elseif ($has_nul == 0) {
        $nul_operator = 'IS NULL';
        $or_statement->condition("$this->table_alias.$this->real_field", NULL, $nul_operator);
        $this->query->add_where($this->options['group'], $or_statement);
      }
    }
  }

}
