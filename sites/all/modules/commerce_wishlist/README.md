This module adds Wish list support to Drupal Commerce.

It provides a basic upgrade path from the 1.x or 2.x branches but nothing
it guaranteed if you've made any changes. The 3.x branch is a essentially
a complete rewrite.

Wish lists are implemented as orders. A user can have multiple wish lists by
creating multiple orders with the state/status of "wishlist" but a user only
gets one out of the box.

Users have a "Wish list" tab on their account that shows the contents of their
wish list. They can add an item from the wish list to the cart by clicking on
the "Add to Cart" button.

You can choose a 'button' or a ajaxified 'link' option for adding items to
a Wish list.

Views that normally filter out shopping carts will be altered to also filter
out Wish lists. This includes the customer's order tab and the admin UI.

This module also adds a "Wish lists" tab to the admin order management UI.

There are 4 events that this module provides which haven't been fully tested.

A test suite is included that tests some of the basic functionality and will
test blocks, rules, etc. as time allows.

If you want to support multiple wishlists, you'll have to create your own
views and create your own menu callbacks, but this should be fairly easy
since most of the functions you need support specifying a wish list.
