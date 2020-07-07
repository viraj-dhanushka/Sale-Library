Mailjet for Drupal 7.x module
===========================

Mailjet APIv3 module for Drupal

This module for Drupal 7.x. provides complete control of Drupal Email 
settings with Mailjet and also adds specific Drupal Commerce email marketing 
functionality such as triggered marketing emails and marketing campaign revenue statistics.

The Mailjet Module for Drupal 7.x configures your default Drupal SMTP 
settings to use Mailjet's SMTP relay with enhanced deliverability and 
tracking. The module also provides the ability to synchronise your Drupal 
opt-in contacts and send bulk and targeted emails to them with real time 
statistics including opens, clicks, geography, average time to click, unsubs, etc. 

Mailjet is a powerful all-in-one email service provider used to get maximum 
insight and deliverability results from both  marketing and transactional 
emails. Our analytics tools and intelligent APIs give senders the best 
understanding of how to maximize benefits for each individual contact and 
campaign email after email. 

Requirements
------------
  * Views (https://www.drupal.org/project/views)
  * Views Bulk Operations (https://www.drupal.org/project/views_bulk_operations)
  * Entity (https://www.drupal.org/project/entity)
  * Chaos tool suite (ctools) (https://www.drupal.org/project/ctools)

Recommended modules
-------------------
  The following modules are not strictly required but it is nice to install them to get the full capability of the Mailjet features.
  * commerce (https://www.drupal.org/project/commerce)
  For the stats sub module to enable the ROI feature install:
  * views_date_format_sql (https://www.drupal.org/project/views_date_format_sql)
  * charts (https://www.drupal.org/project/charts)
  For the list module you need to install:
  * viewfield (https://www.drupal.org/project/viewfield)


Prerequisites
-------------

The Mailjet plugin relies on the PHPMailer v5.2.21 for sending emails.

To install PHPMailer via composer use:

```
composer require phpmailer/phpmailer=~5.2
```

To install PHPMailer manually:
1) Get the PHPMailer v5.2.22 from GitHub here:
http://github.com/PHPMailer/PHPMailer/archive/v5.2.22.zip
2) Extract the archive and rename the folder "PHPMailer-5.2.22" to "phpmailer".
3) Upload the "phpmailer" folder to your server inside
DRUPAL_ROOT/sites/all/libraries/.
4) Verify that the file class.phpmailer.php is correctly located at this path: DRUPAL_ROOT/sites/all/libraries/phpmailer/class.phpmailer.php
* Note: Libraries API can be used to move the library to an alternative location, if needed, e.g. for use in a distribution.

Installation
------------

1. Download the latest release from https://www.drupal.org/project/mailjet.
2. Upload the module in your Drupal sites/all/modules/ directory.
3. Log in as administrator in Drupal.
4. Enable the Mailjet settings module on the Administration > Modules page.
5. Fill in required settings on the Administration > Configuration > Mailjet
 settings page.
6. You will be required to enter API key and Secret Key from your Mailjet account. If you do not have an account yet, please [create one](https://app.mailjet.com/signup?aff=drupalmj). 

Configuration
-------------

1. The site can be set up to use the Mailjet module as an email gateway, this can be easily configured, by clicking on the Settings tab => your_site/admin/config/system/mailjet, and then selecting the checkbox on the top, "Send emails through Mailjet", click "Save Settings" button on the bottom of the page. You can test that feature by sending a test email, just click the button on the top of the page "Send test email" in Settings tab.
2. If you want to enable the Campaign feature, you should enable the mailjet_campaign module, you can do that from Administration > Site building > Modules page (your_site/admin/modules)
3. Enabling the campaign sub module will create additional menu item in your administration menu, the new menu is called "Campaign" (your_site/admin/mailjet/campaign). 
4. Clicking this menu item will display all the campaigns created by the administrator, from this point you will be able to create new campaigns as well, the same way you do that on app.mailjet.com.
5. If you want to create a campaign simply go to the Campaigns page => your_site/admin/mailjet/campaign
6. If you enable the stats module 2 menu items will appear Dashboard where you can see the results of the mail campaigns and the Mailjet ROI stats, clicking the ROI stats you can see the actual conversion of your campaigns. This feature will display a view which will present the campaign name, number of orders made by users who clicked on the link of your site in your email campaign.
7. My account menu item will redirect you to the Mailjet logging page.
8. Upgrade menu link will redirect you to the pricing list of Mailjet where you can pick up a plan and upgrade your account.
9. The contacts menu item allows you to create lists that can be used for your campaigns. New lists can be created and contacts can be  import  in several ways: Upload from CSV, Copy/Paste from Excel, Copy/Paste from TXT, CSV, RTF.
10. If you want to enable the trigger_examples sub-module you need to enable the views_bulk_operations module and apply the following patch to it: https://www.drupal.org/files/issues/views-vbo-patch-anon-users.patch
      
Author
------
Mailjet SAS
plugins@mailjet.com

Changelog
---------

#### 7.x-2.22 13 September 2019
* Fix Drupal account custom fields sync to Mailjet

#### 7.x-2.21 27 August 2019
* Add possibility to sync custom profile fields along with the user's `name` either on initial and single contact sync

#### 7.x-2.20 22 August 2019
* Bugfixes and internal improvements regarding single contact sync

#### 7.x-2.19 18 February 2019
* Bugfixes and internal improvements regarding contacts sync, subscription and unsibscription of contacts
* Updated texts and translations

#### 7.x-2.18 6 June 2018
* Fix issue when Stats module enabled and Campaign module disabled
* Fix "Send emails through Mailjet" unchecked overrides non-default Mail system
* Fix redirecting regular users to the Mailjet Settings
* Add config setting for user property syncing
* Fix subscription form issue
* Fix an issue if campaings module is not installed

#### 7.x-2.17 17 April 2018
* Update the tracking parameter

#### 7.x-2.16 5 December 2017
* New feature: added creation of a subscription block
* Various bugfixes and improvements

#### 7.x-2.15 16 October 2017
* Bug fix: The callback parameter is temporarily removed

#### 7.x-2.14 1 June 2017
* Bug fix: Messages improved
* Bug fix: Validate add domain field
* Bug fix: Unable to synchronize a new user if the Drupal contact list already exists

#### 7.x-2.13 6 Mar 2017
* Bug fix: Unable to send more than 1 mail per request when using Libraries API in a non default directory
https://www.drupal.org/node/2853496

#### 7.x-2.12 13 Feb 2017
* Feature: Support autoload for PHPMailer
https://www.drupal.org/node/2850791
* Bug fix: Fatal error with plugin
https://www.drupal.org/node/2753095
* Bug fix: Error when user profile is associated with entity_reference field
https://www.drupal.org/node/2491389

#### 7.x-2.11 7 Feb 2017
* Bug fix: Module breaks site login
https://www.drupal.org/node/2491395
https://www.drupal.org/node/2598060
* Bug fix: Call to undefined function mailjet_campaign_access()
https://www.drupal.org/node/2597309
* Feature: Add ability to disable user information syncing
https://www.drupal.org/node/2663328

#### 7.x-2.10 25 Jan 2017
* Bug fix: https://www.drupal.org/node/2842760

#### 7.x-2.9 11 Jan 2017
* Feature: update PHPmailer library
https://www.drupal.org/node/2842760

#### 7.x-2.7 25 Oct 2016
* Bug fixes

#### 7.x-2.6 12 Feb 2016
* Mailjet event URL is now public
Fixed URL path to avoid Drupal redirect

#### 7.x-2.5 8 Oct 2015
* Added tracking information

#### 7.x-2.4 3 Aug 2015
* Added iFrame parameter to show/hide sending policy

#### 7.x-2.3 8 May 2015
* Bug fixes

#### 7.x-2.2 8 Apr 2015
* Bug fix: not able to display the trusted domain form
https://www.drupal.org/node/2456715

#### 7.x-2.1 24 Mar 2015
#### 7.x-2.0 17 Mar 2015
* First release of the new version of the Mailjet module
