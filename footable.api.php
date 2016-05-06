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