<?php

/**
 * @file
 * FooTable administration forms.
 */

/**
 * FooTable settings configuration form.
 */
function footable_form_admin_settings($form, &$form_state) {
  $form['config'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['config']['plugin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plugin'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['config']['plugin']['footable_plugin_type'] = array(
    '#type' => 'radios',
    '#title' => t('Type'),
    '#options' => array(
      'standalone' => t('Standalone'),
      'bootstrap' => t('Bootstrap'),
    ),
    '#default_value' => variable_get('footable_plugin_type', 'standalone'),
  );

  $form['config']['plugin']['footable_plugin_compression'] = array(
    '#type' => 'radios',
    '#title' => t('Compression level'),
    '#options' => array(
      'minified' => t('Production (minified)'),
      'source' => t('Development (uncompressed)'),
    ),
    '#default_value' => variable_get('footable_plugin_compression', 'minified'),
  );

  $form['config']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['config']['advanced']['footable_breakpoint_load_default'] = array(
    '#type' => 'checkbox',
    '#title' => t("Load the module's default breakpoints"),
    '#description' => t("Control whether the module's default breakpoints are used. This will not affect breakpoints exported via Features."),
    '#default_value' => variable_get('footable_breakpoint_load_default', TRUE),
  );

  return system_settings_form($form);
}

/**
 * FooTable breakpoints configuration form.
 *
 * TODO: Move this form, and all the sub forms to the Ctools export UI.
 */
function footable_form_admin_breakpoint($form, &$form_state) {
  $breakpoints = footable_breakpoint_load_multiple();
  $form['#tree'] = TRUE;
  $form['#breakpoints'] = array_keys($breakpoints);
  $form['#attached']['css'][] = drupal_get_path('module', 'ctools') . '/css/export-ui-list.css';

  foreach ($breakpoints as $breakpoint) {
    $status = footable_breakpoint_is_enabled($breakpoint);

    $form['footable_breakpoint'][$breakpoint->machine_name] = array(
      '#status' => $status,
      'name' => array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#required' => TRUE,
        '#default_value' => $breakpoint->name,
        '#title_display' => 'invisible',
        '#size' => 25,
        '#disabled' => !$status,
      ),
      'machine_name' => array(
        '#markup' => $breakpoint->machine_name,
      ),
      'breakpoint' => array(
        '#type' => 'textfield',
        '#title' => t('Breakpoint'),
        '#required' => TRUE,
        '#element_validate' => array('element_validate_integer_positive'),
        '#default_value' => $breakpoint->breakpoint,
        '#field_suffix' => 'px',
        '#title_display' => 'invisible',
        '#size' => 25,
        '#disabled' => !$status,
      ),
      'status' => array(
        '#markup' => !$status ? t('Disabled') : t('Enabled'),
      ),
      'statustoggle' => array(
        '#type' => 'link',
        '#title' => empty($breakpoint->status) ? t('Enable') : t('Disable'),
        '#href' => 'admin/config/user-interface/footable/breakpoint/' . $breakpoint->machine_name . '/' . (empty($breakpoint->status) ? 'enable' : 'disable'),
      ),
      'delete' => array(
        '#type' => 'link',
        '#title' => t('Delete'),
        '#href' => 'admin/config/user-interface/footable/breakpoint/' . $breakpoint->machine_name . '/delete',
        '#access' => $breakpoint->type == t('Normal'),
      ),
      'export' => array(
        '#type' => 'link',
        '#title' => t('Export'),
        '#href' => 'admin/config/user-interface/footable/breakpoint/' . $breakpoint->machine_name . '/export',
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#title' => t('Weight'),
        '#required' => TRUE,
        '#default_value' => $breakpoint->weight,
        '#title_display' => 'invisible',
        '#size' => 3,
      ),
    );
  }

  $form['footable_breakpoint']['new'] = array(
    'name' => array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#title_display' => 'invisible',
      '#size' => 25,
    ),
    'machine_name' => array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#required' => FALSE,
      '#machine_name' => array(
        'exists' => 'footable_breakpoint_load',
        'source' => array('footable_breakpoint', 'new', 'name'),
        'label' => t('Machine name'),
        'replace_pattern' => '[^a-z0-9-+]',
        'replace' => '-',
        'standalone' => TRUE,
      ),
      '#title_display' => 'invisible',
      '#size' => 25,
    ),
    'breakpoint' => array(
      '#type' => 'textfield',
      '#title' => t('Breakpoint'),
      '#element_validate' => array('element_validate_integer_positive'),
      '#field_suffix' => 'px',
      '#title_display' => 'invisible',
      '#size' => 25,
    ),
    'weight' => array(
      '#type' => 'textfield',
      '#title' => t('Weight'),
      '#default_value' => footable_breakpoint_max_weight() + 1,
      '#title_display' => 'invisible',
      '#size' => 3,
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation handler for footable_form_admin_breakpoint().
 *
 * @see footable_form_admin_breakpoint_submit()
 */
function footable_form_admin_breakpoint_validate($form, &$form_state) {
  $new = $form_state['values']['footable_breakpoint']['new'];

  // We can't allow the user to use the FooTable reserved machine names.
  $reserved = array('all', 'default');
  if (!empty($new['machine_name']) && in_array($new['machine_name'], $reserved)) {
    form_set_error('footable_breakpoint][new][machine_name', t('The chosen machine name is reserved by the FooTable plugin.'));
  }

  if (array_filter(array($new['name'], $new['machine_name'], $new['breakpoint']))) {
    if (!$new['name']) {
      form_set_error('footable_breakpoint][new][name', t('You need to provide a name.'));
    }
    if (!$new['machine_name']) {
      form_set_error('footable_breakpoint][new][machine_name', t('You need to provide a machine name.'));
    }
    if (!$new['breakpoint']) {
      form_set_error('footable_breakpoint][new][breakpoint', t('You need to provide a breakpoint.'));
    }
  }
}

/**
 * Submit handler for footable_form_admin_breakpoint().
 *
 * @see footable_form_admin_breakpoint_validate()
 */
function footable_form_admin_breakpoint_submit($form, &$form_state) {
  $values = $form_state['values']['footable_breakpoint'];

  // Update existing breakpoints.
  foreach ($values as $key => $data) {
    if (in_array($key, $form['#breakpoints'])) {
      $breakpoint = footable_breakpoint_load($key);

      if (!$breakpoint) {
        $breakpoint = (object) array(
          'is_new' => TRUE,
          'machine_name' => $data['machine_name'],
        );
      }

      $breakpoint->name = $data['name'];
      $breakpoint->breakpoint = $data['breakpoint'];
      $breakpoint->weight = $data['weight'];
      footable_breakpoint_save($breakpoint);
    }
  }

  // Add new breakpoint.
  if (!empty($values['new']['machine_name'])) {
    $breakpoint = (object) array(
      'is_new' => TRUE,
      'machine_name' => $values['new']['machine_name'],
      'name' => $values['new']['name'],
      'breakpoint' => $values['new']['breakpoint'],
      'weight' => $values['new']['weight'],
    );
    footable_breakpoint_save($breakpoint);
  }

  drupal_set_message(t('The breakpoints have been saved.'));
}

/**
 * Themable display of the FooTable breakpoint configuration table.
 */
function theme_footable_form_admin_breakpoint($variables) {
  $form = $variables['form'];

  $header = array(
    t('Name'),
    t('Machine name'),
    t('Breakpoint'),
    t('Status'),
    t('Weight'),
    array(
      'data' => t('Operations'),
      'colspan' => 3,
    ),
  );

  $rows = array();
  foreach (element_children($form['footable_breakpoint']) as $key) {
    $form['footable_breakpoint'][$key]['weight']['#attributes']['class'][] = 'footable-breakpoint-weight';
    $class = $key != 'new' && empty($form['footable_breakpoint'][$key]['#status']) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled';

    $rows[] = array(
      'data' => array(
        drupal_render($form['footable_breakpoint'][$key]['name']),
        drupal_render($form['footable_breakpoint'][$key]['machine_name']),
        drupal_render($form['footable_breakpoint'][$key]['breakpoint']),
        drupal_render($form['footable_breakpoint'][$key]['status']),
        drupal_render($form['footable_breakpoint'][$key]['weight']),
        drupal_render($form['footable_breakpoint'][$key]['statustoggle']),
        drupal_render($form['footable_breakpoint'][$key]['delete']),
        drupal_render($form['footable_breakpoint'][$key]['export']),
      ),
      'class' => array($class, 'draggable'),
    );
  }

  $table = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'footable-breakpoints-table'),
  );

  $output = theme('table', $table);
  $output .= drupal_render_children($form);
  drupal_add_tabledrag('footable-breakpoints-table', 'order', 'sibling', 'footable-breakpoint-weight');

  return $output;
}

/**
 * Deletes a FooTable breakpoint.
 *
 * @param object $breakpoint
 *   FooTable breakpoint loaded by footable_breakpoint_load().
 *
 * @see footable_form_admin_config_breakpoint_delete_submit()
 */
function footable_form_admin_breakpoint_delete($form, &$form_state, $breakpoint) {
  return confirm_form(
    array(
      'machine_name' => array(
        '#type' => 'value',
        '#value' => $breakpoint->machine_name,
      ),
      'name' => array(
        '#type' => 'value',
        '#value' => $breakpoint->name,
      ),
    ),
    t('Are you sure you want to remove the breakpoint %breakpoint?', array('%breakpoint' => $breakpoint->name)),
    'admin/config/user-interface/footable',
    t('This action cannot be undone.'),
    t('Delete')
  );
}

/**
 * Form submission handler for footable_form_admin_breakpoint_delete().
 */
function footable_form_admin_breakpoint_delete_submit($form, &$form_state) {
  footable_breakpoint_delete($form_state['values']['machine_name']);
  drupal_set_message(t('Successfully deleted FooTable breakpoint %breakpoint', array('%breakpoint' => $form_state['values']['name'])));
  $form_state['redirect'] = 'admin/config/user-interface/footable';
}

/**
 * Enables a FooTable breakpoint.
 *
 * @param object $breakpoint
 *   FooTable breakpoint loaded by footable_breakpoint_load().
 *
 * @see footable_form_admin_breakpoint_status_enable_submit()
 */
function footable_form_admin_breakpoint_status_enable($form, &$form_state, $breakpoint) {
  return confirm_form(
    array(
      'machine_name' => array(
        '#type' => 'value',
        '#value' => $breakpoint->machine_name,
      ),
      'name' => array(
        '#type' => 'value',
        '#value' => $breakpoint->name,
      ),
    ),
    t('Are you sure you want to enable the breakpoint %breakpoint?', array('%breakpoint' => $breakpoint->name)),
    'admin/config/user-interface/footable',
    '',
    t('Enable')
  );
}

/**
 * Form submission handler for footable_form_admin_breakpoint_status_enable().
 */
function footable_form_admin_breakpoint_status_enable_submit($form, &$form_state) {
  if ($breakpoint = footable_breakpoint_load($form_state['values']['machine_name'])) {
    $breakpoint->status = FOOTABLE_BREAKPOINT_ENABLED;
    footable_breakpoint_save($breakpoint);
  }
  $form_state['redirect'] = 'admin/config/user-interface/footable';
}

/**
 * Disables a FooTable breakpoint.
 *
 * @param object $breakpoint
 *   FooTable breakpoint loaded by footable_breakpoint_load().
 *
 * @see footable_form_admin_breakpoint_status_disable_submit()
 */
function footable_form_admin_breakpoint_status_disable($form, &$form_state, $breakpoint) {
  return confirm_form(
    array(
      'machine_name' => array(
        '#type' => 'value',
        '#value' => $breakpoint->machine_name,
      ),
      'name' => array(
        '#type' => 'value',
        '#value' => $breakpoint->name,
      ),
    ),
    t('Are you sure you want to disable the breakpoint %breakpoint?', array('%breakpoint' => $breakpoint->name)),
    'admin/config/user-interface/footable',
    '',
    t('Disable')
  );
}

/**
 * Form submission handler for footable_form_admin_breakpoint_status_disable().
 */
function footable_form_admin_breakpoint_status_disable_submit($form, &$form_state) {
  if ($breakpoint = footable_breakpoint_load($form_state['values']['machine_name'])) {
    $breakpoint->status = FOOTABLE_BREAKPOINT_DISABLED;
    footable_breakpoint_save($breakpoint);
  }
  $form_state['redirect'] = 'admin/config/user-interface/footable';
}

/**
 * Export a FooTable breakpoint.
 *
 * @param object $breakpoint
 *   Breakpoint loaded by footable_breakpoint_load().
 */
function footable_form_admin_breakpoint_export($form, &$form_state, $breakpoint) {
  ctools_include('export');
  $form['export'] = array(
    '#type' => 'textarea',
    '#title' => t('Export'),
    '#rows' => 20,
    '#default_value' => ctools_export_object('footable_breakpoint', $breakpoint),
  );
  return $form;
}

/**
 * Import a FooTable breakpoint.
 */
function footable_form_admin_breakpoint_import($form, &$form_state) {
  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('FooTable breakpoint'),
    '#rows' => 20,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import')
  );
  return $form;
}

/**
 * Form validation handler for footable_form_admin_breakpoint_import().
 */
function footable_form_admin_breakpoint_import_validate($form, &$form_state) {
  ctools_include('export');
  $breakpoint = ctools_export_crud_import('footable_breakpoint', $form_state['values']['import']);
  if (is_array($breakpoint) || is_object($breakpoint)) {
    $breakpoint = (object) $breakpoint;

    // We can't allow the user to use the FooTable reserved machine names.
    if (!empty($breakpoint->machine_name)) {
      $reserved = array('all', 'default');
      if (in_array($breakpoint->machine_name, $reserved)) {
        form_set_error('import', t('The chosen machine name is reserved by the FooTable plugin.'));
      }

      if (footable_breakpoint_load($breakpoint->machine_name)) {
        form_set_error('import', t('A FooTable breakpoint with machine name %name already exists', array('%name' => $breakpoint->machine_name)));
      }
    }

    if (empty($breakpoint->name)) {
      form_set_error('import', t('The FooTable breakpoint needs to contain a name.'));
    }
    if (empty(!$breakpoint->machine_name)) {
      form_set_error('import', t('The FooTable breakpoint needs to contain a machine name.'));
    }
    if (empty($breakpoint->breakpoint)) {
      form_set_error('import', t('The FooTable breakpoint needs to contain a breakpoint.'));
    }
  }
}

/**
 * Form submission handler for footable_form_admin_breakpoint_import().
 */
function footable_form_admin_breakpoint_import_submit($form, &$form_state) {
  ctools_include('export');
  $breakpoint = ctools_export_crud_import('footable_breakpoint', $form_state['values']['import']);
  if (is_array($breakpoint) || is_object($breakpoint)) {
    $breakpoint = (object) $breakpoint;
    $breakpoint->is_new = TRUE;

    if (!empty($breakpoint->disabled)) {
      $breakpoint->status = FALSE;
    }

    if (footable_breakpoint_save($breakpoint)) {
      drupal_set_message(t('Breakpoint %breakpoint imported.', array('%breakpoint' => $breakpoint->name)));
      $form_state['redirect'] = 'admin/config/user-interface/footable';
    }
  }
}