
"Import Commerce entities (e.g. products) using Feeds"

Features
========

- Product processor for Feeds that creates product entities in Drupal.
- Commerce price mapper supporting the creation of price amounts and
  currency codes from raw input

Help using Feeds
================

- The "site builder's guide to Feeds" http://drupal.org/node/622698

Installation
============

- Install & enable the Feeds (including UI) and Commerce modules (including
  product, price and their UI modules)
- Go to admin/structure/feeds and add a new importer
- Select the "Commerce Product processor"
- In Settings, select a product type to use to create new product entities
- In Mapping, select how raw input fields map on fields of the product entity.
  Select at least Product SKU (make unique) and Product Title.


Example configuration and feature
=================================

The commerce_feeds_example feature is included to quickly show 
- how a comma-separated file can be used to import product entities.
- how the same comma-separated file can be used to import product display nodes
  linking to product entities via their SKU.

1. Import product entities.
- You will probably want to start with the Commerce Kickstart profile,
  http://drupal.org/project/commerce_kickstart, but that's not absolutely
  necessary.
- Install the feature module "commerce_feeds_example"
- Go to /import and click 'Product importer'
- Upload the file 'example_products.csv' and click Import
- Go to /admin/commerce/products to see the imported products

2. Import product display nodes.
Repeat the same steps but chose 'Product reference importer' and select the same 
file 'example_products.csv'.

Note that there are larger feeds available.
http://d7.randyfay.com/books/feed has a 100-product feed.
http://d7.randyfay.com/books/fullfeed has a 1200-product feed.

There is a tutorial and screencast to go with this feature at
http://www.drupalcommerce.org/node/467

Contact
=======

Maintainer: Peter Vanhee (pvhee) - http://drupal.org/user/108811
Contact me for custom implementations.