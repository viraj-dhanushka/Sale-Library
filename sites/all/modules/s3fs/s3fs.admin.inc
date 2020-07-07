<?php

/**
 * @file
 * Administration form setup for S3 File System.
 */

/**
 * Builds the Settings form.
 */
function s3fs_settings() {
  $form = array();
  // NOTE: I'd like to be able to pull this information directly from the SDK, but
  // I couldn't find a way to get the human-readable region names.
  $region_map = array(
    '' => t('Default'),
    'us-east-1' => t('US East - Northern Virginia (us-east-1)'),
    'us-east-2' => t('US East - Ohio (us-east-2)'),
    'us-west-1' => t('US West - Northern California  (us-west-1)'),
    'us-west-2' => t('US West - Oregon (us-west-2)'),
    'us-gov-west-1' => t('USA GovCloud Standard (us-gov-west-1)'),
    'ca-central-1' => t('Canada - Central (ca-central-1)'),
    'eu-west-1' => t('EU - Ireland  (eu-west-1)'),
    'eu-west-2' => t('EU - London (eu-west-2)'),
    'eu-west-3' => t('EU - Paris (eu-west-3)'),
    'eu-central-1' => t('EU - Frankfurt (eu-central-1)'),
    'ap-southeast-1' => t('Asia Pacific - Singapore (ap-southeast-1)'),
    'ap-southeast-2' => t('Asia Pacific - Sydney (ap-southeast-2)'),
    'ap-northeast-1' => t('Asia Pacific - Tokyo (ap-northeast-1)'),
    'ap-northeast-2' => t('Asia Pacific - Seoul (ap-northeast-2)'),
    'ap-northeast-3' => t('Asia Pacific - Osaka-Local (ap-northeast-3)'),
    'ap-south-1' => t('Asia Pacific - Mumbai (ap-south-1)'),
    'sa-east-1' => t('South America - Sao Paulo (sa-east-1)'),
    'cn-north-1' => t('China - Beijing (cn-north-1)'),
    'cn-northwest-1' => t('China - Ningxia (cn-northwest-1)'),
  );

  /////////////////////////////////////
  // AWS CREDENTIALS
  /////////////////////////////////////
  $form['aws_credentials'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Amazon Web Services Credentials'),
    '#description' => t(
      "To configure your Amazon Web Services credentials, enter the values in the appropriate fields below.
      You may instead set \$conf['awssdk2_access_key'] and \$conf['awssdk2_secret_key'] in settings.php."
    ),
    '#collapsible' => TRUE,
    // Simplify the form by collapsing this fieldset if it's already been configured.
    '#collapsed'   => _s3fs_get_setting('awssdk2_access_key') ||
                      _s3fs_get_setting('awssdk2_use_instance_profile'),
  );

  # NOTE: The awssdk2 settings can be a bit confusing. They start with "s3fs_"
  # to ensure that they get pulled as part of _s3fs_get_config(), but they can
  # be hard-coded as overrides in settings.php without the s3fs_ prefix.
  $form['aws_credentials']['s3fs_awssdk2_access_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Amazon Web Services Access Key'),
    '#default_value' => _s3fs_get_setting('awssdk2_access_key'),
  );

  $form['aws_credentials']['s3fs_awssdk2_secret_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Amazon Web Services Secret Key'),
    '#default_value' => _s3fs_get_setting('awssdk2_secret_key'),
  );

  $form['aws_credentials']['s3fs_awssdk2_use_instance_profile'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use EC2 Instance Profile Credentials'),
    '#default_value' => _s3fs_get_setting('awssdk2_use_instance_profile'),
    '#description'   => t(
      'If your Drupal site is running on an Amazon EC2 server, you may use the Instance Profile Credentials
      from that server rather than setting your AWS credentials directly.'
    ),
  );

  $form['aws_credentials']['s3fs_awssdk2_default_cache_config'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Default Cache Location'),
    '#default_value' => _s3fs_get_setting('awssdk2_default_cache_config'),
    '#description'   => t('The default folder in which your EC2 Instance Profile Credentials should be cached.
      This must be a path to a folder in which the webserver can read and write.'),
    '#states'        => array(
      'visible' => array(
        ':input[id=edit-s3fs-awssdk2-use-instance-profile]' => array('checked' => TRUE),
      ),
    ),
  );

  /////////////////////////////////////
  // BASIC CONFIGURATION
  /////////////////////////////////////
  $form['s3fs_bucket'] = array(
    '#type'          => 'textfield',
    '#title'         => t('S3 Bucket Name'),
    '#default_value' => _s3fs_get_setting('bucket'),
    '#required'      => TRUE,
  );

  $form['s3fs_region'] = array(
    '#type'          => 'select',
    '#options'       => $region_map,
    '#title'         => t('S3 Region'),
    '#default_value' => _s3fs_get_setting('region'),
    '#description'   => t(
      'The region in which your bucket resides. Be careful to specify this accurately,
      as you are likely to see strange or broken behavior if the region is set wrong.<br>
      Use of the USA GovCloud region requires !SPECIAL_PERMISSION.<br>
      Use of the China - Beijing region requires a !CHINESE_AWS_ACCT.',
      array(
          '!CHINESE_AWS_ACCT' => l('亚马逊 AWS account', 'http://www.amazonaws.cn'),
          '!SPECIAL_PERMISSION' => l('special permission', 'http://aws.amazon.com/govcloud-us/'),
      )
    ),
  );

  /////////////////////////////////////
  // ADVANCED CONFIGURATION
  /////////////////////////////////////
  $form['advanced'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Advanced Configuration Options'),
    '#collapsible' => TRUE,
    // This is always collapsed because most users won't need to alter these settings.
    '#collapsed'   => TRUE,
  );

  $form['advanced']['s3fs_use_cname'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use a CNAME'),
    '#default_value' => _s3fs_get_setting('use_cname'),
    '#description'   => t('Serve files from a custom domain, instead of "BUCKET_NAME.s3.REGION.amazonaws.com".'),
  );

  $form['advanced']['s3fs_cname_settings_fieldset'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('CNAME Settings'),
    '#states' => array(
      'visible' => array(
        ':input[id=edit-s3fs-use-cname]' => array('checked' => TRUE),
      ),
    ),
    's3fs_domain' => array(
      '#type'          => 'textfield',
      '#title'         => t('Domain Name'),
      '#default_value' => _s3fs_get_setting('domain'),
      '#description'   => t("The domain name from which you want your files to be served. " .
        "You will need to configure a DNS CNAME that maps from this domain to your bucket's URL."
      ),
    ),
    's3fs_domain_root' => array(
      '#type'          => 'select',
      '#title'         => t('Map Domain Name to specific path'),
      '#options'       => array(
        'none' => t('Entire bucket (mybucket)'),
        'root' => t('Root folder (mybucket/root)'),
        'public' => t('Public folder (mybucket/s3fs-public)'),
        'root_public' => t('Root & Public folders (mybucket/root/s3fs-public)'),
      ),
      '#default_value' => _s3fs_get_setting('domain_root'),
      '#description'   => t("Map the domain name to the location from where the file should be pulled. " .
        "This is useful when using a service such as Cloudfront where the origin path can be a specific " .
        "folder in a bucket, rather than the entire bucket. For example, with a root folder of \"staging\" and " .
        "a public folder of \"s3fs-public\" (for a full path of \"mybucket/staging/s3fs-public\"), the Root & " .
        "Public folders option will allow the full path to be mapped directly to the domain name " .
        "(\"my-cloudfront-id.cloudfront.net\"). By default, the domain name maps to the entire bucket."
      ),
    ),
    's3fs_domain_s3_private' => array(
      '#type'          => 'checkbox',
      '#title'         => t('Remove access to files via S3 URL'),
      '#default_value' => _s3fs_get_setting('domain_s3_private'),
      '#description'   => t("If using a domain name to serve files, there is usually no need to provide " .
        "access to the URLs shown on S3 objects. This can also be a cost concern, since accessing files from S3 " .
        "is more expensive than edge services such as Cloudfront. Select this checkbox to remove access to files " .
        "using the S3 URL. Users attempting to reach the S3 URL will receive an \"Access Denied\" message. Note: " .
        "This option has no impact on previously uploaded files."
      ),
    ),
  );

  $form['advanced']['s3fs_use_customhost'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use a Custom Host'),
    '#default_value' => _s3fs_get_setting('use_customhost'),
    '#description'   => t('Connect to an S3-compatible storage service other than Amazon.'),
  );

  $form['advanced']['s3fs_costomhost_settings_fieldset'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Custom Host Settings'),
    '#states' => array(
      'visible' => array(
        ':input[id=edit-s3fs-use-customhost]' => array('checked' => TRUE),
      ),
    ),
    's3fs_hostname' => array(
      '#type'          => 'textfield',
      '#title'         => t('Hostname'),
      '#default_value' => _s3fs_get_setting('hostname'),
      '#description'   => t('Custom service hostname, e.g. "objects.dreamhost.com".'),
      '#states'        => array(
        'visible' => array(
          ':input[id=edit-s3fs-use-customhost]' => array('checked' => TRUE),
        ),
      ),
    ),
  );

  $form['advanced']['s3fs_no_redirect_derivatives'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Don't redirect to derivatives"),
    '#default_value' => _s3fs_get_setting('no_redirect_derivatives'),
    '#description'   => t(
      'Normally, when a new image derivative is created, S3FS will redirect the client to its URL in S3.
      This may be undersirable for sites which use a CDN, so this checkbox was added to make S3FS upload
      the derivative to the client, instead. Doing so is less performant, but it allows the CDN to cache
      the file itself, instead of caching the redirect.'
    ),
  );

  $form['advanced']['s3fs_use_versioning'] = array(
    '#type'             => 'checkbox',
    '#title'            =>  t('Use Bucket Versioning'),
    '#default_value'    => _s3fs_get_setting('use_versioning'),
    '#description'      => t(
        'If your Bucket has Versioning enabled and you would like to store the version metadata
        in the s3fs_file table'
    ),
  );

  $form['advanced']['s3fs_cache_control_header'] = array(
    '#type'          => 'textfield',
    '#title'         => t('S3 Object Cache-Control Header'),
    '#default_value' => _s3fs_get_setting('cache_control_header'),
    '#description'   => t('The cache control header to apply on all S3 objects (for use by CDNs and browsers), e.g.
      "public, max-age=300".'
    ),
  );

  $form['advanced']['s3fs_encryption'] = array(
    '#type'          => 'select',
    '#options'       => array('' => 'None', 'AES256' => 'AES256', 'aws:kms' => 'aws:kms'),
    '#title'         => t('Server-Side Encryption'),
    '#default_value' => _s3fs_get_setting('encryption'),
    '#description'   => t(
      'If your bucket requires !ENCRYPTION, you can specify the encryption algorithm here',
      array(
        '!ENCRYPTION' => l('server-side encryption',
          'http://docs.aws.amazon.com/AmazonS3/latest/dev/UsingServerSideEncryption.html'
        ),
      )
    ),
  );

  $form['advanced']['s3fs_use_https'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Always serve files from S3 via HTTPS'),
    '#default_value' => _s3fs_get_setting('use_https'),
    '#description'   => t(
      'Forces S3 File System to always generate HTTPS URLs for files in your bucket,
      e.g. "https://mybucket.s3.amazonaws.com/smiley.jpg".<br>
      Without this setting enabled, URLs for your files will use the same scheme as the page they are served from.'
    ),
  );

  $form['advanced']['s3fs_ignore_cache'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Ignore the file metadata cache'),
    '#default_value' => _s3fs_get_setting('ignore_cache'),
    '#description'   => t(
      "If you need to debug a problem with S3, you may want to temporarily ignore the file metadata cache.
      This will make all file system reads hit S3 instead of the cache.<br>
      <b>This causes s3fs to function extremely slowly, and should never be enabled on a production site.</b>"
    ),
  );

  $form['advanced']['s3fs_use_s3_for_public'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use S3 for public:// files'),
    '#default_value' => _s3fs_get_setting('use_s3_for_public'),
    '#description'   => t(
      "Enable this option to store all files which would be uploaded to or created in the web server's local file
      system within your S3 bucket instead.<br><br>
      <b>PLEASE NOTE:</b> If you intend to use Drupal's performance options which aggregate your CSS or Javascript
      files, or will be using any other system that writes CSS or Javascript files into your site's public:// file
      system, you must perform some additional configuration on your webserver to make those files work correctly when
      stored in S3. Please see the section titled <i>Aggregated CSS and JS in S3</i> in the !README for details.",
      array('!README' => l('README', drupal_get_path('module', 's3fs') . '/README.txt'))
    ),
  );

  $form['advanced']['s3fs_no_rewrite_cssjs'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Don't render proxied CSS/JS file paths"),
    '#default_value' => _s3fs_get_setting('no_rewrite_cssjs'),
    '#description'   => t(
      "Normally, S3FS renders the URLs of public:// CSS and JS files to make use of the proxy settings that are
        assumed to be configured in your webserver based on the README. However, if you don't want it to do that, you
        can check this box to make S3FS serve the regular S3 or CNAME URL for public:// CSS and JS files, instead.<br>
        Under most setups, this will break your CSS and JS. Only enable this option if you <b>know</b> you need it!<br>
        A tool is available in the <a href=\"/admin/config/media/s3fs/actions\">S3 File System Actions</a> to copy system
        images to S3 when not rendering CSS/JS using proxied file paths. In many situations, this can help correct module,
        theme, and library images that would otherwise be broken when using this setting. Use with caution."
    ),
    '#states' => array(
      'visible' => array(
        ':input[id=edit-s3fs-use-s3-for-public]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['advanced']['s3fs_use_s3_for_private'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use S3 for private:// files'),
    '#default_value' => _s3fs_get_setting('use_s3_for_private'),
      '#description'   => t(
      'Enable this option to store all files which would be uploaded to or created in the private://
      file system (files available only to authenticated users) within your S3 bucket instead.'
    ),
  );

  $form['advanced']['s3fs_root_folder'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Root Folder'),
    '#default_value' => _s3fs_get_setting('root_folder'),
    '#description'   => t(
      'S3 File System uses the specified folder as the root of the file system within your bucket (if blank, the bucket
      root is used). This is helpful when your bucket is used by multiple sites, or has additional data in it which
      s3fs should not interfere with.<br>
      The metadata refresh function will not retrieve metadata for any files which are outside the Root Folder.<br>
      This setting is case sensitive. Do not include leading or trailing slashes.<br>
      Changing this setting <b>will not</b> move any files. If you\'ve already uploaded files to S3 through S3 File
      System, you will need to manually move them into this folder.'
    ),
  );


  $form['advanced']['additional_folders'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Additional Folder Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#description' => t(
      'Like the root folder, changing these settings <b>will not</b> move any files. If you\'ve already uploaded files
      to S3 through S3 File System, you will need to manually move them into the corresponding folders.'),
  );

  $form['advanced']['additional_folders']['s3fs_public_folder'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Public Folder'),
    '#default_value' => _s3fs_get_setting('public_folder', 's3fs-public'),
    '#description'   => t(
      'The name of the folder in your bucket (or within the root folder) where public:// files will be stored.'
    ),
  );

  $form['advanced']['additional_folders']['s3fs_private_folder'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Private Folder'),
    '#default_value' => _s3fs_get_setting('private_folder', 's3fs-private'),
    '#description'   => t(
      'The name of the folder in your bucket (or within the root folder) where private:// files will be stored.'
    ),
  );

  /////////////////////////////////////
  // FILE-SPECIFIC CONFIGURATION
  /////////////////////////////////////
  $form['advanced']['file_specific'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('File-specific Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['advanced']['file_specific']['s3fs_presigned_urls'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Presigned URLs'),
    '#default_value' => _s3fs_get_setting('presigned_urls'),
    '#rows'          => 5,
    '#description'   => t(
      'A list of timeouts and paths that should be delivered through a presigned url.<br>
      Enter one value per line, in the format timeout|path. e.g. "60|private_files/*". Paths use regex patterns
      as per !link. If no timeout is provided, it defaults to 60 seconds.<br>
      <b>This feature does not work when "Enable CNAME" is used.</b>',
      array('!link' => l('preg_match', 'http://php.net/preg_match'))
    ),
  );

  $form['advanced']['file_specific']['s3fs_saveas'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Force Save As'),
    '#default_value' => _s3fs_get_setting('saveas'),
    '#rows'          => 5,
    '#description'   => t(
      'A list of paths for which users will be forced to save the file, rather than displaying it in the browser.<br>
      Enter one value per line. e.g. "video/*". Paths use regex patterns as per !link.<br>
      <b>This feature does not work when "Enable CNAME" is used.</b>',
      array('!link' => l('preg_match', 'http://php.net/preg_match'))
    ),
  );

  $form['advanced']['file_specific']['s3fs_torrents'] = array(
    '#type' => 'textarea',
    '#title' => t('Torrents'),
    '#default_value' => _s3fs_get_setting('torrents'),
    '#rows' => 5,
    '#description' => t(
      'A list of paths that should be delivered via BitTorrent.<br>
      Enter one value per line, e.g. "big_files/*". Paths use regex patterns as per !link.<br>
      <b>Private files and paths already set as Presigned URLs or Forced Save As cannot be delivered as torrents.</b>',
      array('!link' => l('preg_match', 'http://php.net/preg_match'))
    ),
  );

  $form = system_settings_form($form);
  return $form;
}

/**
 * Validates the values on the admin form.
 */
function s3fs_settings_validate($form, &$form_state) {
  $config_from_form = _s3fs_convert_form_state_to_config($form_state);
  // Get all the saved settings from s3fs_get_config(), then override them with the config from the form.
  $config = array_merge(_s3fs_get_config(), $config_from_form);
  _s3fs_validate_config($config);
}

/**
 * Converts a $form_state array to a configuration settings array.
 */
function _s3fs_convert_form_state_to_config($form_state) {
  $config = array();
  foreach ($form_state['values'] as $name => $value) {
    // If the name starts with 's3fs_', strip that off and save the value.
    if (substr($name, 0, 5) == 's3fs_') {
      $config[substr($name, 5)] = $value;
    }
  }
  return $config;
}

/**
 * Builds the Actions form.
 */
function s3fs_actions() {
  $form = array();

  // Drupal's menu system doesn't let you set the page title for tabs.
  // So we set it here.
  drupal_set_title('S3 File System Actions');

  $form['s3fs_refresh_cache'] = array(
    '#type' => 'fieldset',
    '#description' => t(
      "The file metadata cache keeps track of every file that S3 File System writes to (and deletes from) the S3 bucket,
      so that queries for data about those files (checks for existence, filetype, etc.) don't have to hit S3.
      This speeds up many operations, most noticeably anything related to images and their derivatives."
    ),
    '#title' => t('File Metadata Cache'),
  );
  $refresh_description = t(
    "This button queries S3 for the metadata of <i><b>all</b></i> the files in your site's bucket (unless you use the
    Root Folder option), and saves it to the database. This may take a while for buckets with many thousands of files. <br>
    It should only be necessary to use this button if you've just installed S3 File System and you need to cache all the
    pre-existing files in your bucket, or if you need to restore your metadata cache from scratch for some other reason."
  );
  $form['s3fs_refresh_cache']['refresh'] = array(
    '#type' => 'submit',
    '#suffix' => '<div class="refresh">' . $refresh_description . '</div>',
    '#value' => t('Refresh file metadata cache'),
    '#attached' => array(
      'css' => array(
        // Push the button closer to its own description, and push the disable
        // checkbox away from the description.
        '#edit-refresh {margin-bottom: 0; margin-top: 1em;} div.refresh {margin-bottom: 1em;}' => array('type' => 'inline')
      ),
    ),
    '#submit' => array('_s3fs_refresh_cache_submit'),
  );

  $config = _s3fs_get_config();
  $use_public = !empty($config['use_s3_for_public']);
  $use_private = !empty($config['use_s3_for_private']);

  if ($use_public || $use_private) {
    $form['s3fs_copy_local'] = array(
      '#type' => 'fieldset',
      '#description' => t(
        "Since you have S3 File System configured to take over for the public and/or private file systems, you
        may wish to copy any files which were previously uploaded to your site into your S3 bucket. <br>
        If you have a lot of files, or very large files, you'll want to use <i>drush s3fs-copy-local</i>
        instead of this form, as the limitations imposed by browsers may break very long copy operations."
      ),
      '#title' => t('Copy Local Files to S3'),
    );
    if ($use_public) {
      $form['s3fs_copy_local']['public'] = array(
        '#type' => 'submit',
        '#prefix' => '<br>',
        '#name' => 'public',
        '#value' => t('Copy local public files to S3'),
        '#submit' => array('_s3fs_copy_local_submit'),
      );
    }
    if ($use_private) {
      $form['s3fs_copy_local']['private'] = array(
        '#type' => 'submit',
        '#prefix' => '<br>',
        '#name' => 'private',
        '#value' => t('Copy local private files to S3'),
        '#submit' => array('_s3fs_copy_local_submit'),
      );
    }
  }

  $no_rewrite = !empty($config['no_rewrite_cssjs']);

  if ($no_rewrite) {
    $form['s3fs_copy_system'] = array(
      '#type' => 'fieldset',
      '#description' => t(
        "Since you have S3 File System configured to not render proxied CSS/JS file paths, you
        may want to copy system images to S3. These are images included with your site's modules, themes,
        and libraries.<br>If you are using CSS aggregation either through Drupal's default settings or
        a separate module such as AdvAgg, the aggregated files most likely will include <em>url</em>
        definitions that point to your site's file system, not S3.<br>This feature will copy system images
        to S3 with the correct directory structure. <strong>Please note, if new modules, themes, or
        libraries are added to your site, this option will need to be run again to update S3.</strong>"
      ),
      '#title' => t('Copy System Images to S3'),
    );
    $form['s3fs_copy_system']['system'] = array(
      '#type' => 'submit',
      '#prefix' => '<br>',
      '#value' => t('Copy system images to S3'),
      '#submit' => array('_s3fs_copy_system_images_submit'),
    );
  }

  return $form;
}

/**
 * Submit callback for the "Refresh file metadata cache" button.
 */
function _s3fs_refresh_cache_submit($form, &$form_state) {
  _s3fs_refresh_cache(_s3fs_get_config());
}

/**
 * Submit callback for the "Copy local files to S3" buttons.
 */
function _s3fs_copy_local_submit($form, &$form_state) {
  _s3fs_copy_file_system_to_s3($form_state['triggering_element']['#name']);
}

/**
 * Submit callback for the "Copy system images to S3" button.
 */
function _s3fs_copy_system_images_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/media/s3fs/actions/copy-images';
}
