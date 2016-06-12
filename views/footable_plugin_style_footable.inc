<?php

/**
 * @file
 * Contains the FooTable style plugin.
 */

/**
 * Style plugin to render a FooTable.
 *
 * @ingroup views_style_plugins
 */
class footable_plugin_style_footable extends views_plugin_style_table {

  /**
   * @inheritdoc
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['footable']['expand_first'] = array('default' => FALSE);
    $options['footable']['show_header'] = array('default' => TRUE);
    $options['footable']['toggle_column'] = array('default' => 'first');
    $options['footable']['bootstrap']['striped'] = array('default' => FALSE);
    $options['footable']['bootstrap']['bordered'] = array('default' => FALSE);
    $options['footable']['bootstrap']['hover'] = array('default' => FALSE);
    $options['footable']['bootstrap']['condensed'] = array('default' => FALSE);
    return $options;
  }

  /**
   * @inheritdoc
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Check if fields have been added.  Table style plugin will set
    // error_markup if fields are not added.
    // @see views_plugin_style_table::options_form()
    if (isset($form['error_markup'])) {
      return;
    }

    $form['footable'] = array(
      '#type' => 'fieldset',
      '#title' => t('FooTable settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['footable']['expand_first'] = array(
      '#type' => 'select',
      '#title' => t('Expand first row'),
      '#description' => t('Whether or not to expand the first rows details.'),
      '#options' => array(
        FALSE => t('Disabled'),
        TRUE => t('Enabled'),
      ),
      '#default_value' => $this->options['footable']['expand_first'],
    );

    $form['footable']['show_header'] = array(
      '#type' => 'select',
      '#title' => t('Show header'),
      '#description' => t('Whether or not to display a header row in the table.'),
      '#options' => array(
        TRUE => t('Yes'),
        FALSE => t('No'),
      ),
      '#default_value' => $this->options['footable']['show_header'],
    );

    $form['footable']['toggle_column'] = array(
      '#type' => 'select',
      '#title' => t('Expandable column'),
      '#description' => t('Specify which column the toggle is appended to in a row.'),
      '#options' => array(
        'first' => t('First'),
        'last' => t('Last'),
      ),
      '#default_value' => $this->options['footable']['toggle_column'],
    );

    // Bootstrap style configuration.
    if (variable_get('footable_plugin_type', 'standalone') == 'bootstrap') {
      $form['footable']['bootstrap'] = array(
        '#type' => 'fieldset',
        '#title' => t('Bootstrap'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['footable']['bootstrap']['striped'] = array(
        '#type' => 'checkbox',
        '#title' => t('Striped'),
        '#default_value' => $this->options['footable']['bootstrap']['striped'],
      );

      $form['footable']['bootstrap']['bordered'] = array(
        '#type' => 'checkbox',
        '#title' => t('Bordered'),
        '#default_value' => $this->options['footable']['bootstrap']['bordered'],
      );

      $form['footable']['bootstrap']['hover'] = array(
        '#type' => 'checkbox',
        '#title' => t('Hover'),
        '#default_value' => $this->options['footable']['bootstrap']['hover'],
      );

      $form['footable']['bootstrap']['condensed'] = array(
        '#type' => 'checkbox',
        '#title' => t('Condensed'),
        '#default_value' => $this->options['footable']['bootstrap']['condensed'],
      );
    }

    // Breakpoint configuration.
    $form['footable']['breakpoint'] = array(
      '#type' => 'fieldset',
      '#title' => t('Collapsed columns'),
      '#description' => t('Select the "breakpoints" where a particular column should be hidden.'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $breakpoints = array();
    foreach (footable_breakpoint_load_all(TRUE) as $breakpoint) {
      $breakpoints[$breakpoint->machine_name] = check_plain($breakpoint->name);
    }

    foreach ($this->display->handler->get_field_labels() as $name => $label) {
      $form['footable']['breakpoint'][$name] = array(
        '#title' => check_plain($label),
        '#type' => 'checkboxes',
        '#options' => $breakpoints,
        '#default_value' => isset($this->options['footable']['breakpoint'][$name]) ? $this->options['footable']['breakpoint'][$name] : array(),
        '#multiple' => TRUE,
      );
    }
  }

  /**
   * @inheritdoc
   */
  function options_validate(&$form, &$form_state) {
    $breakpoints = FALSE;
    if (!empty($form_state['values']['style_options']['footable']['breakpoint'])) {
      foreach ($form_state['values']['style_options']['footable']['breakpoint'] as $breakpoint) {
        if (!empty(array_filter($breakpoint))) {
          $breakpoints = TRUE;
          break;
        }
      }
    }

    if (!$breakpoints) {
      form_error($form['footable']['breakpoint'], t('You need to have atleast one column that has a breakpoint.'));
    }
  }
}
