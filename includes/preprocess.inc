<?php

/**
 * @file
 * Set up variables to be placed within the template (.tpl.php) files.
 *
 * The variables set up here apply to both templates (.tpl.php) files and
 * functions (theme_HOOK). These are also used for providing
 * @link https://www.drupal.org/node/223440 template suggestions @endlink.
 *
 * @see process.inc
 */


/**
 * Prepares variables for header templates.
 *
 * @see header.tpl.php
 */
function uikit_preprocess_header(&$variables) {
  $logo = $variables['logo'];
  $logo_attributes = $variables['logo_attributes'];

  // Add classes and height/width to logo.
  if ($logo) {
    $logo_wrapper_classes = array();
    $logo_wrapper_classes[] = 'header-logo-wrapper';
    if ($logo_attributes['width'] <= $logo_attributes['height']) {
      $logo_wrapper_classes[] = 'header-logo-tall';
    }

    $variables['logo_wrapper_classes'] = $logo_wrapper_classes;
  }
  
  $menu_primary = 'main-menu';
  $menu_secondary = 'user-menu';

  $variables['menu_primary'] = FALSE;
  $variables['menu_secondary'] = FALSE;
  
  $variables['menu_header_transparent'] = theme_get_setting('menu_header_transparent') ? 'uk-navbar-transparent' : '';
  $variables['menu_header_position'] = theme_get_setting('menu_header_position');
  $variables['menu_header_toggle'] = theme_get_setting('menu_header_toggle');

  // Set defaults for menu configuration
  $config = [
    'style' => 'dropdown',
    'level' => 1,
//    'depth' => 2,
    'toggle' => FALSE,
  ];

//  module_load_include('inc','system','system.menu');

  if ($menu_primary) {
    $main_menu = menu_tree('main-menu');
    $main_menu_rendered = render($main_menu);
    
    $variables['menu_primary'] = menu_tree($menu_primary);
    $config['menu_name'] = $menu_primary;
//    $menu_primary_tree = system_menu_block_build($config);
//    $variables['menu_primary'] = $menu_primary_tree['content'];
//    $variables['menu_primary']['#theme_wrappers'] = ['menu_tree__navbar'];
    
  }

  if ($menu_secondary) {
    $variables['menu_secondary'] = menu_tree($menu_secondary);
    $config['menu_name'] = $menu_secondary;
//    $menu_secondary_tree = system_menu_block_build($config);
//    $variables['menu_secondary'] = $menu_secondary_tree['content'];
//    $variables['menu_secondary']['#theme_wrappers'] = ['menu_tree__navbar'];
  }
}


/**
 * Implements hook_preprocess_page().
 */
function uikit_preprocess_page(&$variables) {
  $variables['html_attributes']['class'] = 'uk-height-1-1';
  
  if ($variables['is_front']) {
    $variables['classes'][] = 'layout-front';
  }
  $node = menu_get_object();
  if (!empty($node)) {
    $variables['classes'][] = 'page-node-' . $node->nid;
  }
  // Assign content attributes.
  $variables['content_attributes'] = [
    'id' => 'page-content',
  ];

  // Define header attributes.
  $variables['header_attributes'] = [
    'id' => 'page-header',
  ];

  // Define navbar attributes.
  $variables['navbar_attributes'] = [
    'id' => 'page-navbar',
    'class' => array('uk-navbar-container', 'uk-navbar-transparent'),
    'data-uk-navbar' => '',
  ];

  // Build the data attributes for the off-canvas menu
  $offcanvas_data_array = [];
  $offcanvas_data_array[] = "mode: " . theme_get_setting('offcanvas_mode');
  $offcanvas_data_array[] = "flip: " . (theme_get_setting('offcanvas_flip') ? 'true' : 'false');
  $offcanvas_data_array[] = "overlay: " . (theme_get_setting('offcanvas_overlay') ? 'true' : 'false');
  $variables['offcanvas_attributes'] = [
    'id' => 'offcanvas',
    'data-uk-offcanvas' => implode("; ", $offcanvas_data_array),
  ];

  $variables['offcanvas_primary'] = FALSE;
  $variables['offcanvas_secondary'] = FALSE;
  
  if (theme_get_setting('offcanvas_primary') != 'none') {
    $variables['offcanvas_primary'] = menu_tree(theme_get_setting('offcanvas_primary'));
    $variables['offcanvas_primary']['#theme_wrappers'] = array('menu_tree__offcanvas');
    foreach ($variables['offcanvas_primary'] as $key => $value) {
      if (is_array($value) && isset($value['#below']) && !empty($value['#below'])) {
        $variables['offcanvas_primary'][$key]['#below']['#theme_wrappers'] = array('menu_tree__offcanvas__below');
      }
    }
  }
  if (theme_get_setting('offcanvas_secondary') != 'none') {
    $variables['offcanvas_secondary'] = menu_tree(theme_get_setting('offcanvas_secondary'));
    $variables['offcanvas_secondary']['#theme_wrappers'] = array('menu_tree__offcanvas');
    foreach ($variables['offcanvas_secondary'] as $key => $value) {
      if (is_array($value) && isset($value['#below']) && !empty($value['#below'])) {
        $variables['offcanvas_secondary'][$key]['#below']['#theme_wrappers'] = array('menu_tree__offcanvas__below');
      }
    }
  }

  $variables['display_breadcrumb'] = theme_get_setting('display_breadcrumbs');
}

/**
 * Implements hook_preprocess_layout().
 */
function uikit_preprocess_layout(&$variables) {
  $layout = $variables['layout'];
  // Add the main content wrapper to the beginning of the list of positions
  $positions = array_merge(['main-content-wrapper' => ['main-content-wrapper']], $layout->positions);
  foreach ($positions as $region => $region_info) {
    $variables['uikit'][$region]['class'] = [];
    $region_classes = theme_get_setting(implode('_', [$layout->name, $region, 'classes']));
    if (!empty($region_classes)) {
      $variables['uikit'][$region]['class'] += explode(' ', $region_classes);
    }
    $background = theme_get_setting(implode('_', array($layout->name, $region, 'background')));
    if (!empty($background)) {
      $background_file = file_load($background);
      $variables['uikit'][$region]['class'][] = 'uk-background-cover';
      $variables['uikit'][$region]['style'] = 'background-image: url(' . file_create_url($background_file->uri) . ');';
      $parallax = theme_get_setting(implode('_', array($layout->name, $region, 'parallax')));
      if (!empty($parallax)) {
        $variables['uikit'][$region]['data-uk-parallax'] = $parallax;
      }
    }
  }

  $variables['attributes']['class'][] = $variables['layout_info']['template'];
  $variables['attributes']['class'][] = 'layout';

  if (isset($variables['layout_info']['flexible'])) {
    // Add css class to layout.
//    $variables['classes'][] = 'layout-' . backdrop_clean_css_identifier($variables['layout_info']['name']);
  }
  $node = menu_get_object();
  if (!empty($node)) {
    $variables['classes'][] = 'layout-node-' . $node->nid;
    foreach ($layout->content as $key => $block) {
      if ($block->module == 'system' && $block->delta == 'header') {
      }
    }
  }
}


/**
 * Implements hook_preprocess_node().
 */
function uikit_preprocess_node(&$variables) {
  $node = $variables['node'];

  // Add the uk-text-lead class to all node titles.
  $variables['title_attributes_array']['class'][] = 'uk-text-lead';

  // Theme the submitted meta data.
  $datetime = date('F j, Y', $node->created);
  if (variable_get('node_submitted_' . $node->type, TRUE)) {
    $variables['submitted'] = t('Written by !username on !datetime', array('!username' => $variables['name'], '!datetime' => $datetime));
  }
}

/**
 * Implements template_preprocess_region().
 */
function uikit_preprocess_region(&$variables) {
  global $theme_key;
  $region = $variables['region'];
  $wrapper_id = str_replace('_', '-', $region);

  // Add a wrapper id and class to help themers.
  $variables['wrapper_attributes_array'] = array(
    'id' => 'region-' . $wrapper_id . '-wrapper',
    'class' => array('region-wrapper'),
  );

  // Add an attributes id.
  $variables['attributes_array'] = array(
    'id' => 'region-' . $wrapper_id,
  );

  // Get all regions for the theme.
  $regions = system_region_list($theme_key);

  foreach ($regions as $key => $value) {
    // Get the settings for each region being used.
    if ($region == $key) {
      $style_setting = theme_get_setting($key . '_style');
      $card_style = theme_get_setting($key . '_card_style');
      $region_style = $style_setting ? $style_setting : 0;
      $region_card_style = $card_style ? $card_style : 0;

      if ($region_style) {
        switch ($region_style) {
          case 'card':
            $variables['attributes_array']['class'][] = 'uk-card';
            $variables['attributes_array']['class'][] = 'uk-card-body';
            break;
        }
      }

      if ($region_card_style) {
        switch ($region_card_style) {
          case 'default':
            $variables['attributes_array']['class'][] = 'uk-card-default';
            break;

          case 'primary':
            $variables['attributes_array']['class'][] = 'uk-card-primary';
            break;

          case 'secondary':
            $variables['attributes_array']['class'][] = 'uk-card-secondary';
            break;
        }
      }
    }
  }
}

/**
 * Implements template_preprocess_block().
 */
function uikit_preprocess_block(&$variables) {
  $uuid = $variables['block']->uuid;
  $variables['region'] = $variables['layout']->getBlockPosition($uuid);
}

/**
 * Implements template_preprocess_HOOK().
 */
function uikit_preprocess_breadcrumb(&$variables) {
  // Remove home link from breadcrumb if disabled in theme settings.
  if (!theme_get_setting('breadcrumbs_home_link')) {
    array_shift($variables['breadcrumb']);
  }

  if (theme_get_setting('breadcrumbs_current_page') && !backdrop_is_front_page()) {
    // Display the current page title if enabled in the theme settings and set
    // it as being active.
    $variables['breadcrumb'][] = '<span class="uk-active">' . backdrop_get_title() . '</span>';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_button().
 */
function uikit_preprocess_button(&$variables) {
  // Add the uk-button-default class to all buttons.
  $variables['element']['#attributes']['class'][] = 'uk-button';
  $variables['element']['#attributes']['class'][] = 'uk-button-default';
}

/**
 * Implements hook_preprocess_HOOK() for theme_textfield().
 */
function uikit_preprocess_textfield(&$variables) {
  $variables['element']['#attributes']['class'][] = 'uk-input';
//  $variables['element']['#attributes']['class'][] = 'uk-form-width-large';

  // Check for errors and set correct error class.
  if (form_get_error($variables['element'])) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_textarea().
 */
function uikit_preprocess_textarea(&$variables) {
  $variables['element']['#attributes']['class'][] = 'uk-textarea';
//  $variables['element']['#attributes']['class'][] = 'uk-form-width-large';

  // Check for errors and set correct error class.
  if (form_get_error($variables['element'])) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_select().
 */
function uikit_preprocess_select(&$variables) {
  $variables['element']['#attributes']['class'][] = 'uk-select';
  if ($variables['element']['#title'] == 'Editor') {
    $variables['element']['#attributes']['class'][] = 'uk-form-width-large';
  }

  // Check for errors and set correct error class.
  if (form_get_error($variables['element'])) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
}

/**
 * Implements template_preprocess_comment().
 */
function uikit_preprocess_comment(&$variables) {
  $comment = $variables['elements']['#comment'];
  $node = $variables['elements']['#node'];

  // Check if user picture in comments is enabled.
  $comment_picture = theme_get_setting('toggle_comment_user_picture') ? 1 : 0;

  // Check if user pictures are enabled.
  $user_pictures = variable_get('user_pictures') ? 1 : 0;

  // Chech if a default picture has been set.
  $user_picture_default = !empty(variable_get('user_picture_default')) ? 1 : 0;

  // Add comment classes.
  $variables['classes_array'][] = 'uk-comment';
  if ($variables['elements']['#comment']->divs > 0) {
    $variables['classes_array'][] = 'uk-comment-primary';
  }
  $variables['title_attributes_array']['class'][] = 'uk-comment-title';
  $variables['title_attributes_array']['class'][] = 'uk-margin-remove';
  $variables['content_attributes_array']['class'][] = 'uk-comment-body';

  // Use the comment id as the permalink text in the comment title.
  $id = $variables['id'];
  $cid = $comment->cid;
  $uri = entity_uri('node', $node);
  $uri['options'] += array(
    'attributes' => array(
      'class' => array(
        'permalink',
        'uk-link-reset',
      ),
      'rel' => 'bookmark',
    ),
    'fragment' => "comment-$cid",
  );
  $variables['permalink'] = l(t('#@id', array('@id' => $id)), $uri['path'], $uri['options']);

  // Use the same uri for the title permalink.
  $variables['title'] = l(t('@subject', array('@subject' => $comment->subject)), $uri['path'], $uri['options']);

  // Use separate submitted by and date variables.
  $variables['submitted_user'] = t('!username', array('!username' => $variables['author']));
  $variables['submitted_date'] = t('!datetime', array('!datetime' => $variables['created']));

  if ($comment_picture && $user_pictures && $user_picture_default && empty($variables['picture'])) {
    // Provide a default image when the user does not have a picture uploaded.
    if (empty($variables['picture'])) {
      $default = ' data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz4NCjwhLS0gR2VuZXJhdG9yOiBBZG9iZSBJbGx1c3RyYXRvciAxNi4wLjQsIFNWRyBFeHBvcnQgUGx1Zy1JbiAuIFNWRyBWZXJzaW9uOiA2LjAwIEJ1aWxkIDApICAtLT4NCjwhRE9DVFlQRSBzdmcgUFVCTElDICItLy9XM0MvL0RURCBTVkcgMS4xLy9FTiIgImh0dHA6Ly93d3cudzMub3JnL0dyYXBoaWNzL1NWRy8xLjEvRFREL3N2ZzExLmR0ZCI+DQo8c3ZnIHZlcnNpb249IjEuMSIgaWQ9IkViZW5lXzEiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIgeG1sbnM6eGxpbms9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkveGxpbmsiIHg9IjBweCIgeT0iMHB4Ig0KCSB3aWR0aD0iNTBweCIgaGVpZ2h0PSI1MHB4IiB2aWV3Qm94PSIwIDAgNTAgNTAiIGVuYWJsZS1iYWNrZ3JvdW5kPSJuZXcgMCAwIDUwIDUwIiB4bWw6c3BhY2U9InByZXNlcnZlIj4NCjxyZWN0IGZpbGw9IiNGRkZGRkYiIHdpZHRoPSI1MCIgaGVpZ2h0PSI1MCIvPg0KPGc+DQoJPHBhdGggZmlsbD0iI0UwRTBFMCIgZD0iTTQ1LjQ1LDQxLjM0NWMtMC4yMDktMS4xNjYtMC40NzMtMi4yNDYtMC43OTEtMy4yNDJjLTAuMzE5LTAuOTk2LTAuNzQ3LTEuOTY3LTEuMjg2LTIuOTE0DQoJCWMtMC41MzgtMC45NDYtMS4xNTUtMS43NTMtMS44NTItMi40MmMtMC42OTktMC42NjctMS41NS0xLjItMi41NTYtMS41OThzLTIuMTE3LTAuNTk4LTMuMzMyLTAuNTk4DQoJCWMtMC4xNzksMC0wLjU5NywwLjIxNC0xLjI1NSwwLjY0MmMtMC42NTcsMC40MjktMS4zOTksMC45MDctMi4yMjYsMS40MzRjLTAuODI3LDAuNTI4LTEuOTAzLDEuMDA2LTMuMjI3LDEuNDM0DQoJCWMtMS4zMjUsMC40MjktMi42NTUsMC42NDMtMy45ODksMC42NDNjLTEuMzM0LDAtMi42NjQtMC4yMTQtMy45ODktMC42NDNjLTEuMzI1LTAuNDI4LTIuNDAxLTAuOTA2LTMuMjI3LTEuNDM0DQoJCWMtMC44MjgtMC41MjctMS41NjktMS4wMDUtMi4yMjYtMS40MzRjLTAuNjU4LTAuNDI4LTEuMDc2LTAuNjQyLTEuMjU1LTAuNjQyYy0xLjIxNiwwLTIuMzI2LDAuMTk5LTMuMzMyLDAuNTk4DQoJCWMtMS4wMDYsMC4zOTgtMS44NTgsMC45MzEtMi41NTQsMS41OThjLTAuNjk5LDAuNjY3LTEuMzE1LDEuNDc0LTEuODUzLDIuNDJjLTAuNTM4LDAuOTQ3LTAuOTY3LDEuOTE3LTEuMjg1LDIuOTE0DQoJCXMtMC41ODMsMi4wNzYtMC43OTIsMy4yNDJjLTAuMjA5LDEuMTY1LTAuMzQ5LDIuMjUxLTAuNDE4LDMuMjU2Yy0wLjA3LDEuMDA2LTAuMTA0LDIuMS0wLjEwNCwzLjE1NUMzLjkwMSw0OC41NCwzLjk4Nyw0OSw0LjE0Myw1MA0KCQloNDEuNTg5YzAuMTU2LTEsMC4yNDItMS40NiwwLjI0Mi0yLjI0M2MwLTEuMDU1LTAuMDM1LTIuMTE4LTAuMTA1LTMuMTI0QzQ1Ljc5OSw0My42MjcsNDUuNjYsNDIuNTEsNDUuNDUsNDEuMzQ1eiIvPg0KCTxwYXRoIGZpbGw9IiNFMEUwRTAiIGQ9Ik0yNC45MzgsMzIuNDg1YzMuMTY3LDAsNS44NzEtMS4xMjEsOC4xMTMtMy4zNjFjMi4yNDEtMi4yNDIsMy4zNjEtNC45NDUsMy4zNjEtOC4xMTMNCgkJcy0xLjEyMS01Ljg3Mi0zLjM2MS04LjExMmMtMi4yNDItMi4yNDEtNC45NDYtMy4zNjItOC4xMTMtMy4zNjJzLTUuODcyLDEuMTIxLTguMTEyLDMuMzYyYy0yLjI0MiwyLjI0MS0zLjM2Miw0Ljk0NS0zLjM2Miw4LjExMg0KCQlzMS4xMiw1Ljg3MSwzLjM2Miw4LjExM0MxOS4wNjUsMzEuMzY1LDIxLjc3MSwzMi40ODUsMjQuOTM4LDMyLjQ4NXoiLz4NCjwvZz4NCjwvc3ZnPg0K';
      $variables['picture'] = '<img class="uk-comment-avatar" width="50" height="50" src="' . $default . '">';
    }
  }
  $variables['content']['links']['#attributes']['class'][] = 'uk-float-right';
}

/**
 * Implements template_preprocess_comment_wrapper().
 */
function uikit_preprocess_comment_wrapper(&$variables) {
  // Remove margin from the comment wrapper element.
  $variables['classes_array'][] = 'uk-margin-top-remove';
}

/**
 * Implements hook_preprocess_HOOK() for theme_confirm_form().
 */
function uikit_preprocess_confirm_form(&$variables) {
  foreach ($variables['form']['actions'] as $key => $action) {
    $type = isset($action['#type']) ? $action['#type'] : 0;

    if ($type && $type == 'link') {
      // Add the uk-button-default class to form action links for consistent
      // styling with form action buttons.
      $variables['form']['actions'][$key]['#attributes']['class'][] = 'uk-button-default';
    }
  }
}

/**
 * Implements template_preprocess_field().
 */
function uikit_preprocess_field(&$variables) {
  // Add utility classes based on field type.
  switch ($variables['element']['#field_type']) {
    case 'image':
      $variables['classes_array'][] = 'uk-display-inline-block';
      $variables['classes_array'][] = 'uk-margin';
      break;
  }

  if ($variables['element']['#field_name'] == 'field_tags') {
    foreach ($variables['items'] as $delta => $item) {
      $variables['item_attributes_array'][$delta]['class'][] = 'field-item';
      $variables['item_attributes_array'][$delta]['class'][] = $delta % 2 ? 'odd' : 'even';
    }
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_fieldset().
 */
function uikit_preprocess_fieldset(&$variables) {
  $collapsible = isset($variables['element']['#collapsible']) && $variables['element']['#collapsible'];
  $group_fieldset = isset($variables['element']['#group_fieldset']) && $variables['element']['#group_fieldset'];
  $format_fieldset = isset($variables['element']['format']);

  // Collapsible, non-grouped fieldsets will use UIkit's accordion components.
  if ($group_fieldset) {
    $variables['theme_hook_suggestions'][] = 'fieldset__grouped';
  }
  elseif ($collapsible) {
    $variables['theme_hook_suggestions'][] = 'fieldset__collapsible';
    $variables['element']['#attributes']['class'][] = 'uk-margin';
    $variables['element']['#attributes']['class'][] = 'uk-accordion';
    $variables['element']['#attributes']['data-uk-accordion'] = '';

    foreach ($variables['element']['#attributes']['class'] as $key => $class) {
      if ($class == 'collapsible' || $class == 'collapsed') {
        unset($variables['element']['#attributes']['class'][$key]);
        array_values($variables['element']['#attributes']['class']);
      }
      if ($class == 'collapsed') {
        $variables['element']['#attributes']['data-uk-accordion'] .= '{showfirst: false}';
      }
    }
  }
  elseif ($format_fieldset) {
    $variables['theme_hook_suggestions'][] = 'fieldset__format';
  }
  else {
    $variables['theme_hook_suggestions'][] = 'fieldset';
    $variables['element']['#attributes']['class'][] = 'uk-fieldset';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_form().
 */
function uikit_preprocess_form(&$variables) {
  $attributes = $variables['element']['#attributes'];

  // Make sure the class attribute is an array. If not we take the class set
  // and add it to an array. This is a special case issue caused by the
  // api_search module, and should not be a common problem.
  if (isset($attributes['class']) && !is_array($attributes['class'])) {
    $class = $attributes['class'];
    $variables['element']['#attributes']['class'] = array($class);
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_item_list().
 */
function uikit_preprocess_item_list(&$variables) {
  $pagination = isset($variables['pagination']) && $variables['pagination'];

  // Add the uk-list class to all but pagination item lists.
  if (!$pagination) {
    $variables['attributes']['class'][] = 'uk-list';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_links().
 */
function uikit_preprocess_links(&$variables) {
  $classes = isset($variables['attributes']['class']) ? $variables['attributes']['class'] : array();

  // Add uk-subnav class to inline links.
  if (in_array('inline', $classes)) {
    $variables['attributes']['class'][] = 'uk-subnav';
  }
}


/**
 * Implements hook_preprocess_HOOK() for theme_links__header_menu().
 */
function uikit_preprocess_links__header_menu(&$variables) {
/*
  $classes = isset($variables['attributes']['class']) ? $variables['attributes']['class'] : array();

  // Add uk-subnav class to inline links.
  if (in_array('inline', $classes)) {
    $variables['attributes']['class'][] = 'uk-subnav';
  }
*/
  $variables['attributes']['class'][] = 'uk-navbar-nav';
  $variables['attributes']['class'][] = 'uk-visible@m';
}


/**
 * Implements hook_preprocess_HOOK() for theme_menu_local_tasks().
 */
function uikit_preprocess_menu_local_tasks(&$variables) {

  // Get the local task styles.
  $primary_style = theme_get_setting('primary_tasks_style');
  $secondary_style = theme_get_setting('secondary_tasks_style');

  $default_class = ['uk-subnav'];

  // Set the default attributes.
  $variables['primary_attributes'] = [
    'id' => 'primary-local-tasks',
    'class' => ($primary_style ? array_merge($default_class, [$primary_style]) : $default_class),
  ];
  $variables['secondary_attributes'] = [
    'id' => 'secondary-local-tasks',
    'class' => ($secondary_style ? array_merge($default_class, [$secondary_style]) : $default_class),
  ];
}

/**
 * Implements hook_preprocess_HOOK() for theme_menu_tree().
 */
function uikit_preprocess_menu_tree(&$variables) {
  $shortcut_set = preg_grep("/^(menu_tree__shortcut_set)/", $variables['#tree']['#theme_wrappers']);

  if ($shortcut_set) {
    // Provide hook suggestion for shortcut set menu trees.
    $variables['theme_hook_suggestions'][] = 'menu_tree__shortcut_set';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_checkbox().
 */
/*function uikit_preprocess_checkbox(&$variables) {

  $variables['element']['#attributes']['class'][] = 'uk-checkbox';

  // Check for errors and set correct error class.
  if (form_get_error($variables['element'])) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
  
}
*/
/**
 * Implements hook_preprocess_HOOK() for theme_password().
 */
function uikit_preprocess_password(&$variables) {
/*
  // Define size attribute for password fields for consistent styling.
  $variables['element']['#attributes']['type'] = 'password';
  element_set_attributes($variables['element'], ['id', 'name', 'size', 'maxlength']);
//  $variables['element']['#attributes']['size'] = 25;
  $variables['element']['#attributes']['class'][] = 'uk-input';
  $variables['element']['#attributes']['class'][] = 'uk-form-width-large';
  $variables['element']['#wrapper_attributes']['class'][] = 'uk-padding';
  // Check for errors and set correct error class.
  if (form_get_error($variables['element'])) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
*/
}

/**
 * Implements hook_preprocess_HOOK() for theme_table().
 */
function uikit_preprocess_table(&$variables) {
  $variables['attributes']['class'][] = 'uk-table';
  $variables['attributes']['class'][] = 'uk-table-small';

  // Add some additional classes to the table for text format filter tips.
  $filter_tips = current_path() === 'filter/tips';
  if ($filter_tips) {
    $variables['attributes']['class'][] = 'uk-table-striped';
    $variables['attributes']['class'][] = 'table-filter-tips';
  }
}

function uikit_preprocess_views_view_table(&$variables) {
  $variables['classes'][] = 'uk-table';
  $variables['classes'][] = 'uk-table-small';
  $variables['classes'][] = 'uk-table-striped';
}

/**
 * Implements template_preprocess_username().
 */
function uikit_preprocess_username(&$variables) {
  global $language;

  if (isset($variables['attributes_array']['xml:lang'])) {
    // Provide a proper lang attribute to username elements.
    $lang = $variables['attributes_array']['xml:lang'];
    $variables['attributes_array']['lang'] = !empty($lang) ? $lang : $language->langcode;
  }
}


/**
 * Implements template_preprocess_file().
 */

function uikit_preprocess_file(&$variables) {
  $variables['element']['#attributes']['class'][] = 'uk-input';
  $variables['element']['#attributes']['class'][] = 'uk-form-width-large';

  if (form_get_error($element)) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
}


/**
 * Implements template_preprocess_maintenance_page().
 */
function uikit_preprocess_maintenance_page(&$variables) {
  // Load UIkit assets, i.e. stylesheets and scripts.
//  uikit_get_cdn_assets();

  // Assign content attributes.
  $content_attributes = array(
    'id' => 'page-content',
    'class' => array('uk-width-1-1@s'),
  );

  // Add attributes.
  $variables['classes_array'][] = 'uk-height-1-1';
  $variables['content_attributes_array'] = $content_attributes;
}
