<?php

/**
 * @file
 * Contains the FooTable style plugin.
 */

/**
 * Style plugin to render each item as a row in a responsive table.
 *
 * @ingroup views_style_plugins
 */
class footable_plugin_style_footable extends views_plugin_style_table {
  /**
   * Provide default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['footable'] = array('default' => array());

    return $options;
  }

  /**
   * Adds FooTable configuration form options.
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
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $form['footable']['expand_first'] = array(
      '#title' => t('Expand first row'),
      '#type' => 'select',
      '#options' => array(
        FALSE => t('Disabled'),
        TRUE => t('Enabled'),
      ),
      '#default_value' => isset($this->options['footable']['expand_first']) ? $this->options['footable']['expand_first'] : FALSE,
      '#description' => t('Whether or not to expand the first rows details..'),
    );

    $form['footable']['show_header'] = array(
      '#title' => t('Show header'),
      '#type' => 'select',
      '#options' => array(
        TRUE => t('Yes'),
        FALSE => t('No'),
      ),
      '#default_value' => isset($this->options['footable']['show_header']) ? $this->options['footable']['show_header'] : TRUE,
      '#description' => t('Whether or not to display a header row in the table.'),
    );

    $form['footable']['toggle_column'] = array(
      '#title' => t('Expandable column'),
      '#type' => 'select',
      '#options' => array(
        'first' => t('First'),
        'last' => t('Last'),
      ),
      '#default_value' => isset($this->options['footable']['toggle_column']) ? $this->options['footable']['toggle_column'] : 'first',
      '#description' => t('Specify which column the toggle is appended to in a row.'),
    );

    // Bootstrap style configuration
    $form['footable']['bootstrap'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bootstrap'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#access' => variable_get('footable_plugin_type', 'standalone') == 'bootstrap',
    );

    $form['footable']['bootstrap']['striped'] = array(
      '#title' => t('Striped'),
      '#type' => 'checkbox',
      '#default_value' => isset($this->options['footable']['bootstrap']['striped']) ? $this->options['footable']['bootstrap']['striped'] : FALSE,
    );

    $form['footable']['bootstrap']['bordered'] = array(
      '#title' => t('Bordered'),
      '#type' => 'checkbox',
      '#default_value' => isset($this->options['footable']['bootstrap']['bordered']) ? $this->options['footable']['bootstrap']['bordered'] : FALSE,
    );

    $form['footable']['bootstrap']['hover'] = array(
      '#title' => t('Hover'),
      '#type' => 'checkbox',
      '#default_value' => isset($this->options['footable']['bootstrap']['hover']) ? $this->options['footable']['bootstrap']['hover'] : FALSE,
    );

    $form['footable']['bootstrap']['condensed'] = array(
      '#title' => t('Condensed'),
      '#type' => 'checkbox',
      '#default_value' => isset($this->options['footable']['bootstrap']['condensed']) ? $this->options['footable']['bootstrap']['condensed'] : FALSE,
    );

    // Breakpoint configuration
    $form['footable']['breakpoint'] = array(
      '#type' => 'fieldset',
      '#title' => t('Collapsed columns'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('Select the "breakpoints" where a particular column should be hidden.'),
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
        '#default_value' => isset($this->options['footable']['breakpoint'][$name]) ? $this->options['footable']['breakpoint'][$name] : NULL,
        '#multiple' => TRUE,
      );
    }
  }

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
