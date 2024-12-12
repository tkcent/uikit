<?php
/**
 * @file
 * Template for the Moscone Flipped layout.
 *
 * Variables:
 * - $title: The page title, for use in the actual HTML content.
 * - $messages: Status and error messages. Should be displayed prominently.
 * - $tabs: Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node.)
 * - $action_links: Array of actions local to the page, such as 'Add menu' on
 *   the menu administration interface.
 * - $classes: Array of CSS classes to be added to the layout wrapper.
 * - $attributes: Array of additional HTML attributes to be added to the layout
 *     wrapper. Flatten using backdrop_attributes().
 * - $content: An array of content, each item in the array is keyed to one
 *   region of the layout. This layout supports the following sections:
 *   - $content['header']
 *   - $content['top']
 *   - $content['content']
 *   - $content['sidebar']
 *   - $content['bottom']
 *   - $content['footer']
 */
?>
<div class="layout--moscone-flipped <?php print implode(' ', $classes); ?>"<?php print backdrop_attributes($attributes); ?>>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>

  <?php if (!empty($content['header'])): ?>
    <?php if (!empty($uikit['header'])): ?>
      <header <?php print backdrop_attributes($uikit['header']); ?> role="banner" aria-label="<?php print t('Site header'); ?>">
    <?php endif; ?>
    <?php print $content['header']; ?>
    <?php if (!empty($uikit['header'])): ?>
      </header>
    <?php endif; ?>
  <?php endif; ?>

  <div <?php print backdrop_attributes($uikit['main-content-wrapper']); ?>>

    <?php if ($messages): ?>
      <div class="l-messages" role="status" aria-label="<?php print t('Status messages'); ?>">
        <?php print $messages; ?>
      </div>
    <?php endif; ?>

    <?php if (!empty($title)): ?>
      <div class="l-page-title">
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <h1 class="page-title"><?php print $title; ?></h1>
        <?php print render($title_suffix); ?>
      </div>
    <?php endif; ?>

    <?php if ($tabs): ?>
      <nav class="tabs" role="tablist" aria-label="<?php print t('Admin content navigation tabs.'); ?>">
        <?php print $tabs; ?>
      </nav>
    <?php endif; ?>

    <?php print $action_links; ?>

    <?php if (!empty($content['top'])): ?>
      <?php if (!empty($uikit['top'])): ?>
        <div <?php print backdrop_attributes($uikit['top']); ?>>
      <?php endif; ?>
      <?php print $content['top']; ?>
      <?php if (!empty($uikit['top'])): ?>
        </div>
      <?php endif; ?>
    <?php endif; ?>

    <div class="l-middle uk-grid uk-grid-match uk-grid-collapse">
      <main <?php print backdrop_attributes($uikit['content']); ?> role="main" aria-label="<?php print t('Main content'); ?>">
        <?php print $content['content']; ?>
      </main>
      <aside <?php print backdrop_attributes($uikit['sidebar']); ?>>
        <?php print $content['sidebar']; ?>
      </aside>
    </div><!-- /.l-middle -->

    <?php if (!empty($content['bottom'])): ?>
      <?php if (!empty($uikit['bottom'])): ?>
        <div <?php print backdrop_attributes($uikit['bottom']); ?>>
      <?php endif; ?>
      <?php print $content['bottom']; ?>
      <?php if (!empty($uikit['bottom'])): ?>
        </div>
      <?php endif; ?>
    <?php endif; ?>

  </div><!-- /.l-wrapper -->

  <?php if (!empty($content['footer'])): ?>
    <?php if (!empty($uikit['footer'])): ?>
      <footer <?php print backdrop_attributes($uikit['footer']); ?>>
    <?php endif; ?>
    <?php print $content['footer']; ?>
    <?php if (!empty($uikit['footer'])): ?>
      </footer>
    <?php endif; ?>
  <?php endif; ?>

</div><!-- /.layout--moscone-flipped -->
