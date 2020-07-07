<?php

/**
 * Implements hook_page_build().
 */
function hook_page_build(&$page) {

  // Load the scripts on a specific url or other case when you don't use the 
  // Field API or Block.
  if (current_path() == 'node/1') {
    $manager = AddThisScriptManager::getInstance();

    // Adjust domready and async settings for this page.
    //
    // NOTE! that on pages where these settings are altered and attach is called
    // more then once but not altered he second time, you will get conflicts.
    // The widget js will be loaded multiple times with different settings.
    // So choose wisely!
    //
    $manager->setAsync(FALSE);
    $manager->setDomReady(FALSE);

    // Using $page['content'] or other area is neccesary because otherwise
    // #attached does not work. You can not attach on $page directly.
    $manager->attachJsToElement($page['content']);
  }
}
