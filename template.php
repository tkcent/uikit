<?php

/**
 * @file
 * Conditional logic and data processing for the UIkit theme.
 */

/**
 * Load UIkit's include files for theme processing.
 */
require_once 'includes/preprocess.inc';
require_once 'includes/theme.inc';
require_once 'includes/alter.inc';

/**
 * Retrieves UIkit, jQuery, jQuery Migrate and Font Awesome CDN assets.
 */
function uikit_get_cdn_assets() {
  if (theme_get_setting('uikit_source') == 'local') {
    $uikit_local_version = theme_get_setting('uikit_local_version');
    $theme_path = backdrop_get_path('theme', 'uikit');
    backdrop_add_css($theme_path . '/src/' . $uikit_local_version . '/css/uikit.min.css', array('type' => 'file', 'weight' => -100));
    backdrop_add_js($theme_path . '/src/' . $uikit_local_version . '/js/uikit.min.js');
    if (theme_get_setting('uikit_icons', 'uikit')) {
      backdrop_add_js($theme_path . '/src/' . $uikit_local_version . '/js/uikit-icons.min.js');
    }
  } else {
    $uikit_online_version = theme_get_setting('uikit_online_version');
    /**
     * Alterate URL for Cloudflare
     */
    $uikit_css_cdn = '//cdnjs.cloudflare.com/ajax/libs/uikit/' . $uikit_online_version . '/css/uikit.min.css';
    $uikit_css_cdn = 'https://cdn.jsdelivr.net/npm/uikit@' . $uikit_online_version . '/dist/css/uikit.min.css';
    backdrop_add_css($uikit_css_cdn, array(
      'type' => 'external',
      'group' => CSS_THEME,
      'every_page' => TRUE,
      'weight' => 0,
      'version' => $uikit_online_version,
    ));
    $uikit_js_cdn = 'https://cdn.jsdelivr.net/npm/uikit@' . $uikit_online_version . '/dist/js/uikit.min.js';
    backdrop_add_js($uikit_js_cdn, array(
      'type' => 'external',
      'group' => JS_THEME,
      'every_page' => TRUE,
      'weight' => -20,
      'version' => $uikit_online_version,
    ));

    if (theme_get_setting('uikit_icons', 'uikit')) {
      // Add the UIkit icons script.
      $uikit_icons_cdn = 'https://cdn.jsdelivr.net/npm/uikit@' . $uikit_online_version . '/dist/js/uikit-icons.min.js';
      backdrop_add_js($uikit_icons_cdn, array(
        'type' => 'external',
        'group' => JS_THEME,
        'every_page' => TRUE,
        'weight' => -20,
        'version' => $uikit_online_version,
      ));
    }
  }
}
