(function ($) {
  Drupal.behaviors.adSenseUnblock = {
    attach: function () {
      setTimeout(function() {
        if ($('.adsense ins').contents().length == 0) {
          var $adsense = $('.adsense');
          $adsense.html(Drupal.t("Please, enable ads on this site. By using ad-blocking software, you're depriving this site of revenue that is needed to keep it free and current. Thank you."));
          $adsense.css({'overflow': 'hidden', 'font-size': 'smaller'});
        }
        // Wait 3 seconds for adsense async to execute.
      }, 3000);
    }
  };

})(jQuery);
