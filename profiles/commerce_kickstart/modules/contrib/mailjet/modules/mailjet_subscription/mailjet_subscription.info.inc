<?php

/* Extend the defaults.
 * 
 * Entity metadata controller about store,edit,delete data of Signup block forms
 */

class SignupFormMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['created'] = array(
        'label' => t('Created'),
        'schema field' => 'created',
        'getter callback' => 'entity_property_getter_method',
        'setter callback' => 'entity_property_verbatim_set',
        'required' => TRUE,
        'description' => t('created date of the assessment data.'),
    );
    $properties['updated'] = array(
        'label' => 'Signup form updated',
        'schema field' => t('updated'),
        'getter callback' => 'entity_property_getter_method',
        'setter callback' => 'entity_property_verbatim_set',
        'required' => TRUE,
        'description' => t('updated date of the assessment data.'),
    );
    $properties['title'] = array(
        'label' => t('Signup form title'),
        'schema field' => 'title',
        'getter callback' => 'entity_property_getter_method',
        'setter callback' => 'entity_property_verbatim_set',
        'required' => TRUE,
        'description' => t('title of  data.'),
    );
    $properties['type'] = array(
        'type' => 'text',
        'label' => t('Type'),
        'description' => t('The human readable name of the bundle type.'),
        'setter callback' => 'entity_property_verbatim_set',
        'getter callback' => 'entity_property_getter_method',
        'options list' => 'mymodule_type_options_list',
        'required' => TRUE,
        'schema field' => 'type',
    );

    return $info;
  }

}
