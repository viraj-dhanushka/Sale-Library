(function (Drupal, $) {
  /**
   * Attaches the comerceAuthorizeNetForm behavior.
   */
  Drupal.behaviors.commerceAuthorizeNetForm = {
    attach: function (context, settings) {
      var $form = $('.acceptjs-form', context).closest('form').once('acceptjs-processed');
      if ($form.length === 0) {
        return;
      }


      var $submit = $form.find('[id="edit-continue"]');
      $submit.attr('disabled', true);
      var authnetSettings = settings.commerceAuthorizeNet;
      if (authnetSettings.paymentMethodType === 'credit_card') {
        Drupal.commerceAuthorizeNetAcceptForm($form, authnetSettings);
      }
      // This is from 8.x branch, keep for #1154294, whenever done.
      // else if (authnetSettings.paymentMethodType === 'authnet_echeck') {
      //  Drupal.commerceAuthorizeNetEcheckForm($form, authnetSettings);
      // }

      // AcceptJS hijacks all submit buttons for this form. Simulate the back
      // button to make sure back submit still works.
      $('.checkout-cancel, .checkout-back').bind('mousedown, click', function(e) {
        e.preventDefault();
        window.history.back();
      });
    },
    detach: function (context, settings) {
      var $form = $('.acceptjs-form').closest('form');
      $form.removeOnce('acceptjs-processed');
      $form.unbind('submit.authnet');
    }
  };
  Drupal.commerceAuthorizeNet = {
    errorDisplay: function (code, error_message) {
      console.log(code + ': ' + error_message);
      var $form = $('.acceptjs-form').closest('form');
      // Display the message error in the payment form.
      var errors = $form.find('#payment-errors');
      errors.html(Drupal.theme('commerceAuthorizeNetError', error_message));
      $('html, body').animate({ scrollTop: errors.offset().top });

      // Allow the customer to re-submit the form.
      $form.find('button').prop('disabled', false);
    }
  };

  $.extend(Drupal.theme, /** @lends Drupal.theme */{
    commerceAuthorizeNetError: function (message) {
      return $('<div class="messages messages--error"></div>').html(message);
    }
  });
})(Drupal, jQuery);
