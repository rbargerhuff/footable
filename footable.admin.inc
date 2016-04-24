<?php

/**
 * @file
 * FooTable administration forms.
 */

/**
 *  FooTable breakpoints configuration form.
 */
function footable_form_admin_breakpoint($form, &$form_state) {
  $breakpoints = footable_breakpoint_load_multiple();
  $form['#tree'] = TRUE;
  $form['#breakpoints'] = array_keys($breakpoints);

  $default = array_keys(module_invoke('footable', 'default_footable_breakpoint'));
  foreach ($breakpoints as $breakpoint) {
    $form['footable_breakpoint'][$breakpoint->machine_name] = array(
      'name' => array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#required' => TRUE,
        '#default_value' => check_plain($breakpoint->name),
        '#title_display' => 'invisible',
        '#size' => 25,
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
      ),
      'status' => array(
        '#markup' => empty($breakpoint->status) ? t('Disabled') : t('Enabled'),
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
        '#access' => !in_array($breakpoint->machine_name, $default),
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

    $rows[] = array(
      'data' => array(
        drupal_render($form['footable_breakpoint'][$key]['name']),
        drupal_render($form['footable_breakpoint'][$key]['machine_name']),
        drupal_render($form['footable_breakpoint'][$key]['breakpoint']),
        drupal_render($form['footable_breakpoint'][$key]['status']),
        drupal_render($form['footable_breakpoint'][$key]['statustoggle']),
        drupal_render($form['footable_breakpoint'][$key]['weight']),
        drupal_render($form['footable_breakpoint'][$key]['delete']),
        drupal_render($form['footable_breakpoint'][$key]['export']),
      ),
      'class' => array('draggable'),
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