/**
 * @file
 * Javascript to generate Accept.js token in PCI-compliant way.
 */

(function ($, Drupal) {

  'use strict';

  /**
   * Authorize.net accept.js form js.
   */
  Drupal.commerceAuthorizeNetAcceptForm = function ($form, settings) {
    var last4 = '';
    // To be used to temporarily store month and year.
    var expiration = {};

    var $submit = $form.find('[id="edit-continue"]');
    $submit.attr('disabled', false);

    // Sends the card data to Authorize.Net and receive the payment nonce in
    // response.
    var sendPaymentDataToAnet = function (event) {
      var secureData = {};
      var authData = {};
      var cardData = {};

      // Extract the card number, expiration date, and card code.
      cardData.cardNumber = $('#credit-card-number').val().replace(/\s/g, '');
      cardData.month = $('#expiration-month').val();
      cardData.year = $('#expiration-year').val();
      cardData.cardCode = $('#cvv').val();
      secureData.cardData = cardData;

      // The Authorize.Net Client Key is used in place of the traditional
      // Transaction Key. The Transaction Key is a shared secret and must never
      // be exposed. The Client Key is a public key suitable for use where
      // someone outside the merchant might see it.
      authData.clientKey = settings.clientKey;
      authData.apiLoginID = settings.apiLoginID;
      secureData.authData = authData;

      // Pass the card number and expiration date to Accept.js for submission
      // to Authorize.Net.
      Accept.dispatchData(secureData, responseHandler);
    };

    // Process the response from Authorize.Net to retrieve the two elements of
    // the payment nonce. If the data looks correct, record the OpaqueData to
    // the console and call the transaction processing function.
    var responseHandler = function (response) {
      if (response.messages.resultCode === 'Error') {
        for (var i = 0; i < response.messages.message.length; i++) {
          Drupal.commerceAuthorizeNet.errorDisplay(response.messages.message[i].code, response.messages.message[i].text);
        }
        $form.find('button').attr('disabled', false);
      }
      else {
        processTransactionDataFromAnet(response.opaqueData);
      }
    };

    var processTransactionDataFromAnet = function (responseData) {
      $('.accept-js-data-descriptor', $form).val(responseData.dataDescriptor);
      $('.accept-js-data-value', $form).val(responseData.dataValue);

      $('.accept-js-data-last4', $form).val(last4);
      $('.accept-js-data-month', $form).val(expiration.month);
      $('.accept-js-data-year', $form).val(expiration.year);

      // Clear out the values so they don't get posted to Drupal. They would
      // never be used, but for PCI compliance we should never send them at.
      $('#credit-card-number').val('');
      $('#expiration-month').val('');
      $('#expiration-year').val('');
      $('#cvv').val('');

      // Submit the form.
      $form.get(0).submit({ 'populated': true });
    };

    // Form submit.
    $form.bind('submit.authnet', function (event, options) {
      event.preventDefault();
      // Disable the submit button to prevent repeated clicks.
      $form.find('button').attr('disabled', true);
      options = options || {};
      if (options.populated) {
        return;
      }

      // Store last4 digit.
      var credit_card_number = $('#credit-card-number').val();
      last4 = credit_card_number.substr(credit_card_number.length - 4);
      expiration = {
        month: $('#expiration-month').val(),
        year: $('#expiration-year').val()
      };

      // Send payment data to anet.
      sendPaymentDataToAnet(event);
      return false;
    });
  };

})(jQuery, Drupal);
