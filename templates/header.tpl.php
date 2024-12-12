<?php
/**
 * @file
 * Display generic site information such as logo, site name, etc.
 *
 * Available variables:
 *
 * - $base_path: The base path of the Backdrop installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the home page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $front_page: The URL of the home page. Use this instead of $base_path, when
 *   linking to the home page. This includes the language domain or prefix.
 * - $site_name: The name of the site, empty when display has been disabled.
 * - $site_slogan: The site slogan, empty when display has been disabled.
 * - $menu: The menu for the header (if any), as an HTML string.
 */
?>
<div class="uk-grid uk-flex uk-flex-middle">
  <?php if ($logo): ?>
    <div class="uk-width-auto">
      <a href="<?php print $front_page; ?>" id="site-logo" class="uk-responsive" title="<?php print t('Home'); ?>" rel="home">
        <img class="uk-margin-small-right" src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    </div>
  <?php endif; ?>
  <?php if ($site_name): ?>
    <div class="uk-width-auto">
      <a href="<?php print $front_page; ?>" class="uk-logo uk-text-nowrap" title="<?php print t('Home'); ?>" rel="home">
        <?php print $site_name; ?>
      </a>
    </div>
  <?php endif; ?>
  <?php if ($site_slogan): ?>
    <div class="uk-width-auto@m">
      <div class="xsite-slogan"><?php print $site_slogan; ?></div>
    </div>
  <?php endif; ?>
  <?php if ($menu): ?>
    <div class="uk-width-expand">
      <nav class="uk-navbar-container uk-navbar-transparent" data-uk-navbar>
        <div class="<?php echo $menu_header_position; ?>">
          <div class="uk-visible@m">
            <?php print render($menu); ?>
          </div>
          <?php if ($menu_header_toggle): ?>
            <a href="#offcanvas" uk-toggle uk-navbar-toggle-icon class="uk-navbar-toggle uk-hidden@m uk-navbar-toggle-icon uk-icon"></a>
          <?php endif; ?>
        </div>
      </nav>
    </div>
  <?php endif; ?>
</div>

<?php /* print render($header_menu); */ ?>
<!--
<nav class="uk-navbar-container <?php echo $menu_header_transparent; ?>" style="">
  <div data-uk-navbar>
    <?php if ($logo || $site_name || $site_slogan): ?>
      <div class="uk-navbar-left">
        <?php if ($logo || $site_name): ?>
          <div class="uk-navbar-item">
            <a href="<?php print $front_page; ?>" id="site-logo" class="uk-navbar-item uk-logo" title="<?php print t('Home'); ?>" rel="home">
              <img class="uk-margin-small-right" src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
              <span><?php print $site_name; ?></span>
            </a>
          </div>
        <?php endif; ?>
        <?php if ($site_slogan): ?>
          <div class="uk-navbar-item site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div>
    <?php endif; ?>
    <?php if ($menu): ?>
    <div class="uk-navbar-right">
        <?php print render($menu); ?>
    </div>
    <?php endif; ?>
  </div>
</nav>
-->
