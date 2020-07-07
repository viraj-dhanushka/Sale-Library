<?php

/**
 * @file
 * Template file for sharing a wishlist URL.
 *
 * Available variables:
 *   $account - The user account that the wishlist belongs to.
 */

?>
<div class="commerce-wishlist-share">
  <p class="commerce-wishlist-share-description">
    <?php print t('Share your wishlist with your friends and family with the link below!'); ?>
  </p>

  <p class="commerce-wishlist-share-url">
    <?php print l(t("!user's wish list", array('!user' => format_username($account))), url('user/' . $account->uid . '/wishlist', array('absolute' => TRUE))); ?>
  </p>
</div>
