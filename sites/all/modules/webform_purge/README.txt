CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 
 
INTRODUCTION
------------

The Webform Purge allows you to set up automated purging of Webform submissions
on a daily rolling schedule. You select the number of days to retain and the 
module uses hook_cron to purge them during cron runs.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/webform_purge

 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/webform_purge
   
   
REQUIREMENTS
------------

This module requires the following modules:

 * Webform (https://www.drupal.org/project/webform)
 
 
INSTALLATION
------------
 
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

 * The installation creates configuration variables in the drupal core
   'variable' table.
   
   
CONFIGURATION
-------------
 
 * Configure settings in Administration » Configuration » Webform Purge:

   - Enable automated purging

     This option enables the functionality of the module. If not checked, 
	 the purge function will not run.

   - Run only once per day

     This option is used to limit the purge to just one cron run per day.
	 Conversely if unchecked, it will run with every cron run. 

   - Days to retain

     The number of days to retain submissions before they are purged.

    
   
TROUBLESHOOTING
---------------

 * If expected entries are not purged, check the following:

   - Is the Enable automated purging checkbox checked?
   
   - Is the submission younger than the Days to retain?

FAQ
---

Q: If I want to incrementally purge starting with a very old date and progress
   to my target date, do I have to run this across multiple days?	

A: No, you can uncheck the 'Run only once per day' option and then set a 
   'Days to retain' at a high value and then run the cron job.  Repeat this
   until you achieve your target number of days to retain.
   
Q: How can I tell how many entries were purged?

A: Check the 'Recent log messages' under the Reports tab for an informational
   entry from the webform_purge module.
