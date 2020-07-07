<?php

/**
 * @file
 * Includes the Accept.js payment method callbacks.
 */

/**
 * Returns the default settings for the Authorize.Net AIM payment method.
 */
function commerce_authnet_acceptjs_default_settings() {
  return array(
    'login' => '',
    'tran_key' => '',
    'client_key' => '',
    'txn_mode' => AUTHNET_TXN_MODE_LIVE_TEST,
    'txn_type' => COMMERCE_CREDIT_AUTH_CAPTURE,
    'cardonfile' => FALSE,
    'continuous' => FALSE,
    'email_customer' => FALSE,
    'log' => array('request' => '0', 'response' => '0'),
    'card_types' => array(),
  );
}

/**
 * Payment method callback: settings form.
 */
function commerce_authnet_acceptjs_settings_form($settings = NULL) {
  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_authnet_acceptjs_default_settings();

  $form = array();

  $form['login'] = array(
    '#type' => 'textfield',
    '#title' => t('API Login ID'),
    '#description' => t('Your API Login ID is different from the username you use to login to your Authorize.Net account. Once you login, browse to your Account tab and click the <em>API Login ID and Transaction Key</em> link to find your API Login ID. If you are using a new Authorize.Net account, you may still need to generate an ID.'),
    '#default_value' => $settings['login'],
    '#required' => TRUE,
  );
  $form['tran_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Transaction Key'),
    '#description' => t('Your Transaction Key can be found on the same screen as your API Login ID. However, it will not be readily displayed. You must answer your security question and submit a form to see your Transaction Key.'),
    '#default_value' => $settings['tran_key'],
    '#required' => TRUE,
  );
  $form['client_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Key'),
    '#description' => t('Follow the instructions <a href="https://developer.authorize.net/api/reference/features/acceptjs.html#Obtaining_a_Public_Client_Key">here</a> to get a client key.'),
    '#default_value' => $settings['client_key'],
    '#required' => TRUE,
  );
  $form['txn_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Transaction mode'),
    '#description' => t('Adjust to live transactions when you are ready to start processing real payments.') . '<br />' . t('Only specify a developer test account if you login to your account through https://test.authorize.net.'),
    '#options' => array(
      AUTHNET_TXN_MODE_LIVE => t('Live transactions in a live account'),
      AUTHNET_TXN_MODE_LIVE_TEST => t('Test transactions in a live account'),
      AUTHNET_TXN_MODE_DEVELOPER => t('Developer test account transactions'),
    ),
    '#default_value' => $settings['txn_mode'],
  );
  $form['txn_type'] = array(
    '#type' => 'radios',
    '#title' => t('Default credit card transaction type'),
    '#description' => t('The default will be used to process transactions during checkout.'),
    '#options' => array(
      COMMERCE_CREDIT_AUTH_CAPTURE => t('Authorization and capture'),
      COMMERCE_CREDIT_AUTH_ONLY => t('Authorization only (requires manual or automated capture after checkout)'),
    ),
    '#default_value' => $settings['txn_type'],
  );

  // A requires the Card on File module.
  if (module_exists('commerce_cardonfile')) {
    $form['cardonfile'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Card on File functionality with this payment method using Authorize.Net CIM.'),
      '#description' => t('This requires an Authorize.Net account upgraded to include support for CIM (Customer Information Manager).'),
      '#default_value' => $settings['cardonfile'],
    );

    $form['continuous'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use continuous authority transactions.'),
      '#description' => t('A continuous authority merchant account will be required.'),
      '#default_value' => $settings['continuous'],
    );
  }
  else {
    $form['cardonfile'] = array(
      '#type' => 'markup',
      '#markup' => t('To enable Card on File funcitionality download and install the Card on File module.'),
    );
  }

  $form['email_customer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tell Authorize.net to e-mail the customer a receipt based on your account settings.'),
    '#default_value' => $settings['email_customer'],
  );
  $form['log'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => $settings['log'],
  );

  return $form;
}

/**
 * Payment method callback: checkout form.
 */
function commerce_authnet_acceptjs_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  return commerce_authnet_acceptjs_form_elements($payment_method);
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_authnet_acceptjs_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  // If the customer specified a card on file, skip the normal validation.
  if (module_exists('commerce_cardonfile') && !empty($payment_method['settings']['cardonfile']) &&
    !empty($pane_values['cardonfile']) && $pane_values['cardonfile'] !== 'new') {
    return;
  }
  $values = $pane_values['credit_card']['cc'];
  if (empty($pane_values['credit_card']['cc']['data_descriptor']) || empty($pane_values['credit_card']['cc']['data_value'])) {
    drupal_set_message('There was an error', 'error');
    return FALSE;
  }
  return TRUE;
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_authnet_acceptjs_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {

  // If txn_type has been specified in the pane values array, such as through
  // the special select element we alter onto the payment terminal form, use
  // that instead.
  if (!empty($pane_values['txn_type'])) {
    $payment_method['settings']['txn_type'] = $pane_values['txn_type'];
  }

  if (module_exists('commerce_cardonfile') && $payment_method['settings']['cardonfile'] && !empty($pane_values['cardonfile'])) {
    // @TODO SUPPORT COF!
    if (!empty($pane_values['cardonfile']) && $pane_values['cardonfile'] !== 'new') {
      // We're using a stored payment profile. Pass it along to CIM.
      return commerce_authnet_cim_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge);
    }
    else {
      if (!empty($pane_values['credit_card']['cardonfile_store']) && $pane_values['credit_card']['cardonfile_store']) {
        // We've got a request to store a new card.
        return commerce_authnet_acceptjs_submit_new_card_form_submit($payment_method, $pane_form, $pane_values, $order, $charge);
      }
    }
  }
  $values = $pane_values['credit_card']['cc'];
  $data_descriptor = $values['data_descriptor'];
  $data_value = $values['data_value'];

  // https://developer.authorize.net/api/reference/#payment-transactions-create-an-accept-payment-transaction
  $request = array(
    'createTransactionRequest' => array(
      'merchantAuthentication' => array(
        'name' => $payment_method['settings']['login'],
        'transactionKey' => $payment_method['settings']['tran_key'],
      ),
      'clientId' => 'CG-PHP-SDK',
      'refId' => 'ref' . time(),
      'transactionRequest' => array(
        'transactionType' => ($payment_method['settings']['txn_type'] == COMMERCE_CREDIT_AUTH_CAPTURE) ? 'authCaptureTransaction' : 'authOnlyTransaction',
        'amount' => number_format(commerce_currency_amount_to_decimal($charge['amount'], $charge['currency_code']), 2, '.', ''),
        'payment' => array(
          'opaqueData' => array(
            'dataDescriptor' => $data_descriptor,
            'dataValue' => $data_value,
          ),
        ),
        'solution' => array(
          'id' => 'A1000009',
        ),
      ),
    ),
  );

  if (!empty($order)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $request['createTransactionRequest']['transactionRequest']['order']['invoiceNumber'] = $order->order_number;
    $description = array();
    // Descriptions come from products, though not all environments have them.
    // So check first.
    if (function_exists('commerce_product_line_item_types')) {
      foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
        if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
          $description[] = round($line_item_wrapper->quantity->value(), 2) . 'x ' . $line_item_wrapper->line_item_label->value();
          $request['createTransactionRequest']['transactionRequest']['lineItems']['lineItem'][] = array(
            'itemId' => drupal_substr($line_item_wrapper->line_item_label->value(), 0, 31),
            'name' => drupal_substr($line_item_wrapper->commerce_product->title->value(), 0, 31),
            'quantity' => round($line_item_wrapper->quantity->value(), 2),
            'unitPrice' => commerce_currency_amount_to_decimal($line_item_wrapper->commerce_unit_price->amount->value(), $line_item_wrapper->commerce_unit_price->currency_code->value()),
          );
        }
      }
    }
    $request['createTransactionRequest']['transactionRequest']['order']['description'] = substr(implode(', ', $description), 0, 255);
    $request['createTransactionRequest']['transactionRequest']['customer'] = array(
      'id' => substr($order->uid, 0, 20),
      'email' => substr($order->mail, 0, 255),
    );
    if (isset($order->commerce_customer_billing)) {
      $request['createTransactionRequest']['transactionRequest']['billTo'] = commerce_authnet_cim_billto_array($order);
    }
    if (isset($order->commerce_customer_shipping)) {
      $request['createTransactionRequest']['transactionRequest']['shipTo'] = commerce_authnet_cim_shipto_array($order);
    }
    $request['createTransactionRequest']['transactionRequest']['customerIP'] = substr(ip_address(), 0, 15);
  }

  $request_json = drupal_json_encode($request);

  if ($payment_method['settings']['txn_mode'] == AUTHNET_TXN_MODE_DEVELOPER) {
    $api_url = 'https://apitest.authorize.net/xml/v1/request.api';
  }
  else {
    $api_url = 'https://api.authorize.net/xml/v1/request.api';
  }

  // Setup the cURL request.
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $api_url);
  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $request_json);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 1);
  curl_setopt($ch, CURLOPT_NOPROGRESS, 1);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array(
    'Content-Type: application/json',
    'Content-Length: ' . strlen($request_json),
  ));
  $result_json = curl_exec($ch);

  // Log any errors to the watchdog.
  if ($error = curl_error($ch)) {
    watchdog('commerce_authnet', 'cURL error: @error', array('@error' => $error), WATCHDOG_ERROR);
    curl_close($ch);
    return FALSE;
  }
  curl_close($ch);
  // The API returns hidden Byte Order Mark characters, causing the
  // JSON decoding to fail.
  $result_json = preg_replace('/[\x00-\x1F\x80-\xFF]/', '', $result_json);
  $result = drupal_json_decode($result_json);

  // Prepare a transaction object to log the API response.
  $transaction = commerce_payment_transaction_new('authnet_acceptjs', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->remote_id = $result['transactionResponse']['transId'];
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->payload[REQUEST_TIME] = $result_json;

  if (empty($result['messages']['resultCode']) || $result['messages']['resultCode'] != 'Ok') {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction_message = array();
    foreach ($result['messages']['message'] as $message) {
      $transaction_message[] = t('Authorize.net error: @code - @error', array(
        '@code' => $message['code'],
        '@error' => $message['text'],
      ));
      watchdog('commerce_authnet', 'Authorize.net error: @code - @error', array(
        '@code' => $message['code'],
        '@error' => $message['text'],
      ), WATCHDOG_ERROR);
      $transaction->message = implode('<br />', $transaction_message);
    }
    commerce_payment_transaction_save($transaction);
    return FALSE;
  }
  elseif (!empty($result['transactionResponse']['errors'])) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction_message = array();
    foreach ($result['transactionResponse']['errors'] as $message) {
      $transaction_message[] = t('Authorize.net error: @code - @error', array(
        '@code' => $message['errorCode'],
        '@error' => $message['errorText'],
      ));
      watchdog('commerce_authnet', 'Authorize.net error: @code - @error', array(
        '@code' => $message['errorCode'],
        '@error' => $message['errorText'],
      ), WATCHDOG_ERROR);
      $transaction->message = implode('<br />', $transaction_message);
      drupal_set_message(check_plain($message['errorText']), 'error');
    }
    commerce_payment_transaction_save($transaction);
    return FALSE;
  }

  // Set the transaction status based on the type of transaction this was.
  switch ($payment_method['settings']['txn_type']) {
    case COMMERCE_CREDIT_AUTH_ONLY:
      $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
      break;

    case COMMERCE_CREDIT_AUTH_CAPTURE:
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      break;

    case COMMERCE_CREDIT_CAPTURE_ONLY:
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      break;
  }

  $transaction->remote_status = commerce_authnet_txn_type($payment_method['settings']['txn_type']);

  // Build a meaningful response message.
  $message = array(
    '<b>' . commerce_authnet_reverse_txn_type($transaction->remote_status) . '</b>',
    '<b>' . ($result['transactionResponse']['messages'][0]['code'] != '1' ? t('REJECTED') : t('ACCEPTED')) . ':</b> ' . check_plain($result['transactionResponse']['messages'][0]['description']),
    t('AVS response: @avs', array('@avs' => commerce_authnet_avs_response($result['transactionResponse']['avsResultCode']))),
  );

  // Add the CVV response if enabled.
  if (isset($result['transactionResponse']['cvvResultCode'])) {
    $message[] = t('CVV match: @cvv', array('@cvv' => commerce_authnet_cvv_response($result['transactionResponse']['cvvResultCode'])));
  }

  $transaction->message = implode('<br />', $message);

  commerce_payment_transaction_save($transaction);

  return TRUE;
}

/**
 * Handles advanced logic relating to creating AcceptJS orders with new card data.
 *
 * @see commerce_authnet_aim_submit_form_submit()
 */
function commerce_authnet_acceptjs_submit_new_card_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // At this point, we have a few choices to make. We know the user is logged in
  // and so we know if they have any existing card on file profiles. If they do
  // not, then we can assume that they need a new profile. If they do have one,
  // then we need take appropriate steps to add the CC info to the existing
  // profile and process the payment based on their profile.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  $payment_details = array(
    'opaqueData' => array(
      'dataDescriptor' => $pane_values['credit_card']['cc']['data_descriptor'],
      'dataValue' => $pane_values['credit_card']['cc']['data_value'],
    ),
  );

  // Prepare the billing address for use in the request.
  if (isset($order->commerce_customer_billing) && $order_wrapper->commerce_customer_billing->value()) {
    $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();

    if (empty($billing_address['first_name'])) {
      $name_parts = explode(' ', $billing_address['name_line']);
      $billing_address['first_name'] = array_shift($name_parts);
      $billing_address['last_name'] = implode(' ', $name_parts);
    }
  }

  $remote_id = FALSE;

  // First look to see if we already have cards on file for the user.
  $stored_cards = array();
  if (!user_is_anonymous()) {
    $stored_cards = commerce_cardonfile_load_multiple_by_uid($order->uid, $payment_method['instance_id']);
  }
  $customer_profile_id = NULL;

  if (empty($stored_cards)) {
    // We do not, create the profile (which includes the payment details).
    $response = commerce_authnet_acceptjs_create_customer_profile_request($payment_method, $order);
    // If the Customer Profile creation was a success, store the new card on
    // file data locally.
    if ((string) $response->messages->resultCode == 'Ok') {
      $customer_profile_id = (string) $response->customerProfileId;
    }
    elseif ((string) $response->messages->message->code == 'E00039') {
      // But if a Customer Profile already existed for this user, attempt
      // instead to add this card as a new Payment Profile to it.
      $result = array_filter(explode(' ', (string) $response->messages->message->text), 'is_numeric');
      $customer_profile_id = reset($result);
    }
    // The profile could not be created, exit.
    else {
      return FALSE;
    }
  }
  else {
    // Extract the user's Customer Profile ID from the first card's remote ID.
    $card_data = reset($stored_cards);
    list($customer_profile_id) = explode('|', $card_data->remote_id);
  }

  // Attempt to add the card to an existing Customer Profile if specified.
  if (!empty($customer_profile_id)) {
    $response = commerce_authnet_acceptjs_create_customer_payment_profile_request($payment_method, $customer_profile_id, $order, $payment_details);

    if ((string) $response->messages->resultCode == 'Ok') {
      $payment_profile_id = (string) $response->customerPaymentProfileId;
    }
    elseif ((string) $response->messages->message->code == 'E00039') {
      if (empty($response->customerPaymentProfileId)) {
        return FALSE;
      }
      $payment_profile_id = $response->customerPaymentProfileId;
    }
    elseif ($response->messages->message->code == 'E00040') {
      // But if we could not find a customer profile, create a new one.
      $response = commerce_authnet_acceptjs_create_customer_profile_request($payment_method, $order, $payment_details);
      // If the Customer Profile creation was a success, store the new card on
      // file data locally.
      if ((string) $response->messages->resultCode == 'Ok') {
        // Build a remote ID that includes the Customer Profile ID and the
        // Payment Profile ID.
        $customer_profile_id = (string) $response->customerProfileId;
        $payment_profile_id = (string) $response->customerPaymentProfileIdList->numericString;
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }

    $remote_id = $customer_profile_id . '|' . $payment_profile_id;
  }
  // Build our card for storing with card on file.
  /** @var \stdClass $card_data */
  $card_data = commerce_cardonfile_new();
  $card_data->uid = $order->uid;
  $card_data->payment_method = $payment_method['method_id'];
  $card_data->instance_id = $payment_method['instance_id'];
  $card_data->remote_id = $remote_id;
  $card_data->card_type = !empty($card_type) ? $card_type : 'card';
  $card_data->card_name = !empty($billing_address['name_line']) ? $billing_address['name_line'] : '';
  $card_data->card_number = $pane_values['credit_card']['cc']['last4'];
  $card_data->card_exp_month = $pane_values['credit_card']['cc']['expiration_month'];
  $card_data->card_exp_year = substr(date('Y'), 0, 2) . $pane_values['credit_card']['cc']['expiration_year'];
  $card_data->status = 1;

  if (isset($pane_values['cardonfile_instance_default'])) {
    $card_data->instance_default = $pane_values['cardonfile_instance_default'];
  }

  // Save and log the creation of the new card on file.
  commerce_cardonfile_save($card_data);

  if ($customer_profile_id) {
    watchdog('commerce_authnet', 'CIM Payment Profile added to Customer Profile @profile_id for user @uid.', array('@profile_id' => $customer_profile_id, '@uid' => $order->uid));
  }
  else {
    watchdog('commerce_authnet', 'CIM Customer Profile @profile_id created and saved to user @uid.', array('@profile_id' => (string) $response->customerProfileId, '@uid' => $order->uid));
  }

  // Process the payment.
  return commerce_authnet_cim_cardonfile_charge($payment_method, $card_data, $order, $charge);
}

/**
 * Submits a createCustomerProfileRequest XML CIM API request to Authorize.Net.
 *
 * This function will attempt to create a CIM Customer Profile and a default
 * Payment Profile for it using the given payment details.
 *
 * @param $payment_method
 *   The payment method instance array containing the API credentials for a CIM
 *   enabled Authorize.Net account.
 * @param $order
 *   The order object containing the billing address and e-mail to use for the
 *   customer profile.
 * @param $payment_details
 *   An array of payment details to use in the default payment profile. See the
 *   respective helper array functions for possible keys.
 *
 * @return
 *   A SimpleXMLElement containing the API response.
 *
 * @see commerce_authnet_cim_credit_card_array()
 */
function commerce_authnet_acceptjs_create_customer_profile_request($payment_method, $order, $payment_details = array()) {
  $billto = commerce_authnet_cim_billto_array($order);

  // Build the base profile request data.
  $api_request_data = array(
    'profile' => array(
      'merchantCustomerId' => $order->uid,
      'description' => $billto['firstName'] . ' ' . $billto['lastName'],
      'email' => $order->mail,
    ),
  );

  if (!empty($payment_details)) {
    $api_request_data['profile']['paymentProfiles'] = array(
      'billTo' => $billto,
      'payment' => array(),
    );
  }

  // Add the shipping address if available.
  if (isset($order->commerce_customer_shipping)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    if ($order_wrapper->commerce_customer_shipping->value()) {
      $api_request_data['profile']['shipToList'] = commerce_authnet_cim_shipto_array($order);
    }
  }

  // If the order is anonymous, unset the merchantCustomerId from the request.
  if (empty($api_request_data['profile']['merchantCustomerId'])) {
    unset($api_request_data['profile']['merchantCustomerId']);
  }

  if (!empty($payment_details['opaqueData'])) {
    $api_request_data['profile']['paymentProfiles']['payment'] = $payment_details;
  }

  return commerce_authnet_cim_request($payment_method, 'createCustomerProfileRequest', $api_request_data);
}

/**
 * Submits a createCustomerPaymentProfileRequest XML CIM API request.
 *
 * @param $payment_method
 *   The payment method instance array containing the API credentials for a CIM
 *   enabled Authorize.Net account.
 * @param $cim_customer_profile_id
 *   A numerical CIM Customer Profile ID.
 * @param $order
 *   The order object containing the billing address and e-mail to use for the
 *   payment profile.
 * @param $payment_details
 *   An array of payment details to use in the default payment profile. See the
 *   respective helper array functions for possible keys.
 *
 * @return
 *   A SimpleXMLElement containing the API response.
 *
 * @see commerce_authnet_cim_credit_card_array()
 */
function commerce_authnet_acceptjs_create_customer_payment_profile_request($payment_method, $cim_customer_profile_id, $order, $payment_details) {
  $billto = commerce_authnet_cim_billto_array($order);

  // Build the base profile request data.
  $api_request_data = array(
    'customerProfileId' => $cim_customer_profile_id,
    'paymentProfile' => array(
      'billTo' => $billto,
      'payment' => array(),
    ),
  );

  if (!empty($payment_details['opaqueData'])) {
    $api_request_data['paymentProfile']['payment'] = $payment_details;
  }

  return commerce_authnet_cim_request($payment_method, 'createCustomerPaymentProfileRequest', $api_request_data);
}

/**
 * Return the form elements for Accept.js.
 *
 * @param array $payment_method
 *   The payment method.
 *
 * @return array
 *   The form elements.
 */
function commerce_authnet_acceptjs_form_elements(array $payment_method) {
  $form = array();
  // @todo change to proper library.
  if ($payment_method['settings']['txn_mode'] == AUTHNET_TXN_MODE_DEVELOPER) {
    $form['#attached']['library'][] = array('commerce_authnet', 'commerce_authnet.acceptjs.sandbox');
  }
  else {
    $form['#attached']['library'][] = array('commerce_authnet', 'commerce_authnet.acceptjs.production');
  }
  $form['#attached']['library'][] = array('commerce_authnet', 'commerce_authnet.acceptjs.accept');
  $form['#attached']['css'][] = drupal_get_path('module', 'commerce_payment') . '/theme/commerce_payment.theme.css';

  $js_settings = array(
    'clientKey' => $payment_method['settings']['client_key'],
    'apiLoginID' => $payment_method['settings']['login'],
    'paymentMethodType' => 'credit_card',
  );
  // Allow other modules to alter the JS settings.
  drupal_alter('commerce_authnet_acceptjs_settings', $js_settings, $payment_method);
  $form['#attached']['js'][] = array(
    'data' => array(
      'commerceAuthorizeNet' => $js_settings,
    ),
    'type' => 'setting',
  );
  $form['credit_card'] = array(
    '#type' => 'container',
  );
  $form['credit_card']['#attributes']['class'][] = 'acceptjs-form';
  $form['credit_card']['errors'] = array(
    '#type' => 'hidden',
  );
  $form['credit_card']['cc'] = array(
    '#type' => 'container',
  );
  // Fields placeholder to be built by the JS.
  $form['credit_card']['cc']['credit_card_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Card number'),
    '#attributes' => array(
      'placeholder' => '•••• •••• •••• ••••',
      'autocomplete' => 'off',
      'autocorrect' => 'off',
      'autocapitalize' => 'none',
      'id' => 'credit-card-number',
      'required' => 'required',
    ),
    '#label_attributes' => array(
      'class' => array(
        'js-form-required',
        'form-required',
      ),
    ),
    '#maxlength' => 20,
    '#size' => 20,
  );

  $form['credit_card']['cc']['expiration'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'credit-card-form__expiration',
        'container-inline',
      ),
    ),
  );
  $form['credit_card']['cc']['expiration']['month'] = array(
    '#type' => 'textfield',
    '#title' => t('Month'),
    '#attributes' => array(
      'placeholder' => 'MM',
      'autocomplete' => 'off',
      'autocorrect' => 'off',
      'autocapitalize' => 'none',
      'id' => 'expiration-month',
      'required' => 'required',
    ),
    '#label_attributes' => array(
      'class' => array(
        'js-form-required',
        'form-required',
      ),
    ),
    '#maxlength' => 2,
    '#size' => 3,
  );

  $form['credit_card']['cc']['expiration']['divider'] = array(
    '#type' => 'item',
    '#title' => '',
    '#markup' => '<span class="credit-card-form__divider">/</span>',
  );
  $form['credit_card']['cc']['expiration']['year'] = array(
    '#type' => 'textfield',
    '#title' => t('Year'),
    '#attributes' => array(
      'placeholder' => 'YY',
      'autocomplete' => 'off',
      'autocorrect' => 'off',
      'autocapitalize' => 'none',
      'id' => 'expiration-year',
      'required' => 'required',
    ),
    '#label_attributes' => array(
      'class' => array(
        'js-form-required',
        'form-required',
      ),
    ),
    '#maxlength' => 2,
    '#size' => 3,
  );

  $form['credit_card']['cc']['security_code'] = array(
    '#type' => 'textfield',
    '#title' => t('CVV'),
    '#attributes' => array(
      'placeholder' => '•••',
      'autocomplete' => 'off',
      'autocorrect' => 'off',
      'autocapitalize' => 'none',
      'id' => 'cvv',
      'required' => 'required',
    ),
    '#label_attributes' => array(
      'class' => array(
        'js-form-required',
        'form-required',
      ),
    ),
    '#maxlength' => 4,
    '#size' => 4,
  );

  // To display validation errors.
  $form['credit_card']['cc']['payment_errors'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="payment-errors"></div>',
    '#weight' => -200,
  );

  // Populated by the JS library after receiving a response from AuthorizeNet.
  $form['credit_card']['cc']['data_descriptor'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'class' => array('accept-js-data-descriptor'),
    ),
  );
  $form['credit_card']['cc']['data_value'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'class' => array('accept-js-data-value'),
    ),
  );
  $form['credit_card']['cc']['last4'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'class' => array('accept-js-data-last4'),
    ),
  );
  $form['credit_card']['cc']['expiration_month'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'class' => array('accept-js-data-month'),
    ),
  );
  $form['credit_card']['cc']['expiration_year'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'class' => array('accept-js-data-year'),
    ),
  );
  return $form;
}
