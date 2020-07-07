<?php

/**
 * @file
 * Class definition of FeedsCommerceProductProcessor.
 */

/**
 * Creates products from feed items.
 */
class FeedsCommerceProductProcessor extends FeedsProcessor {

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'commerce_product';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Commerce Products');
    return $info;
  }

  /**
   * Creates a new product in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $type = $this->config['product_type'];
    $product = commerce_product_new($type);
    $product->uid = $this->config['author'];
    $product->log = 'Created by FeedsCommerceProductProcessor';
    return $product;
  }

  /**
   * Loads an existing product.
   */
  protected function entityLoad(FeedsSource $source, $product_id) {
    $product = parent::entityLoad($source, $product_id);
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $product->log = 'Updated by FeedsCommerceProductProcessor';
    }
    else {
      $product->log = 'Replaced by FeedsCommerceProductProcessor';
    }
    return $product;
  }

  /**
   * Save a product.
   */
  public function entitySave($entity) {
    commerce_product_save($entity);
  }

  /**
   * Delete a series of products.
   */
  protected function entityDeleteMultiple($nids) {
    commerce_product_delete_multiple($nids);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $types = commerce_product_type_get_name();
    $type = isset($types['product']) ? 'product' : key($types);
    return array(
      'product_type' => $type,
      'author' => 0,
      'tax_rate' => '',
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $types = commerce_product_type_get_name();
    array_walk($types, 'check_plain');
    $form = parent::configForm($form_state);
    $form['product_type'] = array(
      '#type' => 'select',
      '#title' => t('Product type'),
      '#description' => t('Select the product type for the products to be created. <strong>Note:</strong> Users with "import !feed_id feeds" permissions will be able to <strong>import</strong> products of the type selected here regardless of the product level permissions. Further, users with "clear !feed_id permissions" will be able to <strong>delete</strong> imported products regardless of their product level permissions.', array('!feed_id' => $this->id)),
      '#options' => $types,
      '#default_value' => $this->config['product_type'],
    );
    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the products to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ? 'anonymous' : check_plain($author->name),
    );
    if (module_exists('commerce_tax')) {
      $inclusive_types = array();

      foreach (commerce_tax_types() as $name => $tax_type) {
        if ($tax_type['display_inclusive']) {
          $inclusive_types[$name] = $tax_type['title'];
        }
      }

      $options = array();

      foreach (commerce_tax_rates() as $name => $tax_rate) {
        if (in_array($tax_rate['type'], array_keys($inclusive_types))) {
          $options[$inclusive_types[$tax_rate['type']]][$name] = $tax_rate['title'];
        }
      }

      $form['tax_rate'] = array(
        '#type' => 'select',
        '#title' => t('Tax rate'),
        '#description' => t('Select a display inclusive tax rate to set on the product prices in import.'),
        '#options' => count($options) == 1 ? reset($options) : $options,
        '#empty_value' => TRUE,
        '#default_value' => !empty($this->config['tax_rate']) ? $this->config['tax_rate'] : '',
      );
    }
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    if ($author = user_load_by_name($values['author'])) {
      $values['author'] = $author->uid;
    }
    else {
      $values['author'] = 0;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      'product_id' => array(
        'name' => t('Product ID'),
        'description' => t('The id of the product. NOTE: use this feature with care, product ids are usually assigned by Drupal Commerce.'),
        'optional_unique' => TRUE,
      ),
      'sku' => array(
        'name' => t('Product SKU'),
        'description' => t('The product identifier. Must be unique.'),
        'optional_unique' => TRUE,
      ),
      'title' => array(
        'name' => t('Product title'),
        'description' => t('The product title.'),
      ),
      'status' => array(
        'name' => t('Product status'),
        'description' => t('Status of the product.'),
      ),
      'uid' => array(
        'name' => t('User ID'),
        'description' => t('The Drupal user ID of the product owner.'),
      ),
    );

    // Let other modules expose mapping targets.
    self::loadMappers();
    $entity_type = $this->entityType();
    $bundle = $this->config['product_type'];
    drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);

    return $targets;
  }

  /**
   * Get product_id of an existing product if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($product_id = parent::existingEntityId($source, $result)) {
      return $product_id;
    }

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'product_id':
          $product_id = db_query("SELECT product_id FROM {commerce_product} WHERE product_id = :product_id", array(':product_id' => $value))->fetchField();
          break;
        case 'sku':
          $product_id = db_query("SELECT product_id FROM {commerce_product} WHERE sku = :sku", array(':sku' => $value))->fetchField();
          break;
      }
      if ($product_id) {
        // Return with the first sku found.
        return $product_id;
      }
    }
    return 0;
  }

  /**
   * Validate the commerce_product entity.
   */
  protected function entityValidate($product) {
    if (empty($product->sku)) {
      throw new FeedsValidationException(t('Required product SKU is missing.'));
    }
    else if (!commerce_product_validate_sku($product->sku)) {
      throw new FeedsValidationException(t('Product SKU ("@sku") is invalid.', array('@sku' => $product->sku)));
    }
  }
}
