<?php

/**
 * @file
 * FeedsUserProcessor class.
 */

/**
 * Feeds processor plugin. Create users from feed items.
 */
class FeedsCommerceCouponProcessor extends FeedsProcessor {

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'commerce_coupon';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Commerce coupons');
    return $info;
  }

  /**
   * Creates a new coupon and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $type = $this->config['coupon_type'];
    $coupon = commerce_coupon_create($type);
    $coupon->log = 'Created by FeedsCommerceCouponProcessor';
    return $coupon;
  }

  /**
   * Loads an existing coupon.
   */
  protected function entityLoad(FeedsSource $source, $coupon_id) {
    $coupon = parent::entityLoad($source, $coupon_id);
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $coupon->log = 'Updated by FeedsCommerceCouponProcessor';
    }
    else {
      $coupon->log = 'Replaced by FeedsCommerceCouponProcessor';
    }
    return $coupon;
  }

  /**
   * Save a user account.
   */
  protected function entitySave($entity) {
    commerce_coupon_save($entity);
  }

  /**
   * Delete multiple user accounts.
   */
  protected function entityDeleteMultiple($ids) {
    commerce_coupon_delete_multiple($ids);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $types = commerce_coupon_type_get_name();
    return array(
      'coupon_type' => key($types),
      'is_active' => TRUE,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);

    $types = commerce_coupon_type_get_name();
    $form['coupon_type'] = array(
      '#type' => 'select',
      '#title' => t('Coupon type'),
      '#description' => t('Select the coupon type for the coupon to be created.'),
      '#options' => $types,
      '#default_value' => $this->config['coupon_type'],
    );
    $form['is_active'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#description' => t('Select whether coupons should be imported active or not.'),
      '#options' => array(0 => t('Inactive'), 1 => t('Active')),
      '#default_value' => $this->config['is_active'],
    );

    return $form;
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();

    // Let other modules expose mapping targets.
    self::loadMappers();
    $entity_type = $this->entityType();
    $bundle = $this->config['coupon_type'];
    drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);

    return $targets;
  }

  /**
   * Get id of an existing feed item term if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($coupon_id = parent::existingEntityId($source, $result)) {
      return $coupon_id;
    }

    // Iterate through all unique targets and try to find a coupon for the
    // target's value.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'coupon_code':
          $query = new EntityFieldQuery();
          $query->entityCondition('entity_type', 'commerce_coupon')
            ->fieldCondition('commerce_coupon_code', 'value', $value, '=');


          $coupon = $query->execute();

          if ($coupon == NULL || !isset($coupon['commerce_coupon']) || !is_array($coupon['commerce_coupon'])) {
            return NULL;
          }

          $coupon = reset($coupon['commerce_coupon']);

          if (isset($coupon) && isset($coupon->coupon_id)) {
            return $coupon->coupon_id;
          }
          break;
      }
    }
    return 0;
  }

}
