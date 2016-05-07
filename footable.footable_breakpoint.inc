<?php

/**
 * Implements hook_default_footable_breakpoint().
 */
function footable_default_footable_breakpoint() {
  $breakpoints = array();

  $breakpoint = new stdClass;
  $breakpoint->api_version = 1;
  $breakpoint->name = 'Extra small devices';
  $breakpoint->machine_name = 'xs';
  $breakpoint->breakpoint = 480;
  $breakpoint->status = TRUE;
  $breakpoint->weight = 0;
  $breakpoints[$breakpoint->machine_name] = $breakpoint;

  $breakpoint = new stdClass;
  $breakpoint->api_version = 1;
  $breakpoint->name = 'Small devices';
  $breakpoint->machine_name = 'sm';
  $breakpoint->breakpoint = 768;
  $breakpoint->status = TRUE;
  $breakpoint->weight = 1;
  $breakpoints[$breakpoint->machine_name] = $breakpoint;

  $breakpoint = new stdClass;
  $breakpoint->api_version = 1;
  $breakpoint->name = 'Medium devices';
  $breakpoint->machine_name = 'md';
  $breakpoint->breakpoint = 992;
  $breakpoint->status = TRUE;
  $breakpoint->weight = 2;
  $breakpoints[$breakpoint->machine_name] = $breakpoint;

  $breakpoint = new stdClass;
  $breakpoint->api_version = 1;
  $breakpoint->name = 'Large devices';
  $breakpoint->machine_name = 'lg';
  $breakpoint->breakpoint = 1200;
  $breakpoint->status = TRUE;
  $breakpoint->weight = 3;
  $breakpoints[$breakpoint->machine_name] = $breakpoint;

  return $breakpoints;
}