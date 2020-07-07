<?php
function drupalchat_mobile_auth()
{ 
    if (variable_get('drupalchat_enable_mobile_sdk_integration', '2') == '1') {
            $form_state = array(
                'values' => array
                (
                    'name' => $_POST['username'],
                    'pass' => $_POST['password'],
                    'submit' => 'Log in',
                    'form_build_id' => 'form-' . drupal_random_key(),
                    'form_id' => 'user_login_block',
                    'op' => 'Log in',
                ),
				);
                
            user_login_authenticate_validate(null, $form_state);
            if (empty($form_state['uid'])) {
                // Always register an IP-based failed login event.
                flood_register_event('failed_login_attempt_ip', variable_get('user_failed_login_ip_window', 3600));
                // Register a per-user failed login event.
                if (isset($form_state['flood_control_user_identifier'])) {
                    flood_register_event('failed_login_attempt_user', variable_get('user_failed_login_user_window', 21600), $form_state['flood_control_user_identifier']);
                }

                if (isset($form_state['flood_control_triggered'])) {
                    if ($form_state['flood_control_triggered'] == 'user') {
                        print_r('Temporarily blocked.');
                    } else {
                        // We did not find a uid, so the limit is IP-based.
                        print_r('IP blocked');
                    }
                }
				else {
                    print_r('Wrong password');
                }
            } elseif (isset($form_state['flood_control_user_identifier'])) {
                // Clear past failures for this user so as not to block a user who might
                // log in and out more than once in an hour.
                flood_clear_event('failed_login_attempt_user', $form_state['flood_control_user_identifier']);
				   global $user;
                   $user = user_load($form_state['uid']);
			       $result = _drupalchat_get_auth($_POST['username']);
				   drupal_json_output($result); 
					
                } else {
                    drupal_access_denied();
                }
            
        }else {
            print_r("Please Enable Mobile SDK Integration");
        }
}
