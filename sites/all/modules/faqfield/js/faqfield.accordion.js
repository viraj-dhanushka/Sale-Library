/**
 * @file
 * Binds accordion to provided faq fields.
 */

(function ($) {
  /**
   * Add faqfield accordion behaviour.
   */
  Drupal.behaviors.faqfieldAccordion = {
    attach: function (context, settings) {
      if (settings.faqfield != undefined) {
        // Bind the accordion to any defined faqfield accordion formatter with
        // provided settings.
        for (var selector in settings.faqfield) {
          var specs = settings.faqfield[selector];
          if (window.location.hash) {
            var hash = window.location.hash.replace('#', '');
            if (hash in settings.faqfieldAnchors) {
              specs.active = settings.faqfieldAnchors[hash];
            }
          }
          $(selector, context).accordion(specs);
        }
      }
    }
  };
})(jQuery);

