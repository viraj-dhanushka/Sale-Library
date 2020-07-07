<?php

/**
 * @file
 * Define field and field intances, attach them to entity bundles.
 */
function mailjet_subscription_get_forms_field_data() {

  $signup_forms_fields = array(
    'signup_form_description' => array(
      'field' => array(
        'field_name' => 'signup_description', // field name
        'entity_types' => array('signup_units_data'), // entity type to which the field is attached
        'cardinality' => 1, // number of values to be saved at a time
        'type' => 'text', // type of field
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_description',
        'label' => 'Description',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'The description',
      ),
    ),
    'signup_submit_label2' => array(
      'field' => array(
        'field_name' => 'signup_submit_label2',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_submit_label2',
        'label' => 'Field - Submit Button Label',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - Submit Button Label',
      ),
    ),
    'signup_destination_field' => array(
      'field' => array(
        'field_name' => 'signup_destination_field',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_destination_field',
        'label' => 'Field - Form destination page',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - Form destination page ',
      ),
    ),
    'signup_confirmation_mess' => array(
      'field' => array(
        'field_name' => 'signup_confirmation_mess',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_confirmation_mess',
        'label' => 'Field - Confirmation Message',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - Confirmation Message ',
      ),
    ),
    'signup_error_token_field' => array(
      'field' => array(
        'field_name' => 'signup_error_token_field',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_error_token_field',
        'label' => 'Field - Error. Token verification failed. ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field -  Error. Token verification failed. ',
      ),
    ),
    'signup_missmatch_error_field' => array(
      'field' => array(
        'field_name' => 'signup_missmatch_error_field',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_missmatch_error_field',
        'label' => 'Field - signup_missmatch_error_field ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field -  signup_missmatch_error_field ',
      ),
    ),
    'signup_confirmation_email' => array(
      'field' => array(
        'field_name' => 'signup_confirmation_email',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_confirmation_email',
        'label' => 'Field - Confirmation email text ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - Confirmation email text. ',
      ),
    ),
    'signup_subscribe_error' => array(
      'field' => array(
        'field_name' => 'signup_subscribe_error',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_subscribe_error',
        'label' => 'Field - Subscribe error. ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field -  Subscribe error.',
      ),
    ),
    'signup_subscribe_exist_string' => array(
      'field' => array(
        'field_name' => 'signup_subscribe_exist_string',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 400),
      ),
      'instance' => array(
        'field_name' => 'signup_subscribe_exist_string',
        'label' => 'Field - The contact % is already subscribed  ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field -  The contact % is already subscribed ',
      ),
    ),
    'email_text_button' => array(
      'field' => array(
        'field_name' => 'signup_email_text_button',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 400),
      ),
      'instance' => array(
        'field_name' => 'signup_email_text_button',
        'label' => 'Field - email_text_button  ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field -  email_text_button ',
      ),
    ),
    'email_text_thank_you' => array(
      'field' => array(
        'field_name' => 'signup_email_text_thank_you',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 400),
      ),
      'instance' => array(
        'field_name' => 'signup_email_text_thank_you',
        'label' => 'Field - email_text_thank_you ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field -  email_text_thank_you ',
      ),
    ),
    'email_text_description' => array(
      'field' => array(
        'field_name' => 'signup_email_text_description',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 400),
      ),
      'instance' => array(
        'field_name' => 'signup_email_text_description',
        'label' => 'Field - email_text_description ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field -  email_text_description ',
      ),
    ),
    'signup_footer_email_description' => array(
      'field' => array(
        'field_name' => 'signup_footer_email_description',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 400),
      ),
      'instance' => array(
        'field_name' => 'signup_footer_email_description',
        'label' => 'Field - signup_footer_email_description',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field -  ssignup_footer_email_description ',
      ),
    ),
    'email_owner' => array(
      'field' => array(
        'field_name' => 'signup_email_owner',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_email_owner',
        'label' => 'Field - email_owner ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - email_owner ',
      ),
    ),
    'signup_succesfull_subscribe' => array(
      'field' => array(
        'field_name' => 'signup_succesfull_subscribe',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_succesfull_subscribe',
        'label' => 'Field - Thanks for subscribing with %s  ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - Thanks for subscribing with %s  ',
      ),
    ),
    'signup_succesfull_subscribe' => array(
      'field' => array(
        'field_name' => 'signup_succesfull_subscribe',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_succesfull_subscribe',
        'label' => 'Field - Thanks for subscribing with %s  ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - Thanks for subscribing with %s  ',
      ),
    ),
    'signup_double_opt_in' => array(
      'field' => array(
        'field_name' => 'signup_double_opt_in',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'number_integer',
      ),
      'instance' => array(
        'field_name' => 'signup_double_opt_in',
        'label' => 'Field - subscribers - Double Opt-in   ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - subscribers - Double Opt-in   ',
      ),
    ),
    'signup_fields_form' => array(
      'field' => array(
        'field_name' => 'signup_fields_form',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_fields_form',
        'label' => 'Field - Mailjet Fields ',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - Maijet Fields ',
      ),
    ),
    'signup_contact_lists' => array(
      'field' => array(
        'field_name' => 'signup_contact_lists',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 255),
      ),
      'instance' => array(
        'field_name' => 'signup_contact_lists',
        'label' => 'Field - Contact list for subcribe',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - Contact list for subcribe ',
      ),
    ),
    'signup_field_css' => array(
      'field' => array(
        'field_name' => 'signup_field_css',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 8000),
      ),
      'instance' => array(
        'field_name' => 'signup_field_css',
        'label' => 'Field - about  inline CSS',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - about inline CSS ',
      ),
    ),
    'signup_field_js' => array(
      'field' => array(
        'field_name' => 'signup_field_js',
        'entity_types' => array('signup_units_data'),
        'cardinality' => 1,
        'type' => 'text',
        'settings' => array('max_length' => 8000),
      ),
      'instance' => array(
        'field_name' => 'signup_field_js',
        'label' => 'Field - about inline JS',
        'entity_type' => 'signup_units_data',
        'bundle' => 'main',
        'description' => 'Field - about inline JS',
      ),
    ),
  );

  return $signup_forms_fields;
}

/**
 * Create fields and field instances for training entity type.
 */
function mailjet_subscription_create_forms_fields() {
  // Define field and field instances.
  $signup_forms_fields = mailjet_subscription_get_forms_field_data();

  foreach ($signup_forms_fields as $field_data) {
    $field = $field_data['field'];
    // Create field if not exists.
    if (!field_read_field($field['field_name'], array('include_inactive' => TRUE))) {
      field_create_field($field);
    }

    $instance = $field_data['instance'];
    // Create field instance if not exists.
    if (!field_read_instance($instance['entity_type'], $instance['field_name'], $instance['bundle'], array('include_inactive' => TRUE))) {
      field_create_instance($instance);
    }
  }
}
