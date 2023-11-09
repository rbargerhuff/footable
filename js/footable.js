/**
 * @file
 * Javascript file for the FooTable module.
 */

(function ($) {
  'use strict';

  Backdrop.behaviors.footable = {
    attach: function (context, settings) {
      if ($('.footable', context).footable) {
        $('.footable', context).once('footable', function () {
          var id = $(this).attr('id');
          $(this).on({
            'expand.ft.row': function (e, ft, row) {
              row.$el.addClass('expanded');
            },
            'collapse.ft.row': function (e, ft, row) {
              row.$el.removeClass('expanded');
            }
          }).footable(settings.footable[id]).on();
        });
      }
    }
  };
}(jQuery));
