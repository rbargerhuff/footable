<?php

/**
 * @file
 * FooTable administration forms.
 */

/**
 * FooTable settings configuration form.
 */
function footable_form_admin_settings($form, &$form_state) {
  $config = config('footable.settings');
  $form['#config'] = 'footable.settings';
  $form['config'] = [
    '#type' => 'vertical_tabs',
  ];

  $form['config']['plugin'] = [
    '#type' => 'fieldset',
    '#title' => t('Plugin'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ];

  $form['config']['plugin']['footable_plugin_type'] = [
    '#type' => 'radios',
    '#title' => t('Type'),
    '#options' => [
      'standalone' => t('Standalone'),
      'bootstrap' => t('Bootstrap'),
    ],
    '#default_value' => $config->get('footable_plugin_type'),
  ];

  $form['config']['plugin']['footable_plugin_compression'] = [
    '#type' => 'radios',
    '#title' => t('Compression level'),
    '#options' => [
      'minified' => t('Production (minified)'),
      'source' => t('Development (uncompressed)'),
    ],
    '#default_value' => $config->get('footable_plugin_compression'),
  ];

  $form['config']['advanced'] = [
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  ];

  $form['config']['advanced']['footable_breakpoint_load_default'] = [
    '#type' => 'checkbox',
    '#title' => t("Load the module's default breakpoints"),
    '#description' => t("Control whether the module's default breakpoints are used. This will not affect breakpoints exported via Features."),
    '#default_value' => $config->get('footable_breakpoint_load_default'),
  ];

  return system_settings_form($form);
}
