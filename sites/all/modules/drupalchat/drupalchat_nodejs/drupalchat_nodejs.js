(function ($) {
  Drupal.drupalchat = Drupal.drupalchat || {};
  Drupal.drupalchat.removeDuplicates = function() {
    var liText = '', liList = $('#chatpanel .subpanel ul li'), listForRemove = [];
	$(liList).each(function () {
	  var text = $(this).text();
	  if (liText.indexOf('|'+ text + '|') == -1)
	    liText += '|'+ text + '|';
	  else
	    listForRemove.push($(this));
    });
	$(listForRemove).each(function(){
	  $(this).remove();
	  //drupalchat.online_users = drupalchat.online_users - 1;
	  jQuery('#chatpanel .online-count').html($('#chatpanel .subpanel ul > li').size());
	});
  };  
  
  Drupal.drupalchat.processChatDataNodejs = function(data) {
      var drupalchat_messages = data;
      if (drupalchat_messages.message.length > 0) {
        // Play new message sound effect
        var obj = swfobject.getObjectById("drupalchatbeep");
	if (obj) {
	  obj.drupalchatbeep(); // e.g. an external interface call
	}
      }
      value = data;
      //Add div if required.
      chatboxtitle = value.uid1;
      if (jQuery("#chatbox_"+chatboxtitle).length <= 0) {
        createChatBox(chatboxtitle, value.name, 1);
      }
      else if (jQuery("#chatbox_"+chatboxtitle+" .subpanel").is(':hidden')) {
        if (jQuery("#chatbox_"+chatboxtitle).css('display') == 'none') {
          jQuery("#chatbox_"+chatboxtitle).css('display','block');
        }
	jQuery("#chatbox_"+chatboxtitle+" a:first").click(); //Toggle the subpanel to make active
	jQuery("#chatbox_"+chatboxtitle+" .chatboxtextarea").focus();
      }
      value.message = value.message.replace(/{{drupalchat_newline}}/g,"<br />");
      value.message = emotify(value.message);
      if (jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent .chatboxusername a:last").html() == value.name) {
        jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").append('<p>'+value.message+'</p>');
      }
      else {
        var currentTime = new Date();
	var hours = currentTime.getHours();
	var minutes = currentTime.getMinutes();
	if (hours < 10) {
	  hours = "0" + hours;
	}
	if (minutes < 10) {
	  minutes = "0" + minutes;
	}				
	jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").append('<div class="chatboxusername"><span class="chatboxtime">'+hours+':'+minutes+'</span><a href="'+Drupal.settings.basePath+'user/'+chatboxtitle+'">'+value.name+'</a></div><p>'+value.message+'</p>');
      }
      jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").scrollTop(jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent")[0].scrollHeight);
      jQuery.titleAlert(Drupal.settings.drupalchat.newMessage, {requireBlur:true, stopOnFocus:true, interval:800});
  };
  
Drupal.drupalchat.processUserOnline = function(data){
  if(data.uid!=Drupal.settings.drupalchat.uid) {
    if(jQuery("a #drupalchat_user_"+data.uid).length <= 0) {
      jQuery('#chatpanel .subpanel ul > li.link').remove();
      jQuery('#chatpanel .subpanel ul').append('<li class="status-' + '1' + '"><a class="' + data.uid + '" href="#" id="drupalchat_user_' + data.uid + '">' + data.name + '</a></li>');
      //drupalchat.online_users = drupalchat.online_users + 1;
      jQuery('#chatpanel .online-count').html($('#chatpanel .subpanel ul > li').size());
    }
  }
  Drupal.drupalchat.removeDuplicates();
};

Drupal.drupalchat.processUserOffline = function(data){
  if(data!=Drupal.settings.drupalchat.uid) {
    if(jQuery("#drupalchat_user_"+data).length > 0) {
      jQuery("#drupalchat_user_"+data).parent().remove();
      //drupalchat.online_users = drupalchat.online_users - 1;
      jQuery('#chatpanel .online-count').html($('#chatpanel .subpanel ul > li').size());
      if($('#chatpanel .subpanel ul > li').size() == 0)
        jQuery('#chatpanel .subpanel ul').empty();
        //jQuery('#chatpanel .subpanel ul').append(Drupal.settings.drupalchat.noUsers);
		//drupalchat.online_users = 0;
    }
  }
  Drupal.drupalchat.removeDuplicates();
};
Drupal.behaviors.drupalchat_nodejs = {
  attach: function(context, settings) {
    
  }
}   
Drupal.Nodejs.callbacks.drupalchatNodejsMessageHandler = {
  callback: function (message) {
    switch (message.type) {
      case 'newMessage':
        Drupal.drupalchat.processChatDataNodejs(jQuery.parseJSON(message.data));
        break;
      case 'userOnline':
        Drupal.drupalchat.processUserOnline(message.data);
        break;
      case 'userOffline':
        Drupal.drupalchat.processUserOffline(message.data);
        break;
      case 'createChannel':
        jQuery.post(Drupal.settings.drupalchat.addUrl);
	break;
    }
  }
};


})(jQuery);

