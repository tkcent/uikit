<?php

/**
 * @file
 * Modify structured content arrays.
 *
 * These hooks are called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * content structure has been built.
 *
 * If the theme wishes to act on the rendered HTML of the content rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_HOOK().
 *
 * @see backdrop_render()
 * @see theme()
 */

/**
 * Implements hook_css_alter().
 */
function uikit_css_alter(&$css) {
  $uikit_path = backdrop_get_path('theme', 'uikit');

  // Stop Backdrop core stylesheets from being loaded.

  unset($css['core/layouts/boxton/boxton.css']);
  unset($css['core/misc/normalize.css']);
  unset($css['core/misc/smartmenus/css/sm-core-css.css']);
  unset($css['core/modules/comment/css/comment.css']);
  unset($css['core/modules/date/css/date.css']);
  unset($css['core/modules/layout/css/grid-flexbox.css']);
  unset($css['core/modules/search/search.theme.css']);
  unset($css['core/modules/system/css/menu-dropdown.theme.css']);
  unset($css['core/modules/system/css/menu-toggle.theme.css']);
  unset($css['core/modules/system/css/messages.theme.css']);
  unset($css['core/modules/system/css/system.css']);
  unset($css['core/modules/system/css/system.messages.css']);
  unset($css['core/modules/system/css/system.theme.css']);
  unset($css['core/modules/user/css/user.css']);
  unset($css['core/modules/views/css/views.css']);

  // Unset other CSS files
  /*
  unset($css['core/modules/admin_bar/css/admin_bar-print.css']);
  unset($css['core/modules/admin_bar/css/admin_bar.css']);
  unset($css['core/modules/ckeditor5/css/ckeditor5-maximize.css']);
  unset($css['core/modules/ckeditor5/css/ckeditor5.css']);
  unset($css['core/modules/contextual/css/contextual.css']);
  unset($css['core/modules/filter/css/filter.css']);
  unset($css['core/modules/layout/css/grid-flex.css']);
  unset($css['core/modules/system/css/jquery.treetable.css']);
  unset($css['core/modules/system/css/jquery.treetable.theme.default.css']);
  unset($css['core/modules/system/css/menu-dropdown.theme.breakpoint-queries.css']);
  unset($css['core/modules/system/css/menu-dropdown.theme.breakpoint.css']);
  unset($css['core/modules/system/css/menu-dropdown.theme.css']);
  unset($css['core/modules/system/css/menu-toggle.theme.css']);
  unset($css['core/modules/system/css/system.admin.css']);
  unset($css['core/modules/system/css/token.css']);
  unset($css['modules/webform/css/webform-admin.css']);
  */


  global $theme_key;
  $theme_path = backdrop_get_path('theme', $theme_key);
  
  if (theme_get_setting('uikit_source') == 'local') {
    $uikit_css_path = $theme_path . '/src/' . theme_get_setting('uikit_local_version') . '/css/uikit.min.css';
    $css[$uikit_css_path] = [
      'data' => $uikit_css_path,
      'type' => 'file',
      'media' => 'all',
      'group' => CSS_DEFAULT,
      'preprocess' => TRUE,
      'weight' => -10,
      'every_page' => TRUE,
      'every_page_weight' => -10,
      'browsers' => [],
    ];
  }

  if (theme_get_setting('uikit_source') == 'online') {
    $uikit_online_version = theme_get_setting('uikit_online_version');
    $uikit_css_cdn = 'https://cdn.jsdelivr.net/npm/uikit@' . $uikit_online_version . '/dist/css/uikit.min.css';
    // Alterate URL for Cloudflare
    // $uikit_css_cdn = '//cdnjs.cloudflare.com/ajax/libs/uikit/' . $uikit_online_version . '/css/uikit.min.css';
    $css[$uikit_css_cdn] = [
      'data' => $uikit_css_cdn,
      'type' => 'external',
      'media' => 'all',
      'group' => CSS_DEFAULT,
      'preprocess' => TRUE,
      'weight' => -10,
      'every_page' => 'TRUE',
      'every_page_weight' => -10,
      'browsers' => [],
    ];
  }
  // Replace the book module's book.css with a custom version.
  $book_css = backdrop_get_path('module', 'book') . '/book.css';
  if (isset($css[$book_css])) {
    $css[$book_css]['data'] = $uikit_path . '/css/book.css';
  }

  // Replace the field module's field.css with a custom version.
  $field_css = backdrop_get_path('module', 'field') . '/css/field.css';
  if (isset($css[$field_css])) {
    $css[$field_css]['data'] = $uikit_path . '/css/field.css';
  }

  // Attach custom css file if it exists.
  if (theme_get_setting('custom_css_checkbox')) {
    $custom_css_file = config_get('system.core', 'file_public_path') . '/' . $theme_key . '_custom.css';
    if (file_exists($custom_css_file)) {
      global $base_url, $base_path;
      $custom_css_path = $base_url . $base_path . $custom_css_file;
      $css[$custom_css_path] = [
        'data' => $custom_css_path,
        'type' => 'file',
        'media' => 'all',
        'group' => CSS_THEME,
        'preprocess' => TRUE,
        'weight' => 10,
        'every_page' => TRUE,
        'every_page_weight' => 10,
        'browsers' => [],
      ];
    }
  }
}

/**
 * Implements hook_js_alter().
 */
function uikit_js_alter(&$javascript) {
//  unset($javascript['core/misc/jquery.js']);
//  unset($javascript['core/misc/jquery.once.js']);
//  unset($javascript['core/misc/backdrop.js']);
  unset($javascript['core/modules/layout/js/grid-fallback.js']);
  unset($javascript['core/misc/smartmenus/jquery.smartmenus.js']);
  unset($javascript['core/misc/smartmenus/addons/keyboard/jquery.smartmenus.keyboard.js']);
  unset($javascript['core/modules/system/js/menus.js']);

  global $theme_key;
  $theme_path = backdrop_get_path('theme', $theme_key);

  if (theme_get_setting('uikit_source') == 'local') {
    $uikit_js_path = $theme_path . '/src/' . theme_get_setting('uikit_local_version') . '/js/uikit.min.js';
    $javascript[$uikit_js_path] = [
      'data' => $uikit_js_path,
      'type' => 'file',
      'every_page' => TRUE,
      'weight' => -5,
      'preprocess' => 'FALSE',
    ] + backdrop_js_defaults();

    if (theme_get_setting('uikit_icons') == 1) {
      $uikit_js_icons_path = $theme_path . '/src/' . theme_get_setting('uikit_local_version') . '/js/uikit-icons.min.js';
      $javascript[$uikit_js_icons_path] = [
        'data' => $uikit_js_icons_path,
        'type' => 'file',
        'every_page' => TRUE,
        'weight' => -4,
        'preprocess' => 'FALSE',
      ] + backdrop_js_defaults();
    }
  }

  if (theme_get_setting('uikit_source') == 'online') {
    $uikit_online_version = theme_get_setting('uikit_online_version');
    $uikit_js_cdn = 'https://cdn.jsdelivr.net/npm/uikit@' . $uikit_online_version . '/dist/js/uikit.min.js';
    // Alterate URL for Cloudflare
    // $uikit_js_cdn = '//cdnjs.cloudflare.com/ajax/libs/uikit/' . $uikit_online_version . '/js/uikit.min.js';

    $javascript[$uikit_js_cdn] = [
      'data' => $uikit_js_cdn,
      'type' => 'external',
      'every_page' => 'TRUE',
      'weight' => -5,
      'preprocess' => FALSE,
    ] + backdrop_js_defaults();

    if (theme_get_setting('uikit_icons') == 1) {
      $uikit_js_icons_cdn = 'https://cdn.jsdelivr.net/npm/uikit@' . $uikit_online_version . '/dist/js/uikit-icons.min.js';
      $javascript[$uikit_js_icons_cdn] = [
        'data' => $uikit_js_icons_cdn,
        'type' => 'external',
        'every_page' => 'TRUE',
        'weight' => -4,
        'preprocess' => FALSE,
      ] + backdrop_js_defaults();
    }
  }

  // Unset unneeded javascript
  /*
  unset($javascript['core/misc/smartmenus/jquery.smartmenus.combined.js']);
  unset($javascript['core/misc/ajax.js']);
  unset($javascript['core/misc/autocomplete.js']);
  unset($javascript['core/misc/collapse.js']);
  unset($javascript['core/misc/form.js']);
  unset($javascript['core/misc/jquery.form.js']);
  unset($javascript['core/misc/progress.js']);
  unset($javascript['core/misc/states.js']);
  unset($javascript['core/misc/tableselect.js']);
  unset($javascript['core/misc/vertical-tabs.js']);
  unset($javascript['core/modules/ckeditor5/js/ckeditor5.formatter.js']);
  unset($javascript['core/modules/ckeditor5/js/ckeditor5.js']);
  unset($javascript['core/modules/ckeditor5/js/plugins/backdrop-basic-styles/backdrop-basic-styles.js']);
  unset($javascript['core/modules/ckeditor5/js/plugins/backdrop-html-engine/backdrop-html-engine.js']);
  unset($javascript['core/modules/ckeditor5/js/plugins/backdrop-image/backdrop-image.js']);
  unset($javascript['core/modules/ckeditor5/js/plugins/backdrop-link/backdrop-link.js']);
  unset($javascript['core/modules/ckeditor5/js/plugins/backdrop-maximize/backdrop-maximize.js']);
  unset($javascript['core/modules/ckeditor5/lib/ckeditor5/build/ckeditor5-dll.js']);
  unset($javascript['core/modules/comment/js/comment.admin.js']);
  unset($javascript['core/modules/field/modules/text/js/text.js']);
  unset($javascript['core/modules/file/js/file.js']);
  unset($javascript['core/modules/filter/js/filter.js']);
  unset($javascript['core/modules/node/js/node.js']);
  unset($javascript['core/modules/path/js/path.js']);
  unset($javascript['core/modules/redirect/js/redirect.js']);
  unset($javascript['core/modules/system/js/jquery.treetable.js']);
  unset($javascript['core/modules/system/js/token.js']);
  unset($javascript['misc/tableheader.js']);
  unset($javascript['modules/webform/js/webform-admin.js']);
  */

  // Admin JS
  /*
  unset($javascript['core/misc/jquery.js']);
  unset($javascript['core/misc/jquery.once.js']);
  unset($javascript['core/misc/backdrop.js']);
  unset($javascript['core/misc/ui/jquery.ui.data.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.disable-selection.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.form.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.labels.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.scroll-parent.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.tabbable.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.unique-id.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.version.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.jquery-patch.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.focusable.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.form-reset-mixin.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.ie.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.keycode.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.plugin.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.safe-active-element.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.safe-blur.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.widget.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.mouse.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.touch-punch.js']);
  unset($javascript['core/misc/ui/jquery.ui.draggable.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.droppable.min.js']);
  unset($javascript['core/misc/ui/jquery.ui.sortable.min.js']);
  unset($javascript['core/misc/dropbutton.js']);
  unset($javascript['core/modules/contextual/js/contextual.js']);
  unset($javascript['core/modules/admin_bar/js/admin_bar.js']);
  unset($javascript['core/modules/layout/js/layout.admin.js']);
  unset($javascript['core/misc/announce.js']);
  */
}

/**
 * Implements hook_html_head_alter().
 */
function uikit_html_head_alter(&$head_elements) {
  // Add a charset meta tag to the head elements.
  if (isset($head_elements['system_meta_content_type'])) {
    $head_elements['system_meta_content_type']['#attributes'] = [
      'charset' => theme_get_setting('meta_charset'),
    ];
    $head_elements['system_meta_content_type']['#weight'] = -9999;
  }

  // Some modules, such as the Adminimal Admin menu, add a viewport meta tag.
  // Remove this so the theme can define the tag.
  if (isset($head_elements['viewport'])) {
    unset($head_elements['viewport']);
  }
}

/**
 * Implements hook_menu_local_tasks_alter().
 */
function uikit_menu_local_tasks_alter(&$data, $router_item, $root_path) {
  // Define various icons.
  $plus = '<i data-uk-icon="icon:plus"></i> ';

  foreach ($data['actions']['output'] as $key => $action) {
    // Add icon based on link path.
    if (isset($action['#link']['path'])) {
      switch ($action['#link']['path']) {
        case 'node/add':
        case 'admin/structure/block/add':
        case 'admin/structure/types/add':
        case 'admin/structure/menu/add':
        case 'admin/structure/taxonomy/add':
        case 'admin/structure/taxonomy/%/add':
        case 'admin/appearance/install':
        case 'admin/people/create':
        case 'admin/modules/install':
        case 'admin/config/content/formats/add':
        case 'admin/config/media/image-styles/add':
        case 'admin/config/search/path/add':
        case 'admin/config/regional/date-time/types/add':
        case 'admin/config/user-interface/shortcut/add-set':
          $title = $plus . $data['actions']['output'][$key]['#link']['title'];
          $data['actions']['output'][$key]['#link']['title'] = $title;
          $data['actions']['output'][$key]['#link']['localized_options']['html'] = TRUE;
          break;
      }
    }
    // Some actions use the href key instead of the path key.
    elseif (isset($action['#link']['href'])) {
      switch ($action['#link']['href']) {
        case 'node/add/blog':
          $title = $plus . $data['actions']['output'][$key]['#link']['title'];
          $data['actions']['output'][$key]['#link']['title'] = $title;
          $data['actions']['output'][$key]['#link']['localized_options']['html'] = TRUE;
          break;
      }
    }
  }
}

function uikit_form_alter(&$form, &$form_state, $form_id) {
  foreach ($form['actions'] as $action => $info) {
    if (isset($info['#type']) && $info['#type'] == 'link') {
      $form['actions'][$action]['#options']['attributes']['class'][] = 'uk-button';
    }
  }
}
