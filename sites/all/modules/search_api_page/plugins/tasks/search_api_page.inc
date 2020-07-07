<?php

/**
 * @file
 * Handles the 'search api page' override task.
 *
 * This plugin overrides [search_api_page_url] and reroutes it to the page
 * manager, where a list of tasks can be used to service this request based upon
 * criteria supplied by access plugins.
 */

/**
 * Implements a specialized version of hook_page_manager_task_tasks().
 *
 * See api-task.html for more information.
 */
function search_api_page_search_api_page_page_manager_tasks() {
  return array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',
    'title' => t('Search API Page'),

    // There are multiple search pages, let's override each of them
    // separately.
    'subtasks' => TRUE,
    'subtask callback' => 'search_api_page_search_api_page_subtask',
    'subtasks callback' => 'search_api_page_search_api_page_subtasks',

    // Menu hooks so that we can alter the node/%node menu entry to point to us.
    'hook menu alter' => 'search_api_page_search_api_page_menu_alter',

    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',
    'get arguments' => 'search_api_page_search_api_page_get_arguments',
    'get context placeholders' => 'search_api_page_search_api_page_get_contexts',
    'access callback' => 'search_api_page_search_api_page_access_check',
  );
}

/**
 * Alters available menu items based on a defined task.
 *
 * Used as a "hook menu alter" callback in
 * search_api_page_search_api_page_page_manager_tasks().
 */
function search_api_page_search_api_page_menu_alter(&$items, $task) {
  // Go through each search page.
  foreach (search_api_page_load_multiple(FALSE) as $page => $info) {
    if (variable_get('search_api_page_search_api_page_disabled_' . $info->machine_name, TRUE)) {
      continue;
    }

    $path = $info->path;
    // If the page was deleted in the meantime, the menu entry might not exist
    // anymore.
    if (empty($items[$path]['page callback'])) {
      continue;
    }
    $callback = $items[$path]['page callback'];

    if ($callback == 'search_api_page_view' || variable_get('page_manager_override_anyway', FALSE)) {
      $items["$path"]['page callback'] = 'search_api_page_search_api_page_page';
      $items["$path"]['file path'] = $task['path'];
      $items["$path"]['file'] = $task['file'];
    }
    else {
      // Automatically disable this task if it cannot be enabled.
      variable_set('search_api_page_search_api_page_disabled_' . $page, TRUE);
      if (!empty($GLOBALS['search_api_page_enabling_search_api_page'])) {
        drupal_set_message(t('Page manager module is unable to enable @path because some other module already has overridden with %callback.', array('%callback' => $callback, '@path' => $path)), 'error');
      }
    }
  }
}

/**
 * Page callback: Displays a search page as a page manager task.
 */
function search_api_page_search_api_page_page($type) {
  ctools_include('menu');

  // Get the arguments and construct a keys string out of them.
  $args = func_get_args();

  // We have to remove the $type.
  array_shift($args);

  // And implode() it all back together.
  $keys = $args ? implode('/', $args) : '';

  // Load my task plugin
  $task = page_manager_get_task('search_api_page');
  $subtask = page_manager_get_task_subtask($task, $type);

  // Load the node into a context.
  ctools_include('context');
  ctools_include('context-task-handler');
  $contexts = ctools_context_handler_get_task_contexts($task, $subtask, array($keys));
  $output = ctools_context_handler_render($task, $subtask, $contexts, array($keys));

  if ($output !== FALSE) {
    return $output;
  }

  // Otherwise, fall back.

  $function = 'search_api_page_view';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('search_api_page')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // Load the search page results with the given keywords.
  module_load_include('inc', 'search_api_page', 'search_api_page.pages');
  $args = array($type, $keys);
  return call_user_func_array($function, $args);
}

/**
 * Retrieves the arguments provided by this task handler.
 *
 * Since this is the node view and there is no UI on the arguments, we create
 * dummy arguments that contain the needed data.
 */
function search_api_page_search_api_page_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => 'keywords',
      'identifier' => t('Keywords'),
      'id' => 1,
      'name' => 'string',
      'settings' => array('use_tail' => TRUE),
    ),
  );
}

/**
 * Retrieves the context placeholders provided by this handler.
 */
function search_api_page_search_api_page_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(search_api_page_search_api_page_get_arguments($task, $subtask_id));
}

/**
 * Enables or disables the page from the UI.
 */
function search_api_page_search_api_page_enable($cache, $status) {
  variable_set('search_api_page_search_api_page_disabled_' . $cache->subtask_id, $status);

  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['search_api_page_enabling_search_api_page'] = TRUE;
  }
}

/**
 * Retrieves all subtasks for a task.
 */
function search_api_page_search_api_page_subtasks($task) {
  $return = array();
  foreach (search_api_page_load_multiple(FALSE) as $page => $info) {
    if ($info->path) {
      // We don't pass the $info because the subtask build could be called
      // singly without the $info when just the subtask needs to be built.
      $return[$page] = search_api_page_search_api_page_build_subtask($task, $page);
    }
  }

  return $return;
}

/**
 * Retrieves a specific subtask.
 */
function search_api_page_search_api_page_subtask($task, $subtask_id) {
  return search_api_page_search_api_page_build_subtask($task, $subtask_id);
}

/**
 * Builds a subtask array for a given search page.
 */
function search_api_page_search_api_page_build_subtask($task, $page) {
  $info = search_api_page_load($page);
  return array(
    'name' => $page,
    'admin title' => $info->name,
    'admin path' => "{$info->path}/!keywords",
    'admin description' => t('Search API Page @name', array('@name' => $info->name)),
    'admin type' => t('System'),
    'row class' => empty($page->disabled) ? 'page-manager-enabled' : 'page-manager-disabled',
    'storage' => t('In code'),
    'disabled' => variable_get('search_api_page_search_api_page_disabled_' . $page, TRUE),
    // This works for both enable AND disable
    'enable callback' => 'search_api_page_search_api_page_enable',
  );
}

/**
 * Access callback: Determines if the given page is accessible.
 *
 * @param string $task
 *   The task plugin.
 * @param string $subtask_id
 *   The subtask ID.
 * @param array $contexts
 *   The contexts loaded for the task.
 *
 * @return bool
 *   TRUE if the current user can access the page, FALSE otherwise.
 */
function search_api_page_search_api_page_access_check($task, $subtask_id, $contexts) {
  return user_access('access search_api_page');
}
