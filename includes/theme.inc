<?php

/**
 * @file
 * The UIkit theme system, which controls the output of the UIkit theme.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by UIkit themes.
 *
 * @ingroup uikit_themeable
 */

/**
 * Implements theme_admin_block_content().
 */
function uikit_admin_block_content($variables) {
  $content = $variables['content'];
  $output = '';

  if (!empty($content)) {
    $class = 'uk-description-list-line';

    if ($compact = system_admin_compact_mode()) {
      $class .= ' compact';
    }

    $output .= '<dl class="' . $class . '">';

    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';

      if (!$compact && isset($item['description'])) {
        $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
      }
    }

    $output .= '</dl>';
  }
  return $output;
}

/**
 * Implements theme_breadcrumb().
 */
function uikit_breadcrumb($variables) {
  if (backdrop_is_front_page() || !theme_get_setting('breadcrumbs_display')) {
    return;
  }

  $output = '';

  if (!$variables['breadcrumb']) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .uk-hidden.
    $output .= '<h2 hidden>' . t('You are here') . '</h2>';
    $output .= '<nav aria-label="' . t('Navigation') . '">';
    $output .= '<ul class="uk-breadcrumb"><li>';
    $output .= implode('</li><li>', $variables['breadcrumb']);
    $output .= '</li></ul>';
  }

  return $output;
}

/**
 * Implements theme_button().
 */
function uikit_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));
  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $value = $element['#attributes']['value'];

  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<button' . backdrop_attributes($element['#attributes']) . '>' . $value . '</button>';
}

/**
 * Implements theme_checkbox().
 */
function uikit_checkbox($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'checkbox';
  element_set_attributes($element, array(
    'id',
    'name',
    '#return_value' => 'value',
  ));

  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-checkbox', 'uk-checkbox'));

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<input' . backdrop_attributes($element['#attributes']) . ' />';
}

/**
 * Implements theme_container().
 */
function uikit_container($variables) {
  $element = $variables['element'];
  // Ensure #attributes is set.
  $element += array('#attributes' => array());
  $prefix = '';
  $suffix = '';

  // Special handling for form elements.
  if (isset($element['#array_parents'])) {
    // Assign an html ID.
    if (!isset($element['#attributes']['id'])) {
      $element['#attributes']['id'] = $element['#id'];
    }

    // Add the 'form-wrapper' class.
    if (isset($element['#type']) && $element['#type'] == 'container') {
      // Ensure that the class attribute is set, while preserving any existing
      // classes.
      $element['#attributes'] += array('class' => array());
      if (!in_array('form-wrapper', $element['#attributes']['class'])) {
        $element['#attributes']['class'][] = 'form-wrapper';
      }
    }
  }

  if (isset($element['#indentation'])) {
      // Ensure that the class attribute is set, while preserving any existing
      // classes.
      $element['#attributes'] += ['class' => []];
      $element['#attributes']['class'][] = 'form-item-indentation';
      $element['#attributes']['class'][] = 'form-item-indentation-' . $element['#indentation'];
      $element['#attributes']['data-indentation-depth'] = $element['#indentation'];
  }

  if (isset($element['#type']) && $element['#type'] === 'actions') {
    $prefix .= '<div class="uk-button-group">';
    $suffix .= '</div>';
  }

  return '<div' . backdrop_attributes($element['#attributes']) . '>' . $prefix . $element['#children'] . $suffix . '</div>';
}

/**
 * Implements theme_date_text().
 */
function uikit_date_text($variables) {
  $element = $variables['element'];
  $attributes = !empty($element['#wrapper_attributes']) ? $element['#wrapper_attributes'] : ['class' => []];
  $attributes['class'][] = 'container-inline-date';
  // If there is no description, the floating date elements need some extra
  // padding below them.
  $wrapper_attributes = [];
  if (empty($element['date']['#description'])) {
    $wrapper_attributes['class'][] = 'clearfix';
  }
  // Add an wrapper to mimic the way a single value field works, for ease in
  // using #states.
  if (isset($element['#children'])) {
    $wrapper_attributes['id'] = $element['#id'];
    $element['#children'] = '<div ' . backdrop_attributes($wrapper_attributes) . '>' . $element['#children'] . '</div>';
  }
  return '<div ' . backdrop_attributes($attributes) . '>' . theme('form_element', $element) . '</div>';
}


/**
 * Implements theme_field() for tag fields.
 */
/**
 * Overrides theme_field__FIELD_TYPE().
 */
function uikit_field__field_tags($variables) {
//function uikit_field__taxonomy_term_reference($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h4 class="field-label">' . $variables['label'] . ': </h4>';
  }
  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline uk-subnav">' : '<ul class="links uk-subnav">';
  foreach ($variables['items'] as $delta => $item) {
    $classes = [
      'class' => [
        'taxonomy-term-reference-' . $delta,
      ],
    ];
    if (backdrop_get_path_alias($item['#href']) == backdrop_get_path_alias(current_path())) {
      $classes['class'][] = 'uk-active';
    }
    $item_attributes = (isset($variables['item_attributes'][$delta])) ? array_merge($variables['item_attributes'][$delta], $classes) : $classes;
    $output .= '<li ' . backdrop_attributes($item_attributes) . '>' . backdrop_render($item) . '</li>';
  }
  $output .= '</ul>';
  
  // Render the top-level DIV.
  $output = '<div class="' . implode(' ', $variables['classes']) . '"' . backdrop_attributes($variables['attributes']) . '>' . $output . '</div>';
  return $output;
}

/**
 * Implements theme_fieldset().
 */
function uikit_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));
  $output = '<fieldset' . backdrop_attributes($element['#attributes']) . '>';

  if (!empty($element['#title'])) {
    $output .= '<legend class="uk-legend">' . $element['#title'] . '</legend>';
  }

  if (!empty($element['#description'])) {
    $output .= '<p class="uk-text-small uk-text-muted uk-margin-small-top">' . $element['#description'] . '</p>';
  }

  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</fieldset>';

  return $output;
}

/**
 * Implements theme_fieldset() for collapsible fieldsets.
 */
function uikit_fieldset__collapsible($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper', 'uk-form-width-large'));

  $output = '<ul' . backdrop_attributes($element['#attributes']) . '>';

  $open = $element['#collapsed'] ? '' : ' class="uk-open"';
  $output .= '<li' . $open . '>';

  if (!empty($element['#title'])) {
    $output .= '<a class="uk-accordion-title">' . $element['#title'] . '</a>';
  }

  $classes = ['uk-accordion-content'];

  if (!empty($element['guidelines'])) {
    $classes[] = "uk-section";
    $classes[] = "uk-section-secondary";
    $classes[] = "uk-padding";
  }

  $output .= '<div class="' . implode(' ', $classes) . '">';

  if (!empty($element['#description'])) {
    $output .= '<p class="uk-text-small uk-text-muted uk-margin-small-top">' . $element['#description'] . '</p>';
  }

  $output .= $element['#children'];

  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }

  $output .= '</div>';
  $output .= '</li>';
  $output .= '</ul>';

  return $output;
}

/**
 * Implements theme_fieldset() for formatted fieldsets.
 */
function uikit_fieldset__format($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper', 'uk-card', 'uk-card-body'));
  $output = '<div' . backdrop_attributes($element['#attributes']) . '>';

  if (!empty($element['#title'])) {
    $output .= '<legend class="fieldset-legend">' . $element['#title'] . '</legend>';
  }

  if (!empty($element['#description'])) {
    $output .= '<p class="uk-text-small uk-text-muted uk-margin-small-top">' . $element['#description'] . '</p>';
  }

  $output .= $element['#children'];

  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }

  $output .= '</div>';

  return $output;
}

/**
 * Implements theme_fieldset() for grouped fieldsets.
 */
function uikit_fieldset__grouped($variables) {
  $element = $variables['element'];
  element_set_attributes($element, ['id']);
  _form_set_class($element, ['form-wrapper']);
  $output = '<fieldset' . backdrop_attributes($element['#attributes']) . '>';

  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend class="fieldset-legend">' . $element['#title'] . '</legend>';
  }

  if (!empty($element['#description'])) {
    $output .= '<p class="uk-text-small uk-text-muted uk-margin-small-top">' . $element['#description'] . '</p>';
  }

  $output .= $element['#children'];

  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }

  $output .= '</fieldset>';

  return $output;
}

/**
 * Implements theme_file().
 */

function uikit_file($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'file';
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-file', 'uk-input'));

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<input' . backdrop_attributes($element['#attributes']) . ' />';
}

/**
 * Implements theme_filter_tips().
 */
function uikit_filter_tips($variables) {
  $tips = $variables['tips'];
  $long = $variables['long'];
  $output = '';

  $multiple = count($tips) > 1;
  if ($multiple) {
    $output = '<h2>' . t('Text Formats') . '</h2>';
  }

  if (count($tips)) {
    if ($multiple) {
      $output .= '<div class="compose-tips">';
    }

    foreach ($tips as $name => $tiplist) {
      if ($multiple) {
        $output .= '<div class="filter-type filter-' . backdrop_html_class($name) . '">';
        $output .= '<h3>' . check_plain($name) . '</h3>';
      }

      if (count($tiplist) > 0) {
        $output .= '<ul class="tips uk-list uk-list-square uk-margin-remove uk-margin-small-top">';
        foreach ($tiplist as $tip) {
          $output .= '<li class="uk-text-small" ' . ($long ? ' id="filter-' . str_replace("/", "-", $tip['id']) . '">' : '>') . $tip['tip'] . '</li>';
        }
        $output .= '</ul>';
      }

      if ($multiple) {
        $output .= '</div><hr class="uk-article-divider">';
      }
    }

    if ($multiple) {
      $output .= '</div>';
    }
  }

  return $output;
}

/**
 * Implements theme_form().
 */
function uikit_form($variables) {
  $element = $variables['element'];

  $horizontal_forms = array(
    'node_admin_content',
  );

  if (in_array($element['#form_id'], $horizontal_forms)) {
    $element['#attributes']['class'][] = 'uk-form-horizontal';
  }
  else {
    $element['#attributes']['class'][] = 'uk-form-stacked';
  }

  if (isset($element['#action'])) {
    $element['#attributes']['action'] = backdrop_strip_dangerous_protocols($element['#action']);
  }

  element_set_attributes($element, array('method', 'id'));

  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }

  return '<form' . backdrop_attributes($element['#attributes']) . '><div>' . $element['#children'] . '</div></form>';
}

/**
 * Implements theme_form_element().
 */
function uikit_form_element($variables) {
  $element = &$variables['element'];
  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += [
    '#title_display' => 'before',
    '#wrapper_attributes' => [],
  ];

  $checkbox = (!empty($element['#type']) && $element['#type'] === 'checkbox') ? TRUE : FALSE;
  $radio = (!empty($element['#type']) && $element['#type'] === 'radio') ? TRUE : FALSE;
  $select = (!empty($element['#type']) && $element['#type'] === 'select') ? TRUE : FALSE;

  // Create an attributes array for the wrapping container.
  if (empty($element['#wrapper_attributes'])) {
    $element['#wrapper_attributes'] = array();
  }

  $wrapper_attributes = &$element['#wrapper_attributes'];

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add wrapper ID for 'item' type.
  if (!empty($element['#type']) && $element['#type'] === 'item' && !empty($element['#markup']) && !empty($element['#id'])) {
    $wrapper_attributes['id'] = $element['#id'];
  }

  // Add necessary classes to wrapper container.
  if (!empty($element['#name'])) {
    $wrapper_attributes['class'][] = 'form-item-' . backdrop_html_class($element['#name']);
  }
  if (!empty($element['#type'])) {
    $wrapper_attributes['class'][] = 'form-type-' . backdrop_html_class($element['#type']);
  }
  if (!empty($element['#attributes']['disabled'])) {
    $wrapper_attributes['class'][] = 'form-disabled';
    $wrapper_attributes['class'][] = 'uk-disabled';
  }
  if (!empty($element['#autocomplete_path']) && backdrop_valid_path($element['#autocomplete_path'])) {
    $wrapper_attributes['class'][] = 'form-autocomplete';
  }
  // Add indentation.
  if (isset($element['#indentation'])) {
    $wrapper_attributes['class'][] = 'form-item-indentation';
    $wrapper_attributes['class'][] = 'form-item-indentation-' . $element['#indentation'];
    $wrapper_attributes['data-indentation-depth'] = $element['#indentation'];
  }
  if (!$select && !$checkbox && !$radio) {
    $wrapper_attributes['class'][] = 'uk-margin';
  }
  if ($checkbox || $radio) {
    $wrapper_attributes['class'][] = 'uk-margin-small-bottom';
  }
  // TODO: Add advanced password options in theme settings.
  // Add a space before the labels of checkboxes and radios.
  if (($checkbox || $radio) && isset($element['#title'])) {
    $variables['element']['#title'] = ' ' . $element['#title'];
  }

  // Create a render array for the form element.
  $build = array(
    '#theme_wrappers' => array('container__form_element'),
    '#attributes' => $wrapper_attributes,
  );

  // Render the label for the form element.
  $build['label'] = array(
    '#markup' => theme('form_element_label', $variables),
  );

  // Increase the label weight if it should be displayed after the element.
  if ($element['#title_display'] === 'after') {
    $build['label']['#weight'] = 10;
  }

  // Checkboxes and radios render the input element inside the label. If the
  // element is neither of those, then the input element must be rendered here.
  if (!$checkbox && !$radio) {
    $prefix = isset($element['#field_prefix']) ? $element['#field_prefix'] : '';
    $suffix = isset($element['#field_suffix']) ? $element['#field_suffix'] : '';

    if ((!empty($prefix) || !empty($suffix))) {
      if (!empty($element['#field_prefix'])) {
        $prefix = '<span class="form-item-prefix">' . $prefix . '</span>';
      }
      if (!empty($element['#field_suffix'])) {
        $suffix = '<span class="form-item-suffix">' . $suffix . '</span>';
      }

      // Add a wrapping container around the elements.
      $prefix .= '<div>' . $prefix;
      $suffix .= '</div>';
    }

    // Build the form element.
    $build['element'] = array(
      '#markup' => $element['#children'],
      '#prefix' => !empty($prefix) ? $prefix : NULL,
      '#suffix' => !empty($suffix) ? $suffix : NULL,
    );
  }

  // Construct the element's description markup.
  if (!empty($element['#description'])) {
    $build['description'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'uk-text-small',
          'uk-text-muted',
          'uk-margin-small-top',
        ),
      ),
      '#weight' => 20,
      0 => array('#markup' => $element['#description']),
    );
  }

  // Print the form element build array.
  return backdrop_render($build);
}

/**
 * Implements theme_form_element_label().
 */
function uikit_form_element_label($variables) {
  $element = $variables['element'];
  
  if ((!isset($element['#title']) || $element['#title'] === '') && empty($element['#required'])) {
    return '';
  }

  $t = get_t();
  // If the element is required, a required marker is appended to the label.
  $required = $t('!required', ['!required' => !empty($element['#required']) ? theme('form_required_marker', ['element' => $element]) : '']);
  $title = !empty($element['#title']) ? filter_xss_admin($element['#title']) : '';
  
  $attributes = [];
  $display = isset($element['#title_display']) ? $element['#title_display'] : 'before';
  $type = !empty($element['#type']) ? $element['#type'] : FALSE;
  $checkbox = ($type && $type === 'checkbox');
  $radio = ($type && $type === 'radio');

  if (!$checkbox && !$radio) {
    $attributes['class'][] = 'uk-form-label';
  }

  $output = '';

  // Only show the required marker if there is an actual title to display.
  if ($title && $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '') {
    $title .= ' ' . $required;
  }

  // Immediately return if the element is not a checkbox or radio and there is
  // no label to be rendered.
  if (!$checkbox && !$radio && ($display === 'none' || !$title)) {
    return '';
  }

  // Add the necessary 'for' attribute if the element ID exists.
  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // Checkboxes and radios must construct the label differently.
  if ($checkbox || $radio) {
    if ($display === 'before') {
      $output .= $title;
    }
    elseif ($display === 'none' || $display === 'invisible') {
      $output .= '<span hidden>' . $title . '</span>';
    }

    // Inject the rendered checkbox or radio element inside the label.
    if (!empty($element['#children'])) {
      $output .= $element['#children'];
    }

    if ($display === 'after') {
      $output .= $title;
    }
  }
  // Otherwise, just render the title as the label.
  else {
    // Show label only to screen readers to avoid disruption in visual flows.
    if ($display === 'invisible') {
      $attributes['class'][] = 'uk-hidden';
    }

    $output .= $title;
  }

  // Add the necessary 'for' attribute if the element ID exists.
  if (!empty($element['#label_for'])) {
    $attributes['for'] = $element['#label_for'];
  }
  elseif (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . backdrop_attributes($attributes) . '>' . $output . '</label>';
}

/**
 * Implements theme_form_required_marker().
 */
function uikit_form_required_marker($variables) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();
  $attributes = [
    'class' => [
      'form-required',
      'uk-text-danger',
    ],
    'title' => $t('This field is required.'),
  ];
  return '<sup' . backdrop_attributes($attributes) . '>*</sup>';
}

/**
 * Implements theme_link().
 */
function uikit_link($variables) {
  $path = $variables['path'];
  $options = $variables['options'];
  $attributes = $variables['options']['attributes'];
  $text = $variables['options']['html'] ? $variables['text'] : check_plain($variables['text']);

  switch ($path) {
    case '<uk-nav-header>':
      return $text;

    case '<uk-parent>':
      return '<a href="#" ' . backdrop_attributes($attributes) . '>' . $text . '</a>';

    case '<uk-nav-divider>':
      return '';

    default:
      return '<a href="' . check_plain(url($path, $options)) . '" ' . backdrop_attributes($attributes) . '>' . $text . '</a>';
  }
}

/**
 * We can customize the header menu links if we really want to.
 */
function uikit_links__header_menu($variables) {
  global $language_url;
  
  $links = (array) $variables['links'];
  $attributes = (array) $variables['attributes'];
  $heading = $variables['heading'];
  $output = '';

  if (!empty($links)) {
    // Prepend the heading to the list, if any.
    if (!empty($heading)) {
      // Convert a string heading into an array, using a H2 tag by default.
      if (is_string($heading)) {
        $heading = array('text' => $heading);
      }
      // Merge in default array properties into $heading.
      $heading += array(
        'level' => 'h2',
        'attributes' => array(),
      );
      // @todo Remove backwards compatibility for $heading['class'].
      if (isset($heading['class'])) {
        $heading['attributes']['class'] = $heading['class'];
      }

      $output .= '<' . $heading['level'] . backdrop_attributes($heading['attributes']) . '>';
      $output .= check_plain($heading['text']);
      $output .= '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . backdrop_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 0;
    foreach ($links as $key => $link) {
      $i++;

      $class = array();
      // Use the array key as class name.
      $class[] = backdrop_html_class($key);
      // Add odd/even, first, and last classes.
      $class[] = ($i % 2 ? 'odd' : 'even');
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }

      // Handle links.
      if (isset($link['href'])) {
        $is_current_path = ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && backdrop_is_front_page()));
        $is_current_language = (empty($link['language']) || $link['language']->langcode == $language_url->langcode);
        if ($is_current_path && $is_current_language) {
          $class[] = 'uk-active';
        }
        // Pass in $link as $options, they share the same keys.
        $item = l($link['title'], $link['href'], $link);
      }
      // Handle title-only text items.
      else {
        // Merge in default array properties into $link.
        $link += array(
          'html' => FALSE,
          'attributes' => array(),
        );
        $item = '<span' . backdrop_attributes($link['attributes']) . '>';
        $item .= ($link['html'] ? $link['title'] : check_plain($link['title']));
        $item .= '</span>';
      }

      $output .= '<li' . backdrop_attributes(array('class' => $class)) . '>';
      $output .= $item;
      $output .= '</li>';
    }

    $output .= '</ul>';
  }

  return $output;
}



/**
 * Implements theme_links().
 */
function uikit_links($variables) {
  global $language_url;
  
  $links = (array) $variables['links'];
  $attributes = (array) $variables['attributes'];
  $heading = $variables['heading'];
  $output = '';

  if (!empty($links)) {
    // Prepend the heading to the list, if any.
    if (!empty($heading)) {
      // Convert a string heading into an array, using a H2 tag by default.
      if (is_string($heading)) {
        $heading = array('text' => $heading);
      }
      // Merge in default array properties into $heading.
      $heading += array(
        'level' => 'h2',
        'attributes' => array(),
      );
      // @todo Remove backwards compatibility for $heading['class'].
      if (isset($heading['class'])) {
        $heading['attributes']['class'] = $heading['class'];
      }

      $output .= '<' . $heading['level'] . backdrop_attributes($heading['attributes']) . '>';
      $output .= check_plain($heading['text']);
      $output .= '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . backdrop_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 0;
    foreach ($links as $key => $link) {
      $i++;

      $class = array();
      // Use the array key as class name.
      $class[] = backdrop_html_class($key);
      // Add odd/even, first, and last classes.
      $class[] = ($i % 2 ? 'odd' : 'even');
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }

      // Handle links.
      if (isset($link['href'])) {
        $is_current_path = ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && backdrop_is_front_page()));
        $is_current_language = (empty($link['language']) || $link['language']->langcode == $language_url->langcode);
        if ($is_current_path && $is_current_language) {
          $class[] = 'uk-active';
        }
        // Pass in $link as $options, they share the same keys.
        $item = l($link['title'], $link['href'], $link);
      }
      // Handle title-only text items.
      else {
        // Merge in default array properties into $link.
        $link += array(
          'html' => FALSE,
          'attributes' => array(),
        );
        $item = '<span' . backdrop_attributes($link['attributes']) . '>';
        $item .= ($link['html'] ? $link['title'] : check_plain($link['title']));
        $item .= '</span>';
      }

      $output .= '<li' . backdrop_attributes(array('class' => $class)) . '>';
      $output .= $item;
      $output .= '</li>';
    }

    $output .= '</ul>';
  }

  return $output;
}

/**
 * Implements theme_links() for node links.
 */
function uikit_links__node($variables) {
  $links = $variables['links'];
  $heading = $variables['heading'];
  $output = '';

  if (count($links) > 0) {
    // Treat the heading first if it is present to prepend it to the list of
    // links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading is a
        // string.
        $heading = array(
          'text' => $heading,
          // Set the default level of the heading.
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= backdrop_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    $output .= '<ul class="uk-subnav uk-subnav-divider">';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $link['attributes']['class'] = array(str_replace('_', '-', $key));

      // Add first and last classes to the list of links to help out themers.
      if ($i == 1) {
        $link['attributes']['class'][] = 'first';
      }

      if ($i == $num_links) {
        $link['attributes']['class'][] = 'last';
      }

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $output .= l($link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for
        // adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }

        $span_attributes = '';

        if (isset($link['attributes'])) {
          $span_attributes = backdrop_attributes($link['attributes']);
        }

        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      $i++;
    }

    $output .= '</ul>';

  }

  return $output;
}

/**
 * Implements theme_menu_link().
 */
function uikit_menu_link(array $variables) {
  global $user;
  $element = $variables['element'];
  $sub_menu = '';

  $options = !empty($element['#localized_options']) ? $element['#localized_options'] : array();

  // Check plain title if "html" is not set, otherwise, filter for XSS attacks.
  $title = empty($options['html']) ? check_plain($element['#title']) : filter_xss_admin($element['#title']);

  // Ensure "html" is now enabled so l() doesn't double encode. This is now
  // safe to do since both check_plain() and filter_xss_admin() encode HTML
  // entities. See: https://www.drupal.org/node/2854978
  $options['html'] = TRUE;

  $href = $element['#href'];
  $attributes = !empty($element['#attributes']) ? $element['#attributes'] : array();

  $front_page = $href == '<front>' && backdrop_is_front_page();
  $user_profile = $href === 'user' && $_GET['q'] === "user/$user->uid";

  if ($href == $_GET['q'] || $front_page || $user_profile) {
    $attributes['class'][] = 'uk-active';
  }

  // Remove expanded classes.
  if (isset($attributes['class']) && ($key = array_search('expanded', $attributes['class'])) !== FALSE) {
    unset($attributes['class'][$key]);
  }

  if ($element['#below']) {
    $offcanvas = in_array('menu_tree__offcanvas__below', $element['#below']['#theme_wrappers']);

    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = backdrop_render($element['#below']);
    }
    elseif ($offcanvas) {
      $sub_menu = backdrop_render($element['#below']);

      // Generate as standard dropdown.
      $attributes['class'][] = 'uk-parent';
      $attributes['aria-expanded'] = 'false';
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
      // Use our own theme wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<div class="uk-navbar-dropdown">';
      $sub_menu .= '<ul class="uk-nav uk-navbar-dropdown-nav">' . backdrop_render($element['#below']) . '</ul>';
      $sub_menu .= '</div>';
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 2)) {
      // Use our own theme wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu .= '<ul>' . backdrop_render($element['#below']) . '</ul>';
    }
  }

  switch ($href) {
    case '<uk-nav-header>':
      $attributes['class'][] = 'uk-nav-header';
      return '<li' . backdrop_attributes($attributes) . '>' . $title . "</li>\n";

    case '<uk-nav-divider>':
      $attributes['class'][] = 'uk-nav-divider';
      return '<li' . backdrop_attributes($attributes) . '>' . l($title, $href, $options) . $sub_menu . "</li>\n";

    default:
      return '<li' . backdrop_attributes($attributes) . '>' . l($title, $href, $options) . $sub_menu . "</li>\n";
  }
}

/**
 * Implements theme_menu_local_action().
 */
function uikit_menu_local_action($variables) {
  $link = $variables['element']['#link'];
  $output = '<li class="uk-active">';

  if (isset($link['href'])) {
    $output .= l($link['title'], $link['href'], isset($link['localized_options']) ? $link['localized_options'] : array());
  }
  elseif (!empty($link['localized_options']['html'])) {
    $output .= $link['title'];
  }
  else {
    $output .= check_plain($link['title']);
  }

  $output .= "</li>\n";

  return $output;
}

/**
 * Implements theme_menu_local_task().
 */
function uikit_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $link_text = $link['title'];

  if (!empty($variables['element']['#active'])) {
    // Add text to indicate active tab for non-visual users.
    $active = '<span class="uk-hidden">' . t('(active tab)') . '</span>';

    // If the link does not contain HTML already, check_plain() it now.
    // After we set 'html'=TRUE the link will not be sanitized by l().
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }

    $link['localized_options']['html'] = TRUE;
    $link_text = t('!local-task-title!active', array('!local-task-title' => $link['title'], '!active' => $active));
  }

  return '<li' . (!empty($variables['element']['#active']) ? ' class="uk-active"' : '') . '>' . l($link_text, $link['href'], $link['localized_options']) . "</li>\n";
}

/**
 * Implements theme_menu_local_tasks().
 */
function uikit_menu_local_tasks($variables) {
  $output = '';

  $primary_attributes = $variables['primary_attributes'];
  $secondary_attributes = $variables['secondary_attributes'];

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="uk-hidden">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= "<ul" . backdrop_attributes($primary_attributes) . ">";
    $variables['primary']['#suffix'] = '</ul>';
    $output .= backdrop_render($variables['primary']);
  }

  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="uk-hidden">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= "<ul" . backdrop_attributes($secondary_attributes) . ">";
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= backdrop_render($variables['secondary']);
  }

  return $output;
}

/**
 * Implements theme_menu_tree().
 */
function uikit_menu_tree($variables) {
  if (empty($variables['attributes']['class'])) {
    $variables['attributes']['class'] = [];
  }
  
  if (!is_array($variables['attributes']['class'])) {
    $class = $variables['attributes']['class'];
    $variables['attributes']['class'] = [$class];
  }

  $variables['attributes']['class'][] = 'uk-navbar-nav';

  return '<ul ' . backdrop_attributes($variables['attributes']) . '>' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for the navigation menu.
 */
function uikit_menu_tree__navbar($variables) {
  if (empty($variables['attributes']['class'])) {
    $variables['attributes']['class'] = [];
  }

  if (!is_array($variables['attributes']['class'])) {
    $class = $variables['attributes']['class'];
    $variables['attributes']['class'] = [$class];
  }
  
  $variables['attributes']['class'][] = 'uk-navbar-nav';
  $variables['attributes']['class'][] = 'uk-visible@m';
  
  return '<ul ' . backdrop_attributes($variables['attributes']) . '>' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for the header block menu.
 */
function uikit_menu_tree__header_menu($variables) {
  if (empty($variables['attributes']['class'])) {
    $variables['attributes']['class'] = [];
  }

  if (!is_array($variables['attributes']['class'])) {
    $class = $variables['attributes']['class'];
    $variables['attributes']['class'] = [$class];
  }
  
  $variables['attributes']['class'][] = 'uk-navbar-nav';
  $variables['attributes']['class'][] = 'uk-visible@m';
  
  return '<ul ' . backdrop_attributes($variables['attributes']) . '>' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for the main menu.
 */
function uikit_menu_tree__main_menu($variables) {
  if (empty($variables['attributes']['class'])) {
    $variables['attributes']['class'] = [];
  }

  if (!is_array($variables['attributes']['class'])) {
    $class = $variables['attributes']['class'];
    $variables['attributes']['class'] = [$class];
  }

  $menu_position = theme_get_setting('main-menu_position');
  $variables['attributes']['class'][] = 'uk-navbar-nav';
  $variables['attributes']['class'][] = 'uk-visible@m';
  $variables['attributes']['class'][] = 'main-menu';
  if (!empty($menu_position)) {
    $variables['attributes']['class'][] = $menu_position;
  }
  
  return '<ul ' . backdrop_attributes($variables['attributes']) . '>' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for the user menu.
 */
function uikit_menu_tree__user_menu($variables) {
  if (empty($variables['attributes']['class'])) {
    $variables['attributes']['class'] = [];
  }

  if (!is_array($variables['attributes']['class'])) {
    $class = $variables['attributes']['class'];
    $variables['attributes']['class'] = [$class];
  }
  
  $variables['attributes']['class'][] = 'uk-navbar-nav';
  $variables['attributes']['class'][] = 'uk-visible@m';
  
  return '<ul ' . backdrop_attributes($variables['attributes']) . '>' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for offcanvas menus.
 */
function uikit_menu_tree__offcanvas($variables) {
  if (empty($variables['attributes']['class'])) {
    $variables['attributes']['class'] = [];
  }

  if (!is_array($variables['attributes']['class'])) {
    $class = $variables['attributes']['class'];
    $variables['attributes']['class'] = [$class];
  }
  
  $variables['attributes']['class'][] = 'uk-nav';
  $variables['attributes']['class'][] = 'uk-nav-default';
  $variables['attributes']['class'][] = 'uk-nav-parent-icon';
  $variables['attributes']['data-uk-nav'][] = '';
//  $variables['attributes']['class'][] = 'uk-navbar-nav';
//  $variables['attributes']['class'][] = 'uk-default';
//  $variables['attributes']['class'][] = 'uk-nav-parent-icon';
//  $variables['attributes']['data-uk-nav'] = '';
  
  return '<ul ' . backdrop_attributes($variables['attributes']) . '>' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for offcanvas menus.
 */
function uikit_menu_tree__offcanvas__below($variables) {
  return '<ul class="uk-nav-sub" role="menu">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for shortcut sets.
 */
function uikit_menu_tree__shortcut_set($variables) {
  return '<ul class="menu">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_node_add_list().
 */
function uikit_node_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="uk-description-list-line">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    $output .= '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }

  return $output;
}

/**
 * Implements theme_pager().
 */
function uikit_pager($variables) {
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = empty($variables['quantity']) ? 0 : (int) $variables['quantity'];
  global $pager_page_array, $pager_total;

  // Return if there is no pager to be rendered
  if (!isset($pager_page_array[$element]) || empty($pager_total)) {
    return '';
  }

  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // Current is the page we are currently paged to.
  $pager_current = $pager_page_array[$element] + 1;
  // First is the first page listed by this pager piece (re quantity).
  $pager_first = $pager_current - $pager_middle + 1;
  // Last is the last page listed by this pager piece (re quantity).
  $pager_last = $pager_current + $quantity - $pager_middle;
  // Max is the maximum page number.
  $pager_max = $pager_total[$element];

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }

  $li_first = theme('pager_first', array(
    'text' => (isset($tags[0]) ? $tags[0] : t('First')),
    'element' => $element,
    'parameters' => $parameters,
  ));

  $li_previous = theme('pager_previous', array(
    'text' => (isset($tags[1]) ? $tags[1] : t('Previous')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));

  $li_next = theme('pager_next', array(
    'text' => (isset($tags[3]) ? $tags[3] : t('Next')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));

  $li_last = theme('pager_last', array(
    'text' => (isset($tags[4]) ? $tags[4] : t('Last')),
    'element' => $element,
    'parameters' => $parameters,
  ));

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '…',
        );
      }

      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_previous', array(
              'text' => $i,
              'element' => $element,
              'interval' => ($pager_current - $i),
              'parameters' => $parameters,
            )),
          );
        }

        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('pager-current', 'uk-active'),
            'data' => '<span>' . $i . '</span>',
          );
        }

        if ($i > $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_next', array(
              'text' => $i,
              'element' => $element,
              'interval' => ($i - $pager_current),
              'parameters' => $parameters,
            )),
          );
        }
      }

      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '<span>…</span>',
        );
      }
    }

    if ($li_next) {
      $items[] = array(
        'class' => array('pager-next'),
        'data' => $li_next,
      );
    }

    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }

    $pagination = theme('item_list', array(
      'items' => $items,
      'attributes' => array('class' => array('uk-pagination')),
      'pagination' => TRUE,
    ));

    return '<h2 class="element-invisible">' . t('Pages') . '</h2>' . $pagination;
  }
}

/**
 * Implements theme_pager_link().
 */
function uikit_pager_link($variables) {
  $text = $variables['text'];
  $page_new = $variables['page_new'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $attributes = $variables['attributes'];

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = backdrop_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;

    if (!isset($titles)) {
      $titles = array(
        t('First') => t('Go to first page'),
        t('Previous') => t('Go to previous page'),
        t('Next') => t('Go to next page'),
        t('Last') => t('Go to last page'),
        t('« first') => t('Go to first page'),
        t('‹ previous') => t('Go to previous page'),
        t('next ›') => t('Go to next page'),
        t('last »') => t('Go to last page'),
      );
    }

    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    elseif (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  // @todo l() cannot be used here, since it adds an 'active' class based on the
  //   path only (which is always the current path for pager links). Apparently,
  //   none of the pager links is active at any time - but it should still be
  //   possible to use l() here.
  // @see http://drupal.org/node/1410574
  $attributes['href'] = url($_GET['q'], array('query' => $query));
  return '<a' . backdrop_attributes($attributes) . '>' . check_plain($text) . '</a>';
}

/**
 * Implements theme_password().
 */
function uikit_password($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'password';
  element_set_attributes($element, ['id', 'name', 'size', 'maxlength']);

  _form_set_class($element, [
    'form-text',
    'uk-input',
    'uk-form-width-large',
  ]);

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<input' . backdrop_attributes($element['#attributes']) . ' />';
}

/**
 * Implements theme_radio().
 */
function uikit_radio($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array(
    'id',
    'name',
    '#return_value' => 'value',
  ));

  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio', 'uk-radio'));

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<input' . backdrop_attributes($element['#attributes']) . ' />';
}

/**
 * Implements theme_select().
 */
function uikit_select($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));

  _form_set_class($element, array(
    'form-select',
    'uk-select',
  ));

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<select' . backdrop_attributes($element['#attributes']) . '>' . form_select_options($element) . '</select>';
}

/**
 * Implements theme_status_messages().
 */
function uikit_status_messages($variables) {
  $display = $variables['display'];
  $message_types = (empty($variables['messages']))? backdrop_get_messages($display) : $variables['messages'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
    'info' => t('Info message'),
  );

  foreach ($message_types as $type => $messages) {
    $class = 'uk-alert-';

    switch ($type) {
      case 'status':
        $class .= 'success';
        break;

      case 'error':
        $class .= 'danger';
        break;

      case 'warning':
        $class .= 'warning';
        break;

      case 'info':
        $class .= 'primary';
        break;
    }

    $output .= "<div id='messages' role='status' aria-label='" . t('Status messages') . "' class='uk-alert " . $class . "' data-uk-alert>\n";
    $output .= '<button class="uk-alert-close" type="button" data-uk-close></button>';

    if (!empty($status_heading[$type])) {
      $output .= '<h2 hidden>' . $status_heading[$type] . "</h2>\n";
    }

    if (count($messages) > 1) {
      $output .= " <ul>\n";

      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }

      $output .= " </ul>\n";
    }
    else {
      $output .= reset($messages);
    }

    $output .= "</div>\n";
  }

  return $output;
}

/**
 * Implements theme_table().
 */
function uikit_table(array $variables) {
  $header = (array) $variables['header'];
  $rows = (array) $variables['rows'];
  $attributes = $variables['attributes'];
  $caption = $variables['caption'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];

  $output = '';
  if (isset($caption)) {
    $output .= '<caption>' . $caption . "</caption>\n";
  }

  // Format the table columns:
  if (!empty($colgroups)) {
    foreach ($colgroups as $colgroup) {
      $colgroup_attributes = array();

      // Check if we're dealing with a simple or complex column.
      if (isset($colgroup['data'])) {
        foreach ($colgroup as $key => $value) {
          if ($key == 'data') {
            $cols = $value;
          }
          else {
            $colgroup_attributes[$key] = $value;
          }
        }
      }
      else {
        $cols = $colgroup;
      }

      // Build colgroup.
      if (is_array($cols) && count($cols)) {
        $output .= ' <colgroup' . backdrop_attributes($colgroup_attributes) . '>';
        foreach ($cols as $col) {
          $output .= ' <col' . backdrop_attributes($col) . ' />';
        }
        $output .= " </colgroup>\n";
      }
      else {
        $output .= ' <colgroup' . backdrop_attributes($colgroup_attributes) . " />\n";
      }
    }
  }

  // Add the 'empty' row message if available.
  if (empty($rows) && $empty) {
    $header_count = 0;
    if (!empty($header)) {
      foreach ($header as $header_cell) {
        if (is_array($header_cell)) {
          $header_count += isset($header_cell['colspan']) ? $header_cell['colspan'] : 1;
        }
        else {
          $header_count++;
        }
      }
    }

    $rows[] = array(array('data' => $empty, 'colspan' => $header_count, 'class' => array('empty', 'message')));
  }

  $responsive_columns = array();
  // Format the table header.
  if (!empty($header)) {
    $ts = tablesort_init($header);
    // HTML requires that the thead tag has tr tags in it followed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (!empty($rows) ? ' <thead><tr>' : ' <tr>');
    $i = 0;
    foreach ($header as $cell) {
      $i++;
      // Track responsive classes for each column as needed. Only the header
      // cells for a column are marked up with the responsive classes by a
      // module developer or themer. The responsive classes on the header cells
      // must be transferred to the content cells.
      if (!empty($cell['class']) && is_array($cell['class'])) {
        if (in_array(RESPONSIVE_PRIORITY_MEDIUM, $cell['class'])) {
          $responsive_columns[$i] =  RESPONSIVE_PRIORITY_MEDIUM;
        }
        elseif (in_array(RESPONSIVE_PRIORITY_LOW, $cell['class'])) {
          $responsive_columns[$i] =  RESPONSIVE_PRIORITY_LOW;
        }
      }
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }
    // Using ternary operator to close the tags, based on whether or not there
    // are rows.
    $output .= (!empty($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }

  // Format the table and footer rows.
  $sections = array();

  if (!empty($rows)) {
    $sections['tbody'] = $rows;
  }

  if (!empty($variables['footer'])) {
    $sections['tfoot'] = $variables['footer'];
  }

  // The tbody and tfoot HTML tags have the same structure, and are built
  // using the same procedure.
  foreach ($sections as $tag => $content) {
    $output .= "<$tag>\n";
    $flip = array('even' => 'odd', 'odd' => 'even');
    $class = 'even';
    $default_no_striping = ($tag === 'tfoot');

    foreach ($content as $number => $row) {
      // Check if we're dealing with a simple or complex row.

      if (isset($row['data'])) {
        $cells = $row['data'];
        $no_striping = isset($row['no_striping']) ? $row['no_striping'] : $default_no_striping;

        // Set the attributes array and exclude 'data' and 'no_striping'.
        $tr_attributes = $row;
        unset($tr_attributes['data']);
        unset($tr_attributes['no_striping']);
      }
      else {
        $cells = $row;
        $tr_attributes = array();
        $no_striping = $default_no_striping;
      }

      if (!empty($cells)) {
        // Add odd/even class.
        if (!$no_striping) {
          $class = $flip[$class];
          $tr_attributes['class'][] = $class;
        }

        // Build row.
        $output .= ' <tr' . backdrop_attributes($tr_attributes) . '>';
        $i = 0;
        foreach ($cells as $cell) {
          // Add active class if needed for sortable tables.
          $cell = tablesort_cell($cell, $header, $ts, $i);
          $i++;
          // Copy RESPONSIVE_PRIORITY_LOW/RESPONSIVE_PRIORITY_MEDIUM
          // class from header to cell as needed.
          if (isset($responsive_columns[$i])) {
            if (is_array($cell)) {
              $cell['class'][] = $responsive_columns[$i];
            }
            else {
              $cell = array('data' => $cell, 'class' => array($responsive_columns[$i]));
            }
          }
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }

    $output .= "</$tag>\n";
  }

  // Add sticky headers if enabled.
  if (!empty($header) && $sticky) {
    backdrop_add_js('core/misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'sticky-enabled';
  }
  // If the responsive classes were used, add the matching library.
  if (count($responsive_columns)) {
    backdrop_add_library('system', 'backdrop.tableresponsive');
    // Add 'responsive-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'responsive-enabled';
    $attributes['class'][] = 'uk-table-responsive';
  }

  $output = '<table' . backdrop_attributes($attributes) . ">\n$output</table>\n";
  $output = "<div class='uk-overflow-auto'>\n$output</div>\n";

  return $output;
}

function uikit_text_format_wrapper($variables) {
  $element = $variables['element'];
  $output = '<div class="uk-section uk-section-default uk-padding">';
  $output .= $element['#children'];
  if (!empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . '</div>';
  }
  $output .= "</div>\n";

  return $output;
}


function uikit_filter_guidelines($variables) {
  $format = $variables['format'];
  $attributes['class'][] = 'filter-guidelines-item';
  $attributes['class'][] = 'filter-guidelines-' . $format->format;
  $output = '<div' . backdrop_attributes($attributes) . '>';
  $output .= '<h6 class="uk-text-uppercase uk-margin-remove-bottom uk-margin-top">' . check_plain($format->name) . '</h6>';
  $output .= theme('filter_tips', array('tips' => _filter_tips($format->format, FALSE)));
  $output .= '</div>';
  return $output;
}

function uikit_webform_email($variables) {
  $element = $variables['element'];  
  $variables['element']['#attributes']['class'][] = 'uk-input';
//  $variables['element']['#attributes']['class'][] = 'uk-form-width-large';
  if (form_get_error($element)) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
  return theme_webform_email($variables);
}

function uikit_webform_number($variables) {
  $element = $variables['element'];
  $variables['element']['#attributes']['class'][] = 'uk-input';
//  $variables['element']['#attributes']['class'][] = 'uk-form-width-large';
  if (form_get_error($element)) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
  return theme_webform_number($variables);
}

/**
 * Implements theme_email().
 */
function uikit_email($variables) {
  $element = $variables['element'];
  $variables['element']['#attributes']['class'][] = 'uk-input';
//  $variables['element']['#attributes']['class'][] = 'uk-form-width-large';
  if (form_get_error($element)) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
  return theme_email($variables);
}

/**
 * Implements theme_tel().
 */
function uikit_tel($variables) {
  $element = $variables['element'];
  $variables['element']['#attributes']['class'][] = 'uk-input';
//  $variables['element']['#attributes']['class'][] = 'uk-form-width-large';
  if (form_get_error($element)) {
    $variables['element']['#attributes']['class'][] = 'uk-form-danger';
  }
  return theme_tel($variables);
}

/**
 * Implements theme_textarea().
 */
function uikit_textarea($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  _form_set_class($element, array('form-textarea', 'uk-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    backdrop_add_library('system', 'backdrop.textarea');
    $wrapper_attributes['class'][] = 'resizable';
  }

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  $output = '<div' . backdrop_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . backdrop_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

/**
 * Implements theme_textfield().
 */
function uikit_textfield($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'text';
  element_set_attributes($element, [
    'id',
    'name',
    'value',
    'size',
    'maxlength',
  ]);

  _form_set_class($element, [
    'form-text',
    'uk-input',
    'uk-form-width-large',
  ]);

  $extra = '';

  if ($element['#autocomplete_path'] && !empty($element['#autocomplete_input'])) {
    backdrop_add_library('system', 'backdrop.autocomplete');
    $element['#attributes']['class'][] = 'form-autocomplete';

    $autocomplete = [
      '#type' => 'hidden',
      '#value' => $element['#autocomplete_input']['#url_value'],
      '#attributes' => [
        'class' => ['autocomplete'],
        'id' => $element['#autocomplete_input']['#id'],
        'disabled' => 'disabled',
      ],
    ];
    $extra = backdrop_render($autocomplete);
  }

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  $output = '<input' . backdrop_attributes($element['#attributes']) . ' />';

  return $output . $extra;
}

/**
 * Returns rendered HTML for a menu toggle.
 *
 * @param $variables
 *   An associative array containing:
 *     - enabled: A boolean indicating whether the menu toggle should be shown.
 *     - id: Unique identifier, generated with backdrop_html_id().
 *     - text: Translated text for the menu toggle button.
 *
 * @see theme_menu_toggle()
 * @ingroup themeable
 */
function uikit_menu_toggle($variables) {
  $output = '';
  if ($variables['enabled']) {
    $id = $variables['id'];
    $output .= '<a href="#offcanvas" class="uk-navbar-toggle uk-hidden@m" data-uk-toggle aria-expanded="false">';
    $output .= '  <span data-uk-navbar-toggle-icon class="uk-icon uk-navbar-toggle-icon"></span>';
    if (!empty($variables['text'])) {
      $output .= '  <span class="uk-margin-small-left">' . check_plain($variables['text']) .'</span>';
    }
    $output .= '</a>';
  }
  return $output;
}


/**
 * Replacement for theme_form_element().
 */
function uikit_webform_element($variables) {
  $element = $variables['element'];

  $output = '<div ' . backdrop_attributes($element['#wrapper_attributes']) . '>' . "\n";
  $prefix = isset($element['#field_prefix']) ? '<span class="uk-input-prefix">' . webform_filter_xss($element['#field_prefix']) . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="uk-input-suffix">' . webform_filter_xss($element['#field_suffix']) . '</span>' : '';

  // Generate description for above or below the field.
  $above = !empty($element['#webform_component']['extra']['description_above']);
  $description = array(
    FALSE => '',
    TRUE => !empty($element['#description']) ? ' <div class="description">' . $element['#description'] . "</div>\n" : '',
  );

  // If #children does not contain an element with a matching @id, do not
  // include @for in the label.
  if (isset($variables['element']['#id']) && strpos($element['#children'], ' id="' . $variables['element']['#id'] . '"') === FALSE) {
    $variables['element']['#id'] = NULL;
  }

  // Determine whether or not this element has form control children. If so and
  // if webform_fieldset_wrap is TRUE, wrap them in a fieldset and use legend
  // instead of label.
  $has_element_children = FALSE;
  if (webform_variable_get('webform_fieldset_wrap')) {
    foreach (array_keys($element) as $key) {
      if (substr($key, 0, 1) !== '#') {
        $has_element_children = TRUE;
        break;
      }
    }
  }

  if ($has_element_children) {
    $output .= '<fieldset class="fieldset-invisible">';
  }

  switch ($element['#title_display']) {
    case 'inline':
      $output .= $description[$above];
      $description[$above] = '';
    case 'before':
    case 'invisible':
    case 'after':
      if ($has_element_children) {
        $title = '<legend>' . $element['#title'];

        if ($element['#required']) {
          $title .= ' ' . theme('form_required_marker', $variables);
        }

        $title .= '</legend>';
      }
      else {
        $title = ' ' . theme('form_element_label', $variables);
      }
      break;
  }

  $wrapper_start = $wrapper_end = '';
  if (!empty($prefix) || !empty($suffix)) {
    $wrapper_start = '<div class="uk-input-group">';
    $wrapper_end = '</div>';
  }

  $children = ' ' . $description[$above] . $wrapper_start . $prefix . $element['#children'] . $suffix . $wrapper_end;
  switch ($element['#title_display']) {
    case 'inline':
    case 'before':
    case 'invisible':
      $output .= $title;
      $output .= $children;
      break;

    case 'after':
      $output .= $children;
      $output .= $title;
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= $children;
      break;
  }
  $output .= "\n";

  $output .= $description[!$above];

  if ($has_element_children) {
    $output .= '</fieldset>';
  }

  $output .= "</div>\n";

  return $output;
}


/**
 * Implements theme_date().
 */
function uikit_date($variables) {
  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'uk-grid';
  $attributes['class'][] = 'uk-grid-small';

  return '<div' . backdrop_attributes($attributes) . '>' . backdrop_render_children($element) . '</div>';
}

/**
 * Implements theme_html_date().
 */
function uikit_html_date($variables) {
  $element = $variables['element'];
  $attributes = array();
  if (isset($element['#attributes'])) {
    $attributes = $element['#attributes'];
  }
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  $attributes['class'][] = 'form-date';
  $attributes['type'] = 'date';
  $attributes['value'] = $element['#value'];
  $attributes['name'] = $element['#name'];

  $output = '<input ' . backdrop_attributes($attributes) . ' />';
  return $output;
}



/**
 * Implements theme_html_datetime().
 */
function uikit_html_datetime($variables) {
  $element = $variables['element'];
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'uk-grid';
  $attributes['class'][] = 'uk-grid-small';
  return '<div' . backdrop_attributes($attributes) . '>' . backdrop_render_children($element) . '</div>';
}

/**
 * Implement theme_webform_date().
 */
function uikit_webform_date($variables) {
  $element = $variables['element'];

  $element['year']['#attributes']['class'][] = 'year';
  $element['month']['#attributes']['class'][] = 'month';
  $element['day']['#attributes']['class'][] = 'day';

  // Add error classes to all items within the element.
  if (form_get_error($element)) {
    $element['year']['#attributes']['class'][] = 'uk-form-danger';
    $element['month']['#attributes']['class'][] = 'uk-form-danger';
    $element['day']['#attributes']['class'][] = 'uk-form-danger';
  }

  // Add HTML5 required attribute, if needed.
  if ($element['#required']) {
    $element['year']['#attributes']['required'] = 'required';
    $element['month']['#attributes']['required'] = 'required';
    $element['day']['#attributes']['required'] = 'required';
  }

  $class = array('webform-container-inline', 'uk-grid', 'uk-grid-small');

  // Add the JavaScript calendar if available (provided by Date module package).
  if (!empty($element['#datepicker'])) {
    $class[] = 'webform-datepicker';
    $calendar_class = array('webform-calendar');
    if ($element['#start_date']) {
      $calendar_class[] = 'webform-calendar-start-' . $element['#start_date'];
    }
    if ($element['#end_date']) {
      $calendar_class[] = 'webform-calendar-end-' . $element['#end_date'];
    }
    $calendar_class[] = 'webform-calendar-day-' . config_get('system.date', 'first_day');

    $calendar = theme('webform_calendar', array('component' => $element['#webform_component'], 'calendar_classes' => $calendar_class));
  }

  $output = '';
  $output .= '<div class="' . implode(' ', $class) . '">';
  $output .= backdrop_render_children($element);
  $output .= isset($calendar) ? $calendar : '';
  $output .= '</div>';

  return $output;
}
