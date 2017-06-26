<?php
/**
 * Handler for shortcode spreaker
 *
 * @package    BardCanvas
 * @subpackage spreaker_shortcode
 * @author     https://www.spreaker.com - Ported by Alejandro Caballero - lava.caballero@gmail.com
 * 
 * Trailing globals:
 * @var module[] $modules
 * @var module   $current_module Topmost module
 * @var module   $this_module    Self
 * @var config   $config
 * @var settings $settings
 * @var account  $account
 *
 * Trailing vars:
 * @var string   $matched_str
 * @var string   $contents
 * @var array    $attributes
 *
 * Expected attributes:
 * • type
 * • resource
 * • theme
 * • autoplay
 * • playlist
 * • width
 * • height
 */

use hng2_base\account;
use hng2_base\config;
use hng2_base\module;
use hng2_base\settings;

include_once __DIR__ . "/functions.inc";

$res      = process_spreaker_shortcode($attributes);
$contents = str_replace($matched_str, $res, $contents);
return;
