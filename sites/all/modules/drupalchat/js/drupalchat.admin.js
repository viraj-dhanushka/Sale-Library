jQuery(document).ready(function() {

	jQuery("input[name=drupalchat_polling_method]").change(function() {
	    if(jQuery("input[name=drupalchat_polling_method]:checked").val() != '3'){
	    	jQuery('.form-item-drupalchat-app-id').hide();
	    	jQuery('.form-item-drupalchat-external-api-key').hide();
	    	jQuery('.form-item-drupalchat-notification-sound').show();
	    	jQuery('.form-item-drupalchat-theme').show();
	    	jQuery('.form-item-drupalchat-user-picture').show();
	    	jQuery('.form-item-drupalchat-enable-smiley').show();
	    	jQuery('.form-item-drupalchat-log-messages').show();
	    	jQuery('.form-item-drupalchat-anon-prefix').show();
	    	jQuery('.form-item-drupalchat-anon-use-name').show();
	    	jQuery('.form-item-drupalchat-user-latency').show();
	    	jQuery('.form-item-drupalchat-refresh-rate').show();
	    	jQuery('#edit-drupalchat-pc').show();
	    	jQuery('.form-item-drupalchat-enable-chatroom').show();
	    	jQuery('#edit-drupalchat-show-embed-chat').hide();
	    	jQuery('#edit-drupalchat-advanced-settings').hide();
	    	jQuery('.form-item-drupalchat-session-caching').hide();
	    }else{
	    	jQuery('.form-item-drupalchat-app-id').show();
	    	jQuery('.form-item-drupalchat-external-api-key').show();
	    	jQuery('.form-item-drupalchat-notification-sound').hide();
	    	jQuery('.form-item-drupalchat-theme').hide();
	    	jQuery('.form-item-drupalchat-user-picture').hide();
	    	jQuery('.form-item-drupalchat-enable-smiley').hide();
	    	jQuery('.form-item-drupalchat-log-messages').hide();
	    	jQuery('.form-item-drupalchat-anon-prefix').hide();
	    	jQuery('.form-item-drupalchat-anon-use-name').hide();
	    	jQuery('.form-item-drupalchat-user-latency').hide();
	    	jQuery('.form-item-drupalchat-refresh-rate').hide();
	    	jQuery('#edit-drupalchat-pc').hide();
	    	jQuery('.form-item-drupalchat-enable-chatroom').hide();
	    	jQuery('#edit-drupalchat-show-embed-chat').show();
	    	jQuery('#edit-drupalchat-advanced-settings').show();
	    	jQuery('.form-item-drupalchat-session-caching').show();
	    } 
	});

  jQuery("input[name=drupalchat_rel]").change(function() {
      if (jQuery("input[name=drupalchat_rel]:checked").val() == '1') {
        jQuery('#edit-drupalchat-ur-name').removeAttr('disabled');
		jQuery('#edit-drupalchat-ur-name').attr('required', 'true');
        jQuery('#edit-drupalchat-ur-name-wrapper').fadeIn();     
      }
      else {
        jQuery('#edit-drupalchat-ur-name').attr('disabled', 'disabled');
        jQuery('#edit-drupalchat-ur-name').removeAttr('required');
		jQuery('#edit-drupalchat-ur-name-wrapper').hide();
      }
  }); 
	
	jQuery("input[name=drupalchat_polling_method]").change();
	jQuery("input[name=drupalchat_rel]").change();

});



