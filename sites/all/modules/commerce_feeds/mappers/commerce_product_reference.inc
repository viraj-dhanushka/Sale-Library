<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for commerce_product_reference.module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets().
 */
function _commerce_product_reference_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'commerce_product_reference') {
      if (array_key_exists('product_id', $info['columns'])) {
        // Real target is modified in order to keep the older values from the
        // field of the entity to support accumulative imports.
        // See http://drupal.org/node/1188994

        $targets[$name . ":product_id"] = array(
          'name' => $instance['label'] . ': Product ID',
          'callback' => 'commerce_product_reference_feeds_set_target',
          'description' => t('The product id for the @name field. NOTE: use this feature with care, products ids are usually assigned by Drupal.', array('@name' => $instance['label'])),
          'real_target' => 'commerce-' . $name,
        );
        $targets[$name . ":sku"] = array(
          'name' => $instance['label'] . ': SKU',
          'callback' => 'commerce_product_reference_feeds_set_target',
          'description' => t('The SKU reference for the @name field. NOTE: the product entity needs to exist.', array('@name' => $instance['label'])),
          'real_target' => 'commerce-' . $name,
        );
      }
    }
  }
}

/**
 * Callback for mapping. Here is where the actual mapping happens.
 *
 * When the callback is invoked, $target contains the name of the field the
 * user has decided to map to and $value contains the value of the feed item
 * element the user has picked as a source.
 */
function commerce_product_reference_feeds_set_target(FeedsSource $source, $entity, $target, $value) {
  if (empty($value)) {
    return;
  }

  // Handle non-multiple value fields.
  if (!is_array($value)) {
    $value = array($value);
  }

  // Get field information.
  list($field_name, $sub_field) = explode(':', $target);
  $info = field_info_field($field_name);
  $field = isset($entity->$field_name) ? $entity->$field_name : array();

  // If we're updating the existing content, we'd want to keep the existing
  // field content and update it accordingly.
  // Also, if the field is empty, we map the values the usual way.
  if ($source->importer->config['processor']['config']['update_existing'] == FEEDS_UPDATE_EXISTING && !empty($field) && count($field[LANGUAGE_NONE])) {
    $i = count($field[LANGUAGE_NONE]);
    // Load all SKUs first to minimize loading calls if we're dealing with SKUs.
    $entities_by_sku = array();
    if($sub_field == 'sku') {
      $skus = array();
      foreach ($value as $v) {
        $skus[] = $v;
      }
      $product_entities = commerce_product_load_multiple(array(), array('sku' => $skus));

      // Get loaded entities into array keyed by SKU.
      $entities_by_sku = array();
      foreach($product_entities as $product_entity){
        $entities_by_sku[$product_entity->sku] = $product_entity;
      }
    }

    foreach ($value as $v) {
      if (!is_array($v) && !is_object($v)) {
        if ($sub_field == 'product_id') {
          $product_id = $v;
        }
        elseif ($sub_field == 'sku') {
          if ($product = $entities_by_sku[$v]) {
            $product_id = $product->product_id;
          }
          else {
            drupal_set_message(t('A product with SKU %sku could not be found. Please check that the product exists or import it first.', array('%sku' => $v)), 'error');
          }
        }
        if ($product_id) {
          $delta = $i;
          // If there are already some elements in the field, we search for it to reuse them.
          foreach($field[LANGUAGE_NONE] as $key=>$id) {
            if ($id['product_id'] == $product_id) {
              $delta = $key;
              break;
            }
          }
          $field[LANGUAGE_NONE][$delta]['product_id'] = $product_id;
        }
      }
      if ($info['cardinality'] == 1) {
        break;
      }
      $i++;
    }
  }
  else {
    // If we're not updating, we're replacing or creating so a normal behavior
    // is expected.
    foreach ($value as $i => $v) {
      if (!is_array($v) && !is_object($v)) {
        if ($sub_field == 'product_id') {
          $field[LANGUAGE_NONE][$i]['product_id'] = $v;
        }
        elseif ($sub_field == 'sku') {
          if ($product = commerce_product_load_by_sku($v)) {
            $field[LANGUAGE_NONE][$i]['product_id'] = $product->product_id;
          } else {
            drupal_set_message(t('A product with SKU %sku could not be found. Please check that the product exists or import it first.', array('%sku' => $v)), 'error');
          }
        }
      }
      if ($info['cardinality'] == 1) {
        break;
      }
    }
  }

  $entity->{$field_name} = $field;
}
