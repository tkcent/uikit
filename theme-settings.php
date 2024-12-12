<?php

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function uikit_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL) {
  // General "alters" use a form id. Settings should not be set here. The only
  // thing useful about this is if you need to alter the form for the running
  // theme and *not* the theme setting.
  // @see http://drupal.org/node/943212
  if (isset($form_id)) {
    return;
  }

  $theme_name = $form['theme']['#value'];

  // Create vertical tabs for all UIkit related settings.
  $form['uikit'] = [
    '#type' => 'vertical_tabs',
    '#weight' => -10,
  ];

  /*
  **  UIkit Libraries
  */
  $form['uikit_libraries'] = [
    '#type' => 'fieldset',
    '#title' => t('UIkit Libraries'),
    '#group' => 'uikit',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  ];

  $uikit_source = theme_get_setting('uikit_source', $theme_name);
  $showlinks = theme_get_setting('uikit_doc_links', $theme_name);

  $uikit_source_options = [
    'online' => 'Online CDN',
    'local' => 'Local Source',
  ];

  // Local versions in the current theme folder
  $active_theme_source_path = backdrop_get_path('theme', $theme_name) . '/src';
  $active_theme_source_directories = scandir($active_theme_source_path);
  $available_local_versions = [];
  foreach ($active_theme_source_directories as $directory) {
    if (is_dir($active_theme_source_path . '/' . $directory) && strpos($directory, '.') !== 0) {
      $available_local_versions[$directory] = $directory;
    }
  }

  if (empty($available_local_versions)) {
    unset($uikit_source_options['local']);
    $form['uikit_libraries']['missing_local'] = [
      '#markup' => 'No local versions available',
    ];    
  }

  // Online versions
  $jsdelivr_uikit_url = "https://data.jsdelivr.com/v1/packages/gh/uikit/uikit";
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_URL, $jsdelivr_uikit_url);
  $json_result = curl_exec($ch);
  curl_close($ch);
  $uikit_cdn_info = json_decode($json_result, TRUE);
  $uikit_online_versions = [];
  foreach ($uikit_cdn_info['versions'] as $cdn_record) {
    $uikit_online_versions[$cdn_record['version']] = $cdn_record['version'];
  }
  // Latest version is the first one in the list
  $uikit_latest_version = key($uikit_online_versions);
 
  // Mark the latest version
  $uikit_online_versions[$uikit_latest_version] .= ' (' . t('Latest') . ')';

  $selected_local_version = theme_get_setting('uikit_local_version', $theme_name);
  $selected_online_version = theme_get_setting('uikit_online_version', $theme_name);

  if (($uikit_source == 'local' && $selected_local_version != $uikit_latest_version) || ($uikit_source == 'online' && $selected_online_version != $uikit_latest_version)) {
    $selected = explode('.', ($uikit_source == 'local' ? $selected_local_version : $selected_online_version));
    $latest = explode('.', $uikit_latest_version);
    // Check the major, minor and patch numbers.
    $uikit_version_alert = theme_get_setting('uikit_version_alert', $theme_name);
    if ($uikit_version_alert && ($selected[0] < $latest[0] || ($selected[0] == $latest[0] && $selected[1] < $latest[1]) || 
          ($selected[0] == $latest[0] && $selected[1] == $latest[1] && $selected[2] < $latest[2]))) {
      backdrop_set_message('A new version of the UIkit library (v' . $uikit_latest_version . ') is available. See what\'s changed: <a href="https://github.com/uikit/uikit/releases" target="_blank" rel="noopener">https://github.com/uikit/uikit/releases</a>', 'warning');
    }
  }

  $form['uikit_libraries']['uikit_source'] = [
    '#type' => 'radios',
    '#title' => t('UIkit library location'),
    '#options' => $uikit_source_options,
    '#default_value' => theme_get_setting('uikit_source', $theme_name),
  ];
  $form['uikit_libraries']['uikit_online_version'] = [
    '#type' => 'select',
    '#title' => t('Choose online version'),
    '#description' => t('<p>Using jsDelivr CDN - <a href="https://www.jsdelivr.com/package/npm/uikit" target="_blank" rel="noopener">https://www.jsdelivr.com/package/npm/uikit</a></p>'),
    '#options' => $uikit_online_versions,
    '#default_value' => $selected_online_version,
    '#states' => [
      'visible' => [
        ':input[name="uikit_source"]' => ['value' => 'online'],
      ],
    ],
  ];
  $form['uikit_libraries']['uikit_local_version'] = [
    '#type' => 'select',
    '#title' => t('Choose locally installed version'),
    '#description' => t('<p>Minimized CSS and JS files must be located in the /src subdirectory of this theme.  The latest version can be downloaded at <a href="https://getuikit.com/download" target="_blank" rel="noopener">https://getuikit.com/download</a></p>'),
    '#options' => $available_local_versions,
    '#default_value' => $selected_local_version,
    '#states' => [
      'visible' => [
        ':input[name="uikit_source"]' => ['value' => 'local'],
      ],
    ],
  ];
  $form['uikit_libraries']['uikit_icons_header'] = [
    '#markup' => '<hr>',
  ];
  $form['uikit_libraries']['uikit_icons'] = [
    '#type' => 'checkbox',
    '#title' => t('Load UIkit icon library'),
    '#description' => t('View the available icons at <a href="https://getuikit.com/docs/icon" target="_blank" rel="noopener">https://getuikit.com/docs/icon</a>'),
    '#default_value' => theme_get_setting('uikit_icons', $theme_name),
  ];
  $form['uikit_libraries']['uikit_warning_header'] = [
    '#markup' => '<hr>',
  ];
  $form['uikit_libraries']['uikit_version_alert'] = [
    '#type' => 'checkbox',
    '#title' => t('New version alert'),
    '#description' => t('Display a message when a new version of the UIkit framework is available.'),
    '#default_value' => theme_get_setting('uikit_version_alert', $theme_name),
  ];
  $form['uikit_libraries']['uikit_doc_links'] = [
    '#type' => 'checkbox',
    '#title' => t('View documentation links'),
    '#description' => t('Show links to documentation and examples on the UIkit website.'),
    '#default_value' => theme_get_setting('uikit_doc_links', $theme_name),
  ];


  /*
  **  About UIkit
  */
  $form['about_uikit'] = [
    '#type' => 'fieldset',
    '#title' => t('About UIkit'),
    '#group' => 'uikit',
  ];
  $uikit_description = 'UIkit is a lightweight and modular front-end framework for developing fast and powerful web interfaces.  UIkit gives you a comprehensive collection of HTML, CSS, and JS components. It can be extended with themes and is easy to customize to create your own look.';
  $uikit_info = '<div style="text-align: center">';
  $uikit_info .= '<img src="/' . backdrop_get_path('theme', 'uikit') . '/images/uikit-small.png" />';
  $uikit_info .= '<blockquote>';
  $uikit_info .= t('@description', ['@description' => $uikit_description]);
  $uikit_info .= '</blockquote>';
  $uikit_info .= '<p>';
  $uikit_info .= t('UIkit made by <strong><a href="@yootheme" target="_blank" rel="noopener">YOOtheme</a></strong>, with love and caffeine, under the <a href="@license" target="_blank" rel="noopener">MIT license</a>.', ['@yootheme' => 'http://www.yootheme.com', '@license' => 'http://opensource.org/licenses/MIT']);
  $uikit_info .= '<br>';
  $uikit_info .= t('Ported to Drupal by <a href="@richardbuchanan" target="_blank" rel="noopener" style="font-weight: bold">Richard Buchanan</a>', ['@richardbuchanan' => 'https://www.drupal.org/u/richard-buchanan']);
  $uikit_info .= '</p><p>';
  $uikit_info .= t('<a href="@uikit" target="_blank" rel="noopener" style="font-weight: bold">Documentation</a> | <a href="@overview" target="_blank" rel="noopener" style="font-weight: bold">Examples</a>', ['@uikit' => 'https://www.getuikit.com/docs/introduction', '@overview' => 'https://getuikit.com/assets/uikit/tests/index.html']);
  $uikit_info .= '</p>';
  $uikit_info .= '</div>';
  $form['about_uikit']['info'] = [
    '#markup' => $uikit_info,
  ];


  /*
  **  Header block settings
  */
  $form['header_block'] = [
    '#type' => 'fieldset',
    '#title' => t('Header Block'),
    '#description' => t('Settings specific to the Header Block in Layouts.  The selected menu has different settings than '),
    '#group' => 'uikit',
    '#attributes' => [
      'class' => [
        'uikit-menu-settings-form',
      ],
    ],
  ];
  $form['header_block']['menu_header_transparent'] = [
    '#type' => 'checkbox',
    '#title' => t('Transparent Header Menu'),
    '#description' => t('The menu selected in the header block'),
    '#default_value' => theme_get_setting('menu_header_transparent', $theme_name),
  ];
  $menu_position_options = [
    'uk-navbar-left' => 'Left',
    'uk-navbar-center' => 'Center',
    'uk-navbar-right' => 'Right',
  ];
  $form['header_block']['menu_header_position'] = [
    '#type' => 'select',
    '#title' => t('Header menu position'),
    '#description' => t('Alignment of the selected menu in the header block'),
    '#options' => $menu_position_options,
    '#default_value' => theme_get_setting('menu_header_position', $theme_name),
  ];
  $form['header_block']['menu_header_icon'] = [
    '#type' => 'checkbox',
    '#title' => t('Header menu icon'),
    '#description' => t('Show the menu icon when the menu is collapsed on smaller screens.'),
    '#options' => $menu_position_options,
    '#default_value' => theme_get_setting('menu_header_icon', $theme_name),
  ];


  /*
  **  Menu settings
  */
  $form['menus'] = [
    '#type' => 'fieldset',
    '#title' => t('Menu settings'),
    '#description' => t('Modify the menu positions'),
    '#group' => 'uikit',
    '#attributes' => [
      'class' => [
        'uikit-menu-settings-form',
      ],
    ],
  ];
  $menus = menu_load_all();
  $offcanvas_menu_options = [];
  foreach ($menus as $menu) {
    $offcanvas_menu_options[$menu['menu_name']] = $menu['title'];
    $form_field = implode('_', [$menu['menu_name'], 'position']);
    $form['menus'][$form_field] = [
      '#type' => 'select',
      '#title' => $menu['title'],
      '#options' => $menu_position_options,
      '#default_value' => theme_get_setting($form_field, $theme_name),
    ];
  }
  $offcanvas_menu_options['none'] = 'None';
  $form['menus']['offcanvas'] = [
    '#type' => 'fieldset',
    '#title' => t('Off-canvas Options'),
    '#description' => t('Create an off-canvas sidebar that slides in and out of the page, which is perfect for creating mobile navigations. <a href="@offcanvas_docs" target="_blank" rel="noopener">Documentation</a>', ['@offcanvas_docs' => 'https://getuikit.com/docs/offcanvas']),
  ];
  $form['menus']['offcanvas']['offcanvas_primary'] = [
    '#type' => 'select',
    '#title' => t('Primary Off-canvas Menu'),
    '#default_value' => theme_get_setting('offcanvas_primary', $theme_name),
    '#options' => $offcanvas_menu_options,
  ];
  $form['menus']['offcanvas']['offcanvas_secondary'] = [
    '#type' => 'select',
    '#title' => t('Secondary Off-canvas Menu'),
    '#default_value' => theme_get_setting('offcanvas_secondary', $theme_name),
    '#options' => $offcanvas_menu_options,
  ];
  $form['menus']['offcanvas']['offcanvas_mode'] = [
    '#type' => 'select',
    '#title' => t('Mode'),
    '#description' => t('Off-canvas animation mode'),
    '#default_value' => theme_get_setting('offcanvas_mode', $theme_name),
    '#options' => [
      'slide' => 'Slide',
      'reveal' => 'Reveal',
      'push' => 'Push',
      'none' => 'None',
    ],
  ];
  $form['menus']['offcanvas']['offcanvas_flip'] = [
    '#type' => 'checkbox',
    '#title' => t('Flip'),
    '#description' => t('Off-canvas menu defaults to the left side.  Flip off-canvas to the right side.'),
    '#default_value' => theme_get_setting('offcanvas_flip', $theme_name),
  ];
  $form['menus']['offcanvas']['offcanvas_overlay'] = [
    '#type' => 'checkbox',
    '#title' => t('Overlay'),
    '#description' => t('Display the off-canvas together with an overlay.'),
    '#default_value' => theme_get_setting('offcanvas_overlay', $theme_name),
  ];


  /*
  **  Layout settings
  */
  $form['layout'] = [
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
    '#description' => t('Apply fully responsive fluid grid system and cards, common layout parts like blog articles and comments and useful utility classes.'),
    '#group' => 'uikit',
    '#attributes' => [
      'class' => [
        'uikit-layout-settings-form',
      ],
    ],
  ];
  
  $layouts = layout_load_all();
  foreach ($layouts as $layout_key => $layout) {
    $form['layout'][$layout->name] = [
      '#type' => 'fieldset',
      '#title' => t($layout->title),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    ];
    $form['layout'][$layout->name]['info'] = [
      '#markup' => '<a href="/admin/structure/layouts/manage/' . $layout->name . '/blocks?destination=admin/appearance/settings/' . $theme_name . '">Manage blocks</a> for this layout.',
    ];
    $positions = array_merge(['main-content-wrapper' => ['main-content-wrapper']], $layout->positions);
    foreach ($positions as $region => $region_info) {
      $form_field = implode("_", [$layout->name, $region, "classes"]);
      $form['layout'][$layout->name][$region] = [
        '#type' => 'fieldset',
        '#title' => ucwords(str_replace(['-','_'], ' ', $region)),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ];
      if ($region == 'main-content-wrapper') {
        $form['layout'][$layout->name][$region]['info'] = [
          '#markup' => 'Most layouts have a main content wrapper that includes all regions not in the header or footer.  This is not configurable in layouts, but if configured properly in your theme, the wrapper <div> can be adjusted here.',
        ];
      }
      $form['layout'][$layout->name][$region][$form_field] = [
        '#type' => 'textfield',
        '#title' => t('Region CSS classes'),
        '#description' => t('Separate class names with spaces. Example: <code>uk-section uk-section-default uk-padding</code>'),
        '#default_value' => theme_get_setting($form_field, $theme_name),
      ];
      $background_file_id = theme_get_setting(implode("_", [$layout->name, $region, "background"]), $theme_name);
      if (!empty($background_file_id)) {
//        $background_file = file_load($background_file_id);
//        file_usage_add($background_file, 'uikit', 'theme', $background_file->fid);
      }
      $form['layout'][$layout->name][$region][implode("_", [$layout->name, $region, "background"])] = [
        '#type' => 'managed_file',
        '#title' => t('Background image'),
        '#description' => t('Space-delimited list of classes to apply to this region'),
        '#default_value' => theme_get_setting(implode("_", [$layout->name, $region, "background"]), $theme_name),
        '#upload_location' => 'public://background/',
        '#upload_validators' => [
          'file_validate_extensions' => ['jpg jpeg png gif webp'],
        ],
      ];
      $form['layout'][$layout->name][$region][implode("_", [$layout->name, $region, "parallax"])] = [
        '#type' => 'textfield',
        '#title' => t('Parallax settings'),
        '#default_value' => theme_get_setting(implode("_", [$layout->name, $region, "parallax"]), $theme_name),
      ];
      if ($showlinks) {
        $form['layout'][$layout->name][$region][implode("_", [$layout->name, $region, "parallax"])]['#description'] = t('<p style="background-color: #eafaf1 ; padding: 5px 10px; border: 2px solid  #27ae60; border-radius: 4px;">View available parallax properties at <a href="https://getuikit.com/docs/parallax" target="_blank" rel="noopener">https://getuikit.com/docs/parallax</a>.</p>');
      }
    }
  }


  /*
  **  Navigations
  */
  $form['navigations'] = [
    '#type' => 'fieldset',
    '#title' => t('Navigations'),
    '#description' => t('UIkit offers different types of navigations, like navigation bars and side navigations. Use breadcrumbs or a pagination to steer through articles.'),
    '#group' => 'uikit',
  ];
  $form['navigations']['local_tasks'] = [
    '#type' => 'fieldset',
    '#title' => t('Local tasks'),
    '#description' => t('Configure settings for the local tasks menus.'),
  ];
  $primary_subnav_options = [
    0 => t('Basic subnav'),
    'uk-subnav-pill' => t('Subnav pill'),
    'uk-tab' => t('Tabbed'),
  ];
  $secondary_subnav_options = [
    0 => t('Basic subnav'),
    'uk-subnav-pill' => t('Subnav pill'),
  ];
  $form['navigations']['local_tasks']['primary_tasks'] = [
    '#type' => 'container',
  ];
  $form['navigations']['local_tasks']['primary_tasks']['primary_tasks_style'] = [
    '#type' => 'select',
    '#title' => t('Primary tasks style'),
    '#description' => t('Select the style to apply to the primary local tasks.'),
    '#default_value' => theme_get_setting('primary_tasks_style', $theme_name),
    '#options' => $primary_subnav_options,
  ];
  $form['navigations']['local_tasks']['secondary_tasks'] = [
    '#type' => 'container',
  ];
  $form['navigations']['local_tasks']['secondary_tasks']['secondary_tasks_style'] = [
    '#type' => 'select',
    '#title' => t('Secondary tasks style'),
    '#description' => t('Select the style to apply to the secondary local tasks.'),
    '#default_value' => theme_get_setting('secondary_tasks_style', $theme_name),
    '#options' => $secondary_subnav_options,
  ];
  $form['navigations']['local_tasks']['footer'] = [
    '#markup' => '<p>Another description</p>',
  ];


  /*
  **  Breadcrumb settings
  */
  $form['breadcrumbs'] = [
    '#type' => 'fieldset',
    '#title' => t('Breadcrumbs'),
    '#description' => t('Configure settings for breadcrumb navigation.'),
    '#group' => 'uikit',
  ];
  $form['breadcrumbs']['breadcrumbs_display'] = [
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumbs'),
    '#description' => t('Check this box to display the breadcrumb.'),
    '#default_value' => theme_get_setting('breadcrumbs_display', $theme_name),
  ];
  $form['breadcrumbs']['breadcrumbs_home_link'] = [
    '#type' => 'checkbox',
    '#title' => t('Display home link in breadcrumbs'),
    '#description' => t('Check this box to display the home link in breadcrumb trail.'),
    '#default_value' => theme_get_setting('breadcrumbs_home_link', $theme_name),
    '#states' => [
      'invisible' => [
        ':input[name="breadcrumbs_display"]' => ['checked' => FALSE],
      ],
    ],
  ];
  $form['breadcrumbs']['breadcrumbs_current_page'] = [
    '#type' => 'checkbox',
    '#title' => t('Display current page title in breadcrumbs'),
    '#description' => t('Check this box to display the current page title in breadcrumb trail.'),
    '#default_value' => theme_get_setting('breadcrumbs_current_page', $theme_name),
    '#states' => [
      'invisible' => [
        ':input[name="breadcrumbs_display"]' => ['checked' => FALSE],
      ],
    ],
  ];


  /*
  **  Theme debugging
  */
  $form['debug'] = [
    '#type' => 'fieldset',
    '#title' => t('Theme debugging'),
    '#group' => 'uikit',
  ];
  $form['debug']['enable'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable theme debugging output'),
    '#default_value' => config_get('system.core', 'theme_debug'),
    '#description' => t('Output theme debugging information to source code of page - do not forget to disable this on a live site!'),
  ];


  /*
  **  Custom CSS
  */
  $form['custom_css'] = [
    '#type' => 'fieldset',
    '#title' => t('Custom CSS'),
    '#description' => t('Add custom CSS rules.'),
    '#group' => 'uikit',
  ];
  $form['custom_css']['custom_css_checkbox'] = [
    '#type' => 'checkbox',
    '#title' => t('Add custom CSS'),
    '#default_value' => theme_get_setting('custom_css_checkbox', $theme_name),
  ];
  $form['custom_css']['custom_css_code'] = [
    '#type' => 'textarea',
    '#title' => t('Add your custom CSS rules'),
    '#description' => t('Note that you can not preview these rules here.'),
    '#default_value' => theme_get_setting('custom_css_code', $theme_name),
    '#rows' => 12,
    '#states' => [
      'invisible' => [
        ':input[name="custom_css_checkbox"]' => ['checked' => FALSE],
      ],
    ],
  ];


  /*
  **  Export theme settings
  */
  /*
  $form['export'] = [
    '#type' => 'fieldset',
    '#title' => t('Export theme settings'),
    '#group' => 'uikit',
  ];
  $config_export = [];
  $config_export[] = 'settings[uikit_source] = ' . theme_get_setting('uikit_source', $theme_name);
  $config_export[] = 'settings[uikit_local_version] = ' . theme_get_setting('uikit_local_version', $theme_name);
  $config_export[] = 'settings[uikit_online_version] = ' . theme_get_setting('uikit_online_version', $theme_name);
  $config_export[] = 'settings[uikit_icons] = ' . theme_get_setting('uikit_icons', $theme_name);
  $config_export[] = 'settings[page_container] = ' . theme_get_setting('page_container', $theme_name);
  $config_export[] = 'settings[page_margin] = ' . theme_get_setting('uikit_online_version', $theme_name);
  $config_export[] = 'settings[primary_tasks_style] = ' . theme_get_setting('primary_tasks_style', $theme_name);
  $config_export[] = 'settings[secondary_tasks_style] = ' . theme_get_setting('secondary_tasks_style', $theme_name);
  $config_export[] = 'settings[breadcrumbs_display] = ' . theme_get_setting('breadcrumbs_display', $theme_name);
  $config_export[] = 'settings[breadcrumbs_home_link] = ' . theme_get_setting('breadcrumbs_home_link', $theme_name);
  $config_export[] = 'settings[breadcrumbs_current_page] = ' . theme_get_setting('breadcrumbs_current_page', $theme_name);
  $config_export[] = 'settings[breadcrumbs_divider] = ' . theme_get_setting('breadcrumbs_divider', $theme_name);
  $config_export[] = 'settings[custom_css_code] = ' . theme_get_setting('custom_css_code', $theme_name);
  $form['export']['intro'] = [
    '#markup' => '<p><strong>Export theme settings</strong></p>
      <p>Easily create a new sub-theme by copying/pasting the following theme settings into a subtheme.info file.</p>',
  ];
  $form['export']['code'] = [
    '#markup' => '<p style="border: 1px solid #f1f1f1; padding: 10px 20px; font-family: monospace; color: green">' . trim(implode("<br>", $config_export)) . '</p>',
  ];
  */


  /*
  **  Theme settings validation
  */
  $form['#validate'][] = '_uikit_theme_settings_validate';

  $form['#submit'] = [
    '_uikit_theme_settings_submit',
    'system_theme_settings_submit',
  ];
}

function _uikit_theme_settings_validate($form, &$form_state) {
  $theme_name = $form['theme']['#value'];
  if ($form_state['values']['uikit_source'] == 'local') {
    if (empty($form_state['values']['uikit_local_version'])) {
      form_set_error('uikit_source', 'You must choose a local version in the theme /src directory');
    }
    else {
      // Verify the local source files exist
      $local_source_path = backdrop_get_path('theme', $theme_name) . '/src/' . $form_state['values']['uikit_local_version'] . '/';
      $minimized_css_file = $local_source_path . 'css/uikit.min.css';
      $minimized_js_file = $local_source_path . 'js/uikit.min.js';
      $minimized_icons_file = $local_source_path . 'js/uikit-icons.min.js';
      // Check for the minimised CSS file
      $missing_library_file = FALSE;
      if (!file_exists($minimized_css_file)) {
        form_set_error('uikit_source1', 'Missing the UIkit CSS file: ' . $minimized_css_file);
        $missing_library_file = TRUE;
      }
      if (!file_exists($minimized_css_file)) {
        form_set_error('uikit_source2', 'Missing the UIkit JS file: ' . $minimized_js_file);
        $missing_library_file = TRUE;
      }
      if ($form_state['values']['uikit_icons'] && !file_exists($minimized_icons_file)) {
        form_set_error('uikit_source3', 'Missing the UIkit icon file: ' . $minimized_icons_file);
        $missing_library_file = TRUE;
      }
      if ($missing_library_file) {
        $release_url = "https://github.com/uikit/uikit/releases";
        backdrop_set_message('UIkit source files can be found at <a href="' . $release_url . '" target="_blank" rel="noopener">' . $release_url . '</a>');
      }
    }
  }
}

function _uikit_theme_settings_submit($form, &$form_state) {
  // Save the background files
  foreach ($form_state['values'] as $field_name => $file_id) {
    if (preg_match('/_background$/', $field_name) === 1 && !empty($file_id)) {
      $uploaded_file = file_load($file_id);
      file_usage_add($uploaded_file, 'uikit', 'theme', $uploaded_file->fid);
      if ($uploaded_file->status != FILE_STATUS_PERMANENT) {
        $uploaded_file->display = 1;
        $uploaded_file->status = FILE_STATUS_PERMANENT;
        file_save($uploaded_file);
      }
    }
  }
  
  // Save css file to disk.
  $theme_name = $form_state['values']['theme'];
  $filepath = backdrop_realpath("public://{$theme_name}_custom.css");
  $custom_css_code = "";
  if ($form_state['values']['breadcrumbs_display'] && !empty($form_state['values']['breadcrumbs_divider'])) {
    $custom_css_code .= '.uk-breadcrumb > :nth-child(n+2):not(.uk-first_column)::before { ' .
      'content: "' . $form_state['values']['breadcrumbs_divider'] . '"; }';
  }
  if ($form_state['values']['custom_css_checkbox'] && !empty(trim(strip_tags($form_state['values']['custom_css_code'])))) {
    $custom_css_code .= trim(strip_tags($form_state['values']['custom_css_code']));
    $form_state['values']['custom_css_code'] = $custom_css_code;
  }
  if (empty($custom_css_code)) {
    file_unmanaged_delete($filepath);
  }
  else {
    $warn = "/**\n * Do not edit this file directly, your changes will be lost!\n */\n";
    file_unmanaged_save_data($warn . $custom_css_code, $filepath, FILE_EXISTS_REPLACE);
  }

  // Enable or disable theme debugging
  config_set('system.core', 'theme_debug', $form_state['values']['enable']);
}
