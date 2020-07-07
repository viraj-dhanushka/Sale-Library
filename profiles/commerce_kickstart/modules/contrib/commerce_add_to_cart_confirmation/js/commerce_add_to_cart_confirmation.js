(function ($) {
  Drupal.behaviors.commerce_add_to_cart_confirmation_overlay = {
    attach:function (context, settings) {
      var overlayClass = 'commerce_add_to_cart_confirmation_overlay';
      var overlayParentSelector = 'body';

      // Determine the appropriate overlay class and parent selector based on the settings array.
      if (typeof settings.commerceAddToCartConfirmation != 'undefined') {
        if (typeof settings.commerceAddToCartConfirmation.overlayClass != 'undefined') {
          overlayClass = settings.commerceAddToCartConfirmation.overlayClass;
        }

        if (typeof settings.commerceAddToCartConfirmation.overlayParentSelector != 'undefined') {
          overlayParentSelector = settings.commerceAddToCartConfirmation.overlayParentSelector;
        }
      }

      if ($('.commerce-add-to-cart-confirmation').length > 0) {
        // Add the background overlay.
        $(overlayParentSelector).append('<div class="' + overlayClass + '"></div>');

        // Enable the close link.
        $('.commerce-add-to-cart-confirmation-close').bind('click touchend', function(e) {
          e.preventDefault();
          $('.commerce-add-to-cart-confirmation').remove();
          $('.' + overlayClass).remove();
        });
      }
    }
  }
})(jQuery);
