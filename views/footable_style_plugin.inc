<?php
/**
 * @file
 * Contains the footable style plugin.
 */

/**
 * Style plugin to render each item as a row in a responsive table.
 *
 * @ingroup views_style_plugins
 */
class footable_style_plugin extends views_plugin_style_table {
  function option_definition() {
    $options = parent::option_definition();

    unset($options['sticky']);
    unset($options['override']);

    $options['elements'] = array(
      'default' => array(
        'search_box' => TRUE,
        'table_info' => TRUE,
        'save_state' => FALSE,
    ));
    $options['layout'] = array(
      'default' => array(
        'autowidth' => FALSE,
        'themeroller' => FALSE,
        'sdom' => '',
    ));
    $options['pages'] = array(
      'default' => array(
        'pagination_style' => 0,
        'length_change' => 0,
        'display_length' => 10,
    ));
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Check if fields have been added.  Table style plugin will set
    // error_markup if fields are not added.
    // @see views_plugin_style_table::options_form()
    if (isset($form['error_markup'])) {
      return;
    }

    $columns = $this->display->handler->get_field_labels();
    $form['footable'] = array(
      '#type' => 'fieldset',
      '#title' => t('FooTable Settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $field_names = $this->display->handler->get_field_labels();

    $form['footable']['expand'] = array(
      '#title' => t('Expandable Column'),
      '#type' => 'select',
      '#options' => $field_names,
      '#default_value' => $this->options['footable']['expand'],
      '#description' => t('Select the column where a plus/minus icon should appear.'),
    );

    $form['footable']['hide'] = array(
      '#type' => 'fieldset',
      '#title' => t('Collapsed/Hidden Columns'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('Select the "breakpoints" where a particular column should be hidden.'),
    );

    foreach ($columns as $column_name => $column_label) {
      $safe = str_replace(array('][', '_', ' '), '-', $column_name);
      // the $id of the column for dependency checking.

      $form['footable']['hide'][$column_name] = array(
        '#type' => 'checkboxes',
        '#multiple' => TRUE,
        '#title' => check_plain($column_label),
        '#default_value' => isset($this->options['footable']['hide'][$column_name]) ? $this->options['footable']['hide'][$column_name] : 0,
        '#options' => array('phone' => 'Phone', 'tablet' => 'Tablet'),
        
      );
    }
  }
}
