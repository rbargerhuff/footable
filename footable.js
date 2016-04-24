/**
 * @file
 * Javascript file for the FooTable module.
 */

(function ($) {
  'use strict';

  Drupal.behaviors.footable = {
    attach: function (context, settings) {
      $('.footable', context).footable();
    }
  };
}(jQuery));