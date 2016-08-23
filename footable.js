/**
 * @file
 * Javascript file for the FooTable module.
 */

(function ($) {
  'use strict';

  Drupal.behaviors.footable = {
    attach: function (context, settings) {
      if ($('.footable', context).footable) {
        $('.footable', context).footable();
      }
    }
  };
}(jQuery));
