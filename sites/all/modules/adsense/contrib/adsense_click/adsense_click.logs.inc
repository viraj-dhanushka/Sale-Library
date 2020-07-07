<?php

/**
 * @file
 * Functions to display the logs tracked by the adsense_click module.
 */

/**
 * Click log.
 */
function adsense_click_log() {
  $header = array(
    array('data' => t('Timestamp'), 'field' => 'ads.timestamp', 'sort' => 'desc'),
    array('data' => t('Path'), 'field' => 'ads.path'),
    array('data' => t('IP/Host'), 'field' => 'ads.ip'),
    array('data' => t('Referrer'), 'field' => 'ads.referrer'),
  );
  $rows = array();

  $name_resolve = variable_get('adsense_click_tracking_name_resolve', ADSENSE_CLICK_TRACKING_NAME_RESOLVE_DEFAULT);

  // Create our base query.
  $query = db_select('adsense_clicks', 'ads');
  $query = $query->extend('PagerDefault')->extend('TableSort');

  $query
    ->fields('ads', array('ip', 'timestamp', 'path', 'title', 'referrer'))
    ->limit(50)
    ->orderByHeader($header);

  $result = $query->execute();

  foreach ($result as $log) {
    if ($name_resolve) {
      $host = gethostbyaddr($log->ip);
    }
    else {
      $host = $log->ip;
    }

    $rows[] = array(
      array('data' => format_date($log->timestamp, 'small'), 'nowrap' => 'nowrap'),
      _adsense_click_format_path($log->path, $log->title),
      l($host, 'http://whois.domaintools.com/' . $log->ip),
      _adsense_click_format_path($log->referrer),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows)) . theme('pager');
}

/**
 * Click top pages.
 */
function adsense_click_top_pages() {
  $header = array(
    array('data' => t('Path'), 'field' => 'ads.path'),
    array('data' => t('Clicks'), 'field' => 'count', 'sort' => 'desc'),
    array('data' => t('Last'), 'field' => 'last'),
  );
  $rows = array();

  // Create our base query.
  $query = db_select('adsense_clicks', 'ads');
  $query = $query->extend('PagerDefault')->extend('TableSort');

  $query
    ->fields('ads', array('path', 'title'))
    ->groupBy('path')
    ->limit(50)
    ->orderByHeader($header);

  $query->addExpression('COUNT(*)', 'count');
  $query->addExpression('MAX(timestamp)', 'last');

  $result = $query->execute();

  foreach ($result as $log) {
    $rows[] = array(
      array('data' => _adsense_click_format_path($log->path, $log->title)),
      array('data' => $log->count, 'align' => 'right'),
      array('data' => format_date($log->last, 'small'), 'nowrap' => 'nowrap'),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows)) . theme('pager');
}

/**
 * Click by day.
 */
function adsense_click_by_day() {
  $header = array(
    array('data' => t('Day'), 'field' => 'day', 'sort' => 'desc'),
    array('data' => t('Clicks'), 'field' => 'count'),
  );
  $rows = array();

  $query = db_select('adsense_clicks', 'ads');
  $count_query = clone $query;
  $query = $query->extend('PagerDefault')->extend('TableSort');

  // @TODO Must be an easier and prettier way of doing this.
  $database_driver = db_driver();

  switch ($database_driver) {
    case 'mysql':
      $count_query->addExpression("COUNT(DISTINCT(FROM_UNIXTIME(timestamp, '%y-%m-%d')))");
      $query->addExpression("FROM_UNIXTIME(timestamp, '%Y-%m-%d')", 'day');
      break;

    case 'pgsql':
      $count_query->addExpression("COUNT(DISTINCT(TO_CHAR(timestamp, 'YYYY-MM-DD')))");
      $query->addExpression("TO_CHAR(timestamp, 'YYYY-MM-DD')", 'day');
      break;

    case 'sqlite':
      $count_query->addExpression("COUNT(DISTINCT(datetime(timestamp, 'unixepoch', 'localtime')))");
      $query->addExpression("datetime(timestamp, 'unixepoch', 'localtime')", 'day');
      break;
  }

  // Extend our query and add our count expression.
  $query->addExpression('COUNT(*)', 'count');

  $query->groupBy('day')
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);

  $result = $query->execute();

  foreach ($result as $log) {
    $rows[] = array(
      $log->day,
      array('data' => $log->count, 'align' => 'right'),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows)) . theme('pager');
}

/**
 * Format path.
 */
function _adsense_click_format_path($path, $title = '', $width = 32) {
  global $base_url;

  if ($title) {
    $short_title = truncate_utf8($title, $width, FALSE, TRUE);
  }
  else {
    $title = $path;
    $short_path = preg_replace('?^' . $base_url . '?', '', $path);
    $short_title = truncate_utf8($short_path, $width, FALSE, TRUE);
  }

  return l($short_title, $path, array('attributes' => array('title' => $title)));
}
