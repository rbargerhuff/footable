<?php

/**
 * @file
 * API documentation for FooTable module.
 */

/**
 * Implements hook_footable_breakpoint_load_all_alter().
 *
 * Add, remove or edit FooTable breakpoints.
 *
 * @array $breakpoints
 *   A keyed array of breakpoints. Each key represents the name of a
 *   breakpoint, used the the column headers. The values are integers
 *   containing the breakpoint in pixels.
 */
function hook_footable_breakpoint_load_all_alter(&$breakpoints) {
}

/**
 * Implements hook_footable_breakpoint_load_alter().
 *
 * Add, remove or edit FooTable breakpoints.
 *
 * @array $breakpoints
 *   A keyed array of breakpoints. Each key represents the name of a
 *   breakpoint, used the the column headers. The values are integers
 *   containing the breakpoint in pixels.
 */
function hook_footable_breakpoint_load_alter(&$breakpoints) {
}

/**
 * Provides a default configuration for FooTable breakpoints.
 *
 * This hook allows modules to provide their own FooTable breakpoints.
 *
 * This hook should be placed in MODULENAME.footable_breakpoint.inc and it will
 * be auto-loaded. MODULENAME.footable_breakpoint.inc *must* be in the same
 * directory as the .module file which *must* also contain an implementation of
 * hook_ctools_plugin_api, preferably with the same code as found in
 * footable_ctools_plugin_api().
 *
 * The $config->status boolean attribute indicates whether the FooTable
 * instance should be enabled (TRUE) or disabled (FALSE) by default.
 *
 * @return array
 *   An associative array containing the structures of FooTable breakpoints, as
 *   generated from the Export tab, keyed by the FooTable breakpoint machine
 *   name.
 *
 * @see footable_default_footable_breakpoint()
 * @see footable_ctools_plugin_api()
 */
function hook_default_footable_breakpoint() {
  $breakpoints = array();

  $breakpoint = new stdClass;
  $breakpoint->api_version = 1;
  $breakpoint->name = 'Example breakpoint';
  $breakpoint->machine_name = 'breakpoint1';
  $breakpoint->breakpoint = 1000;
  $breakpoint->status = TRUE;
  $breakpoint->weight = 0;
  $breakpoints[$breakpoint->machine_name] = $breakpoint;

  return $breakpoints;
}