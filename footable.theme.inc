<?php

/**
 * Returns HTML for a table.
 *
 * @param array $variables
 *   An associative array containing:
 *   - header: An array containing the table headers. Each element of the array
 *     can be either a localized string or an associative array with the
 *     following keys:
 *     - "data": The localized title of the table column.
 *     - "field": The database field represented in the table column (required
 *       if user is to be able to sort on this column).
 *     - "sort": A default sort order for this column ("asc" or "desc"). Only
 *       one column should be given a default sort order because table sorting
 *       only applies to one column at a time.
 *     - "breakpoints": An array containing FooTable breakpoint machine names
 *       at which point this column should be hidden.
 *     - Any HTML attributes, such as "colspan", to apply to the column header
 *       cell.
 *   - rows: An array of table rows. Every row is an array of cells, or an
 *     associative array with the following keys:
 *     - "data": an array of cells
 *     - Any HTML attributes, such as "class", to apply to the table row.
 *     - "no_striping": a boolean indicating that the row should receive no
 *       'even / odd' styling. Defaults to FALSE.
 *     Each cell can be either a string or an associative array with the
 *     following keys:
 *     - "data": The string to display in the table cell.
 *     - "header": Indicates this cell is a header.
 *     - Any HTML attributes, such as "colspan", to apply to the table cell.
 *     Here's an example for $rows:
 *
 * @code
 *     $rows = array(
 *       // Simple row
 *       array(
 *         'Cell 1', 'Cell 2', 'Cell 3'
 *       ),
 *       // Row with attributes on the row and some of its cells.
 *       array(
 *         'data' => array('Cell 1', array('data' => 'Cell 2', 'colspan' =>
 *   2)), 'class' => array('funky')
 *       )
 *     );
 * @endcode
 *   - attributes: An array of HTML attributes to apply to the table tag.
 *   - caption: A localized string to use for the <caption> tag.
 *   - colgroups: An array of column groups. Each element of the array can be
 *     either:
 *     - An array of columns, each of which is an associative array of HTML
 *       attributes applied to the COL element.
 *     - An array of attributes applied to the COLGROUP element, which must
 *       include a "data" attribute. To add attributes to COL elements, set the
 *       "data" attribute with an array of columns, each of which is an
 *       associative array of HTML attributes.
 *     Here's an example for $colgroup:
 * @code
 *     $colgroup = array(
 *       // COLGROUP with one COL element.
 *       array(
 *         array(
 *           'class' => array('funky'), // Attribute for the COL element.
 *         ),
 *       ),
 *       // Colgroup with attributes and inner COL elements.
 *       array(
 *         'data' => array(
 *           array(
 *             'class' => array('funky'), // Attribute for the COL element.
 *           ),
 *         ),
 *         'class' => array('jazzy'), // Attribute for the COLGROUP element.
 *       ),
 *     );
 * @endcode
 *     These optional tags are used to group and set properties on columns
 *     within a table. For example, one may easily group three columns and
 *     apply same background style to all.
 *   - sticky: Use a "sticky" table header.
 *   - empty: The message to display in an extra row if table does not have any
 *     rows.
 *   - breakpoints: A keyed array of breakpoints. Each key represents the name
 *     of a breakpoint, used the the column headers. The values are integers
 *     containing the breakpoint in pixels. If this value is left empty, all
 *     enabled FooTable breakpoints will be used.
 *   - expand_first: Whether or not to expand the first rows details.
 *   - show_header: Whether or not to display a header row in the table.
 *   - toggle_column: Specify which column the toggle is appended to in a row
 *     ("first" or "last").
 *
 * @return string
 *   An HTML string representing the FooTable table.
 *
 * @see theme_table()
 */
function theme_footable($variables) {
  // Add FooTable class.
  if (empty($variables['attributes']['class']) || !in_array('footable', $variables['attributes']['class'])) {
    $variables['attributes']['class'][] = 'footable';
  }

  if (empty($variables['attributes']['id'])) {
    $variables['attributes']['id'] = backdrop_html_id('footable');
  }

  $footable = [];

  // Breakpoints.
  if (empty($variables['breakpoints'])) {
    $variables['breakpoints'] = [];
    foreach (footable_breakpoint_load_multiple() as $breakpoint) {
      $variables['breakpoints'][$breakpoint->machine_name] = (int) $breakpoint->breakpoint;
    }
  }
  $footable['breakpoints'] = $variables['breakpoints'];

  // Expand first row.
  $footable['expandFirst'] = !empty($variables['expand_first']);

  // Show header.
  $footable['showHeader'] = !empty($variables['show_header']);

  // Expandable column.
  $footable['toggleColumn'] = $variables['toggle_column'];

  foreach ($variables['header'] as &$cell) {
    if (!is_array($cell)) {
      $cell = ['data' => $cell];
    }

    // Add datatype.
    if (empty($cell['data-type'])) {
      $cell['data-type'] = 'html';
    }

    // Add breakpoints.
    if (!empty($cell['breakpoints']) && empty($cell['data-breakpoints'])) {
      $cell['data-breakpoints'] = implode(' ', $cell['breakpoints']);
    }
  }

  footable_libraries_load();
  backdrop_add_js(['footable' => [$variables['attributes']['id'] => $footable]], ['type' => 'setting']);
  backdrop_add_js(backdrop_get_path('module', 'footable') . '/footable.js');
  return theme('table', $variables);
}

/**
 * Theme the form for the FooTable style plugin.
 *
 * @see theme_views_ui_style_plugin_table()
 */
function theme_footable_style_plugin_table($variables) {
  $form = $variables['form'];

  $output = backdrop_render($form['description_markup']);

  $header = [
    t('Field'),
    t('Column'),
    t('Type'),
    t('Align'),
    t('Separator'),
    [
      'data' => t('Sortable'),
      'align' => 'center',
    ],
    [
      'data' => t('Default order'),
      'align' => 'center',
    ],
    [
      'data' => t('Default sort'),
      'align' => 'center',
    ],
    [
      'data' => t('Hide empty column'),
      'align' => 'center',
    ],
  ];
  $rows = [];
  foreach (element_children($form['columns']) as $id) {
    $row = [];
    $row[] = check_plain(backdrop_render($form['info'][$id]['name']));
    $row[] = backdrop_render($form['columns'][$id]);
    $row[] = backdrop_render($form['info'][$id]['type']);
    $row[] = backdrop_render($form['info'][$id]['align']);
    $row[] = backdrop_render($form['info'][$id]['separator']);
    if (!empty($form['info'][$id]['sortable'])) {
      $row[] = [
        'data' => backdrop_render($form['info'][$id]['sortable']),
        'align' => 'center',
      ];
      $row[] = [
        'data' => backdrop_render($form['info'][$id]['default_sort_order']),
        'align' => 'center',
      ];
      $row[] = [
        'data' => backdrop_render($form['default'][$id]),
        'align' => 'center',
      ];
    }
    else {
      $row[] = '';
      $row[] = '';
      $row[] = '';
    }
    $row[] = [
      'data' => backdrop_render($form['info'][$id]['empty_column']),
      'align' => 'center',
    ];
    $rows[] = $row;
  }

  // Add the special 'None' row.
  $rows[] = [
    t('None'),
    '',
    '',
    '',
    '',
    '',
    [
      'align' => 'center',
      'data' => backdrop_render($form['default'][-1]),
    ],
    '',
  ];

  $output .= theme('table', ['header' => $header, 'rows' => $rows]);
  $output .= backdrop_render_children($form);
  return $output;
}
