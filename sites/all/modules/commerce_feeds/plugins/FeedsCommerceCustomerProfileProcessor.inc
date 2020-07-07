<?php

/**
 * @file
 * Class definition of FeedsCommerceCustomerProfileProcessor.
 */

/**
 * Creates profiles from feed items.
 */
class FeedsCommerceCustomerProfileProcessor extends FeedsProcessor {

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'commerce_customer_profile';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Commerce customer profiles');
    return $info;
  }

  /**
   * Creates a new profile in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $type = $this->config['customer_profile_type'];
    $profile = commerce_customer_profile_new($type, 0);
    $profile->log = 'Created by FeedsCommerceProfileProcessor';
    return $profile;
  }

  /**
   * Loads an existing profile.
   */
  protected function entityLoad(FeedsSource $source, $profile_id) {
    $profile = parent::entityLoad($source, $profile_id);
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $profile->log = 'Updated by FeedsCommerceProfileProcessor';
    }
    else {
      $profile->log = 'Replaced by FeedsCommerceProfileProcessor';
    }
    return $profile;
  }

  /**
   * Save a profile.
   */
  public function entitySave($entity) {
    commerce_customer_profile_save($entity);
  }

  /**
   * Delete a series of profiles.
   */
  protected function entityDeleteMultiple($entity_ids) {
    commerce_customer_profile_delete_multiple($entity_ids);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $types = commerce_customer_profile_type_get_name();
    $type = isset($types['billing']) ? 'billing' : key($types);
    return array(
      'customer_profile_type' => $type,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $types = commerce_customer_profile_type_get_name();
    array_walk($types, 'check_plain');
    $form = parent::configForm($form_state);
    unset($form['input_format']);
    $form['customer_profile_type'] = array(
      '#type' => 'select',
      '#title' => t('Customer profile type'),
      '#description' => t('Select the customer profile type for the profiles to be created. <strong>Note:</strong> Users with "import !feed_id feeds" permissions will be able to <strong>import</strong> profiles of the customer type selected here regardless of the customer profile level permissions. Further, users with "clear !feed_id permissions" will be able to <strong>delete</strong> imported profiles regardless of their customer profile level permissions.', array('!feed_id' => $this->id)),
      '#options' => $types,
      '#default_value' => $this->config['customer_profile_type'],
    );
    return $form;
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      'status' => array(
        'name' => t('Customer profile status'),
        'description' => t('Status of the customer profile.'),
      ),
      'uid' => array(
        'name' => t('User ID'),
        'description' => t('The Drupal user ID that owns the profile.'),
      ),
      'type' => array(
        'name' => t('Profile type'),
        'description' => t('Commerce profile type, if not specified the default one will be used.'),
      ),
    );

    // Let other modules expose mapping targets.
    self::loadMappers();
    $entity_type = $this->entityType();
    $bundle = $this->config['customer_profile_type'];
    drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);

    return $targets;
  }

  /**
   * Override setTargetElement to operate on a target item.
   */
  public function setTargetElement(FeedsSource $source, $target_entity, $target_element, $value) {
    switch ($target_element) {
      case 'type':
        $profile_types = commerce_customer_profile_type_get_name();
        $target_entity->type = $value;
        if (!array_key_exists($value, $profile_types)) {
          $target_entity->type = $this->config['customer_profile_type'];
        }
        break;
      case 'uid':
        $target_entity->uid = $value;
        break;
      default:
        parent::setTargetElement($source, $target_entity, $target_element, $value);
        break;
    }
  }

}
