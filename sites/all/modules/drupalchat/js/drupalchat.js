(function() {

// Localize jQuery variable
var jQuery;

/******** Load jQuery if not present *********/
if (window.jQuery === undefined || (Number(window.jQuery.fn.jquery.split('.').join(''))>180)) {
    var script_tag = document.createElement('script');
    script_tag.setAttribute("type","text/javascript");
    script_tag.setAttribute("src",
        "//ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js");
    if (script_tag.readyState) {
      script_tag.onreadystatechange = function () { // For old versions of IE
          if (this.readyState == 'complete' || this.readyState == 'loaded') {
              drupalchatScriptLoadHandler();
          }
      };
    } else { // Other browsers
      script_tag.onload = drupalchatScriptLoadHandler;
    }
    // Try to find the head, otherwise default to the documentElement
    (document.getElementsByTagName("head")[0] || document.documentElement).appendChild(script_tag);
} else {
    jQuery = window.jQuery;
    drupalchatMain();
}

function drupalchatScriptLoadHandler() {
    jQuery = window.jQuery.noConflict(true);
    drupalchatMain(); 
}

function drupalchatMain() {

  (function ($) {

    Drupal.drupalchat = Drupal.drupalchat || {};

    Drupal.drupalchat.sendMessages = function() {
  	jQuery.post(Drupal.settings.drupalchat.sendUrl, {
  	  drupalchat_message_id: drupalchat.send_current_message_id,
     	  drupalchat_uid2: drupalchat.send_current_uid2, 
     	  drupalchat_message: drupalchat.send_current_message,
        form_id: Drupal.settings.drupalchat.formId,
        form_token: Drupal.settings.drupalchat.formToken
  	});
    };

    Drupal.drupalchat.parseHTML = function(text) {
      var patt = /\b(http:\/\/|https:\/\/|ftp:\/\/|file:\/\/)?(www\.|ftp\.)?[A-Za-z0-9]+[-A-Z0-9@\/_$.]*(\.ac|\.ad|\.ae|\.aero|\.af|\.ag|\.ai|\.al|\.am|\.an|\.ao|\.aq|\.ar|\.arpa|\.as|\.asia|\.at|\.au|\.aw|\.ax|\.az|\.ba|\.bb|\.bd|\.be|\.bf|\.bg|\.bh|\.bi|\.biz|\.bj|\.bm|\.bn|\.bo|\.br|\.bs|\.bt|\.bv|\.bw|\.by|\.bz|\.ca|\.cat|\.cc|\.cd|\.cf|\.cg|\.ch|\.ci|\.ck|\.cl|\.cm|\.cn|\.co|\.com|\.coop|\.cr|\.cu|\.cv|\.cw|\.cx|\.cy|\.cz|\.de|\.dj|\.dk|\.dm|\.do|\.dz|\.ec|\.edu|\.ee|\.eg|\.er|\.es|\.et|\.eu|\.fi|\.fj|\.fk|\.fm|\.fo|\.fr|\.ga|\.gb|\.gd|\.ge|\.gf|\.gg|\.gh|\.gi|\.gl|\.gm|\.gn|\.gov|\.gp|\.gq|\.gr|\.gs|\.gt|\.gu|\.gw|\.gy|\.hk|\.hm|\.hn|\.hr|\.ht|\.hu|\.id|\.ie|\.il|\.im|\.in|\.info|\.int|\.io|\.iq|\.ir|\.is|\.it|\.je|\.jm|\.jo|\.jobs|\.jp|\.ke|\.kg|\.kh|\.ki|\.km|\.kn|\.kp|\.kr|\.kw|\.ky|\.kz|\.la|\.lb|\.lc|\.li|\.lk|\.lr|\.ls|\.lt|\.lu|\.lv|\.ly|\.ma|\.mc|\.md|\.me|\.mg|\.mh|\.mil|\.mk|\.ml|\.mm|\.mn|\.mo|\.mobi|\.mp|\.mq|\.mr|\.ms|\.mt|\.mu|\.museum|\.mv|\.mw|\.mx|\.my|\.mz|\.na|\.name|\.nc|\.ne|\.net|\.nf|\.ng|\.ni|\.nl|\.no|\.np|\.nr|\.nu|\.nz|\.om|\.org|\.pa|\.pe|\.pf|\.pg|\.ph|\.pk|\.pl|\.pm|\.pn|\.pr|\.pro|\.ps|\.pt|\.pw|\.py|\.qa|\.re|\.ro|\.rs|\.ru|\.rw|\.sa|\.sb|\.sc|\.sd|\.se|\.sg|\.sh|\.si|\.sj|\.sk|\.sl|\.sm|\.sn|\.so|\.sr|\.st|\.su|\.sv|\.sx|\.sy|\.sz|\.tc|\.td|\.tel|\.tf|\.tg|\.th|\.tj|\.tk|\.tl|\.tm|\.tn|\.to|\.tp|\.tr|\.travel|\.tt|\.tv|\.tw|\.tz|\.ua|\.ug|\.uk|\.us|\.uy|\.uz|\.va|\.vc|\.ve|\.vg|\.vi|\.vn|\.vu|\.wf|\.ws|\.xn|\.xxx|\.ye|\.yt|\.za|\.zm|\.zw)([-._~:\/?#\[\]@!$&'\(\)\*\+,;=][A-Za-z0-9\.\/\+&@#%=~_|]*)*\b/ig;
      var a = text.match(patt);
      var pos= 0,final_string = "",prefix = "";
      if (a)
      {
          for(var i =0 ; i < a.length ;i++)
          {
              pos= text.indexOf(a[i]) + a[i].length;
              prefix = "";
              if(pos > text.length)
              {   pos = text.length}
              if(!(a[i].indexOf("http://") == 0 || a[i].indexOf("https://") == 0))
              {
                  prefix = "http://";
              }
              final_string  = final_string + text.substring(0,pos).replace(a[i], "<a target = '_blank' href='" + prefix + a[i] + "'>" + a[i] + "</a>" );
              text = text.substring(pos,text.length);
          }
          if(text.length != 0)
          {
              final_string = final_string + text;
          }
          return final_string;
      }
      return text;
    };
    
    Drupal.drupalchat.checkChatBoxInputKey = function(event, chatboxtextarea, chatboxtitle) {
      if(event.keyCode == 13 && event.shiftKey == 0)  {
  	  message = jQuery(chatboxtextarea).val();
  	  message = message.replace(/^\s+|\s+$/g,"");
  	  message = message.substr(0,255);
  	  jQuery(chatboxtextarea).val('');
  	  jQuery(chatboxtextarea).focus();
  	  jQuery(chatboxtextarea).css('height','44px');
  	  var currentTime = new Date();
  	  var hours = currentTime.getHours();
  	  var minutes = currentTime.getMinutes();
  	  if (hours < 10) {
  	    hours = "0" + hours;
  	  }
  	  if (minutes < 10) {
  	    minutes = "0" + minutes;
  	  }
  	  if (message != '') {
          drupalchat.send_current_uid2 = chatboxtitle;
          drupalchat.send_current_message = message;
          var d = new Date();
          drupalchat.send_current_message_id = 'm_' + Drupal.settings.drupalchat.uid + '_' + drupalchat.send_current_uid2 + '_' + d.getTime();
          Drupal.drupalchat.sendMessages();

          message = message.replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/\"/g,"&quot;");
          message = Drupal.drupalchat.parseHTML(message);
          if(Drupal.settings.drupalchat.allowSmileys === "1") {
            message = emotify(message);
          }
          if (jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent .chatboxusername a:last").html() == Drupal.settings.drupalchat.username) {
            jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").append('<p class=\''+drupalchat.send_current_message_id+'\'>'+message+'</p>');
          }
          else {
  		  var output ='';
  		  output = output + '<div class="chatboxusername">';
  		  if(Drupal.settings.drupalchat.iup == "1") {
  		    output = output + '<div class="chatboxuserpicture"><img height="20" width="20" src="'+Drupal.settings.drupalchat.up+'" /></div>';
  		  }
  		  
  		  output = output + '<span class="chatboxtime">'+hours+':'+minutes+'</span><a href="'+Drupal.settings.basePath+'user/'+Drupal.settings.drupalchat.uid+'">'+Drupal.settings.drupalchat.username+'</a></div><p class=\''+drupalchat.send_current_message_id+'\'>'+message+'</p>';
            jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").append(output);
          }
  		jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").scrollTop(jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent")[0].scrollHeight);
  	  }
        return false;
  	}

      var adjustedHeight = chatboxtextarea.clientHeight;
      var maxHeight = 94;

      if (maxHeight > adjustedHeight) {
        adjustedHeight = Math.max(chatboxtextarea.scrollHeight, adjustedHeight);
      if (maxHeight)
        adjustedHeight = Math.min(maxHeight, adjustedHeight);
      if (adjustedHeight > chatboxtextarea.clientHeight)
        jQuery(chatboxtextarea).css('height', adjustedHeight+8 + 'px');
      } 
  	else {
        jQuery(chatboxtextarea).css('overflow', 'auto');
      }
      return true;
    };
    
    Drupal.drupalchat.changeStatus = function(id, state) {
  	if(state == 1) {
  	  jQuery('#' + id + ' .subpanel_title > div').removeClass('status-0').addClass('status-1');
  	  jQuery('#' + id + ' .drupalchat_userOffline').css('display','none');
  	}
  	else if(state == 0) {
  	  jQuery('#' + id + ' .subpanel_title > div').removeClass('status-1').addClass('status-0');
  	  jQuery('#' + id + ' .drupalchat_userOffline').css('display','block');
  	}
    };

  })(jQuery);

  var drupalchat = {
  	username: null,
  	uid: null,
  	send_current_message: null,
  	send_current_message_id: null,
  	last_timestamp: 0,
  	send_current_uid2: 0,
  	attach_messages_in_queue: 0,
  	running: 0,
  	online_users: 0,
  	/*smilies: { 
      //smiley     image_url          title_text              alt_smilies           
      ":)":    [ "1.gif",        "happy",                ":-)"                 ],
      ":(":    [ "2.gif",         "sad",                  ":-("                 ],
      ";)":    [ "3.gif",        "winking",              ";-)"                 ],
      ":D":    [ "4.gif",         "big grin",             ":-D"                 ],
      "^_^":   [ "muhaha.gif",      "happy eyes"                                  ],
      ">:o":   [ "haha.gif",        "laughing eyes"                               ],
      ":3":    [ "hohoho.png",      "laughing eyes"                               ],
      ">:-(":  [ "waiting.png",     "grumpy",               "X-(","X(","x-(","x(" ],
      ":'(":   [ "crying.png",      "crying"                                      ],
      ":o":    [ "omg.png",         "shocked",              ":-o"                 ],
      "8)":    [ "cool.png",        "glass",                "8-)","B)","B-)"      ],
      "8-|":   [ "ouch.png",        "cool",                 "8-|"                 ],
      ":p":    [ "tongue.png",      "tongue",               ":-p",":P",":-P"      ],
      "O.o":   [ "stupid-idea.png", "woot?!"                                      ],
      "-_-":   [ "huh-where.png",   "dark emote"                                  ],
      ":/":    [ "oopsy.png",       "duhhh",                ":-/",":\\",":-\\"    ],
      ":*":    [ "kiss.png",        "kiss",                 ":-*"                 ],
      "<3":    [ "love.png",        "heart",                                      ],
      "3:)":   [ "very-devil-plan.png", "devil smile"                                 ],
      "O:)":   [ "idiotic-smile.png",   "angel"                                       ]
    }*/
    smilies: { /*
      smiley     image_url          title_text              alt_smilies           */
      ":)":    [ "1.gif",           "happy",                ":-)"                 ],
      ":(":    [ "2.gif",           "sad",                  ":-("                 ],
      ";)":    [ "3.gif",           "winking",              ";-)"                 ],
      ":D":    [ "4.gif",           "big grin",             ":-D"                 ],
      ";;)":   [ "5.gif",           "batting eyelashes"                           ],
      ">:D<":  [ "6.gif",           "big hug"                                     ],
      ":-/":   [ "7.gif",           "confused",             ":/"                  ],
      ":x":    [ "8.gif",           "love struck",          ":X"                  ],
      ":\">":  [ "9.gif",           "blushing"                                    ],
      ":P":    [ "10.gif",          "tongue",               ":p", ":-p", ":-P"    ],
      ":-*":   [ "11.gif",          "kiss",                 ":*"                  ],
      "=((":   [ "12.gif",          "broken heart"                                ],
      ":-O":   [ "13.gif",          "surprise",             ":O"                  ],
      "X(":    [ "14.gif",          "angry"                                       ],
      ":>":    [ "15.gif",          "smug"                                        ],
      "B-)":   [ "16.gif",          "cool"                                        ],
      ":-S":   [ "17.gif",          "worried"                                     ],
      "#:-S":  [ "18.gif",          "whew!",                "#:-s"                ],
      ">:)":   [ "19.gif",          "devil",                ">:-)"                ],
      ":((":   [ "20.gif",          "crying",               ":-((", ":'(", ":'-(" ],
      ":))":   [ "21.gif",          "laughing",             ":-))"                ],
      ":|":    [ "22.gif",          "straight face",        ":-|"                 ],
      "/:)":   [ "23.gif",          "raised eyebrow",       "/:-)"                ],
      "=))":   [ "24.gif",          "rolling on the floor"                        ],
      "O:-)":  [ "25.gif",          "angel",                "O:)"                 ],
      ":-B":   [ "26.gif",          "nerd"                                        ],
      "=;":    [ "27.gif",          "talk to the hand"                            ],
      "I-)":   [ "28.gif",          "sleepy"                                      ],
      "8-|":   [ "29.gif",          "rolling eyes"                                ],
      "L-)":   [ "30.gif",          "loser"                                       ],
      ":-&":   [ "31.gif",          "sick"                                        ],
      ":-$":   [ "32.gif",          "don't tell anyone"                           ],
      "[-(":   [ "33.gif",          "not talking"                                 ],
      ":O)":   [ "34.gif",          "clown"                                       ],
      "8-}":   [ "35.gif",          "silly"                                       ],
      "<:-P":  [ "36.gif",          "party",                "<:-p"                ],
      "(:|":   [ "37.gif",          "yawn"                                        ],
      "=P~":   [ "38.gif",          "drooling"                                    ],
      ":-?":   [ "39.gif",          "thinking"                                    ],
      "#-o":   [ "40.gif",          "d'oh",                 "#-O"                 ],
      "=D>":   [ "41.gif",          "applause"                                    ],
      ":-SS":  [ "42.gif",          "nailbiting",           ":-ss"                ],
      "@-)":   [ "43.gif",          "hypnotized"                                  ],
      ":^o":   [ "44.gif",          "liar"                                        ],
      ":-w":   [ "45.gif",          "waiting",              ":-W"                 ],
      ":-<":   [ "46.gif",          "sigh"                                        ],
      ">:P":   [ "47.gif",          "phbbbbt",              ">:p"                 ],
      "<):)":  [ "48.gif",          "cowboy"                                      ],
      ":@)":   [ "49.gif",          "pig"                                         ],
      "3:-O":  [ "50.gif",          "cow",                  "3:-o"                ],
      ":(|)":  [ "51.gif",          "monkey"                                      ],
      "~:>":   [ "52.gif",          "chicken"                                     ],
      "@};-":  [ "53.gif",          "rose"                                        ],
      "%%-":   [ "54.gif",          "good luck"                                   ],
      "**==":  [ "55.gif",          "flag"                                        ],
      "(~~)":  [ "56.gif",          "pumpkin"                                     ],
      "~O)":   [ "57.gif",          "coffee"                                      ],
      "*-:)":  [ "58.gif",          "idea"                                        ],
      "8-X":   [ "59.gif",          "skull"                                       ],
      "=:)":   [ "60.gif",          "bug"                                         ],
      ">-)":   [ "61.gif",          "alien"                                       ],
      ":-L":   [ "62.gif",          "frustrated",           ":L"                  ],
      "[-O<":  [ "63.gif",          "praying"                                     ],
      "$-)":   [ "64.gif",          "money eyes"                                  ],
      ":-\"":  [ "65.gif",          "whistling"                                   ],
      "b-(":   [ "66.gif",          "feeling beat up"                             ],
      ":)>-":  [ "67.gif",          "peace sign"                                  ],
      "[-X":   [ "68.gif",          "shame on you"                                ],
      "\\:D/": [ "69.gif",          "dancing"                                     ],
      ">:/":   [ "70.gif",          "bring it on"                                 ],
      ";))":   [ "71.gif",          "hee hee"                                     ],
      "o->":   [ "72.gif",          "hiro"                                        ],
      "o=>":   [ "73.gif",          "billy"                                       ],
      "o-+":   [ "74.gif",          "april"                                       ],
      "(%)":   [ "75.gif",          "yin yang"                                    ],
      ":-@":   [ "76.gif",          "chatterbox"                                  ],
      "^:)^":  [ "77.gif",          "not worthy"                                  ],
      ":-j":   [ "78.gif",          "oh go on"                                    ],
      "(*)":   [ "79.gif",          "star"                                        ],
      ":)]":   [ "100.gif",         "on the phone"                                ],
      ":-c":   [ "101.gif",         "call me"                                     ],
      "~X(":   [ "102.gif",         "at wits' end"                                ],
      ":-h":   [ "103.gif",         "wave"                                        ],
      ":-t":   [ "104.gif",         "time out"                                    ],
      "8->":   [ "105.gif",         "daydreaming"                                 ],
      ":-??":  [ "106.gif",         "I don't know"                                ],
      "%-(":   [ "107.gif",         "not listening"                               ],
      ":o3":   [ "108.gif",         "puppy dog eyes"                              ],
      "X_X":   [ "109.gif",         "I don't want to see",  "x_x"                 ],
      ":!!":   [ "110.gif",         "hurry up!"                                   ],
      "\\m/":  [ "111.gif",         "rock on!"                                    ],
      ":-q":   [ "112.gif",         "thumbs down"                                 ],
      ":-bd":  [ "113.gif",         "thumbs up"                                   ],
      "^#(^":  [ "114.gif",         "it wasn't me"                                ],
      ":bz":   [ "115.gif",         "bee"                                         ],
      ":ar!":  [ "pirate.gif",      "pirate"                                      ],
      "[..]":  [ "transformer.gif", "transformer"                                 ]
    },
    open_chat_uids: {},
    current_open_chat_window_ids: {}
  };
  window.drupalchat = drupalchat;
  //(function ($) {

  jQuery(document).ready(function(){

    if(Drupal.drupalchat.Cookie.read('drupalchat_open_chat_uids') !== null) {
      var temp_open_chat_uids = [];
      temp_open_chat_uids = Drupal.drupalchat.Cookie.read('drupalchat_open_chat_uids').split(",");
      for(var e in temp_open_chat_uids) {
        drupalchat.open_chat_uids[temp_open_chat_uids[e]] = temp_open_chat_uids[e];
      }
    }
    
    if(Drupal.drupalchat.Cookie.read('drupalchat_current_open_chat_window_ids') !== null) {
      var temp_open_chat_window_uids = [];
      temp_open_chat_window_uids = Drupal.drupalchat.Cookie.read('drupalchat_current_open_chat_window_ids').split(",");
      for(var e in temp_open_chat_window_uids) {
        drupalchat.current_open_chat_window_ids[temp_open_chat_window_uids[e]] = temp_open_chat_window_uids[e];
      }
    }
    
  	
  	//Load smileys.
  	emotify.emoticons( Drupal.settings.drupalchat.smileyURL, drupalchat.smilies );
  	//Adjust panel height
  	jQuery.fn.adjustPanel = function(){
  	    jQuery(this).find("ul, .subpanel").css({ 'height' : 'auto'}); //Reset sub-panel and ul height
  	
  	    var windowHeight = jQuery(window).height(); //Get the height of the browser viewport
  	    var panelsub = jQuery(this).find(".subpanel").height(); //Get the height of sub-panel
  	    var panelAdjust = windowHeight - 100; //Viewport height - 100px (Sets max height of sub-panel)
  	    var ulAdjust =  panelAdjust - 25; //Calculate ul size after adjusting sub-panel
  	
  	    if (panelsub > panelAdjust) {	 //If sub-panel is taller than max height...
  	        jQuery(this).find(".subpanel").css({ 'height' : panelAdjust}); //Adjust sub-panel to max height
  	        jQuery(this).find("ul").css({ 'height' : (panelAdjust - 48)}); ////Adjust subpanel ul to new size
  	    }
  	    else { //If sub-panel is smaller than max height...
  	    	jQuery(this).find("ul").css({ 'height' : 'auto'}); //Set sub-panel ul to auto (default size)
  	    }
  	};
  	
  	//Execute function on load
  	jQuery("#chatpanel").adjustPanel(); //Run the adjustPanel function on #chatpanel
  	
  	//Each time the viewport is adjusted/resized, execute the function
  	jQuery(window).resize(function () {
  	    jQuery("#chatpanel").adjustPanel();
  	});
  	
  	//Add sound effect SWF file to document
  	jQuery("<div style=\"width: 0px; height: 0px; overflow: hidden;\"><object id=\"drupalchatbeep\" classid=\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\" width=\"1\" height=\"1\"><param name=\"movie\" value=\"" + Drupal.settings.drupalchat.sound + "\"/><!--[if !IE]>--><object type=\"application/x-shockwave-flash\" data=\"" + Drupal.settings.drupalchat.sound + "\" width=\"1\" height=\"1\"></object><!--<![endif]--></object></div>").appendTo(jQuery("body"));
  	swfobject.registerObject("drupalchatbeep", "9");
  	
  	//Click event on subpanels	
  	jQuery("#mainpanel li a.subpanel_toggle, .chatbox a.chatboxhead").live('click', function() { //If clicked on the first link of #chatpanel...
  	    if(jQuery(this).next(".subpanel").is(':visible')){ //If subpanel is already active...
  	        jQuery(this).next(".subpanel").hide(); //Hide active subpanel
  	        //jQuery("#drupalchat li a").removeClass('active'); //Remove active class on the subpanel trigger
  			jQuery(this).removeClass('active');
  	    }
  	    else { //if subpanel is not active...
  	        //jQuery(".subpanel").hide(); //Hide all subpanels
  	        jQuery(this).next(".subpanel").toggle(); //Toggle the subpanel to make active
  	        //jQuery("#drupalchat li a").removeClass('active'); //Remove active class on all subpanel trigger
  	        jQuery(this).removeClass('active');
  			jQuery(this).toggleClass('active'); //Toggle the active class on the subpanel trigger
  	        // Chat box functions
  	        var isTextarea = jQuery(this).next(".subpanel").children(".chatboxinput").children(".chatboxtextarea");
  	        if (isTextarea.length > 0) { 
  	        	isTextarea[0].focus();
  	        	jQuery(this).next(".subpanel").children(".chatboxcontent").scrollTop(jQuery(this).next(".subpanel").children(".chatboxcontent")[0].scrollHeight);
  	        }
  	    }
  		jQuery('#drupalchat-chat-options').hide();
  	    return false; //Prevent browser jump to link anchor
  	});
  		
  	jQuery('.subpanel .subpanel_title').live('click', function() { //Click anywhere and...
  	    //jQuery(".subpanel").hide(); //hide subpanel
  		jQuery(this).parent().hide(); //hide subpanel
  	    //jQuery("#drupalchat li a").removeClass('active'); //remove active class on subpanel trigger
  		jQuery(this).parent().parent().children('a').removeClass('active');
      var id = jQuery(this).parent().parent().attr('id').split('_')[1];
      if(jQuery('#chatbox_'+id).is(':visible')) {
        updateLocalChatWindowList(jQuery(this).parent().parent().attr('id').split('_')[1], 'min');
      }
  	});	
  	jQuery('#drupalchat .subpanel .subpanel_title span.options').live('click', function() { //Click anywhere and...
  		var offset = jQuery(this).offset();
  		offset.top = offset.top + 20;
  		offset.left = offset.left - 50;
  		if (jQuery('#drupalchat-chat-options').is(":hidden")) {
  		  jQuery('#drupalchat-chat-options').show();
  		}
  		else {
  		  jQuery('#drupalchat-chat-options').hide();
  		}
  		jQuery('#drupalchat-chat-options').offset({top: offset.top, left: offset.left});
  		return false;
  	});
  	jQuery('#drupalchat-chat-options-s').live('click', function(e) {
  	  if(jQuery(this).html() == '<a href="#">Mute</a>') {
  	    jQuery(this).html('<a href="#">Unmute</a>');
  		Drupal.settings.drupalchat.notificationSound = '2';
  	  }
  	  else {
  	    jQuery(this).html('<a href="#">Mute</a>');
  		Drupal.settings.drupalchat.notificationSound = '1';
  	  }
  	  jQuery('#drupalchat-chat-options').hide();
  	  return false;
  	});
  	jQuery('body').live('click', function() {
  	  jQuery('#drupalchat-chat-options').hide();
  	});
  	
  	jQuery('.subpanel ul').click(function(e) {
  //	    e.stopPropagation(); //Prevents the subpanel ul from closing on click
  	});

  	jQuery("#chatpanel .subpanel li:not(.link) a").live('click', function(e) {
        chatWith(jQuery(this).attr("class"), jQuery(this).text());
  			return false;
  	});	
  	
  	jQuery(".chatbox .subpanel_title span:not(.min)").live('click', function (e) {
      closeChatBox(jQuery(this).attr('class'));
  	});
  	//alert(Drupal.settings.drupalchat.status);
  	if (Drupal.settings.drupalchat.status == 1) {
              jQuery(".chat_options .chat_loading").removeClass('chat_loading').addClass('status-2').html(Drupal.settings.drupalchat.goIdle);
              jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "chat-1.png");
  	}
  	else if (Drupal.settings.drupalchat.status == 2) {
              jQuery(".chat_options .chat_loading").removeClass('chat_loading').addClass('status-1').html(Drupal.settings.drupalchat.goOnline);
              jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "chat-2.png");
  	}
  		
  	jQuery(".chat_options .status-1").live('click', function() {
              jQuery(".chat_options .status-1").removeClass('status-1').addClass('chat_loading');
              jQuery.post(Drupal.settings.drupalchat.statusUrl, {status: "1"}, function(data){
                  jQuery(".chat_options .chat_loading").removeClass('chat_loading').addClass('status-2').html(Drupal.settings.drupalchat.goIdle);
                  jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "chat-1.png");
              });
  	});
  	jQuery(".chat_options .status-2").live('click', function() {
              jQuery(".chat_options .status-2").removeClass('status-2').addClass('chat_loading');
              jQuery.post(Drupal.settings.drupalchat.statusUrl, {status: "2"}, function(data){
                  jQuery(".chat_options .chat_loading").removeClass('chat_loading').addClass('status-1').html(Drupal.settings.drupalchat.goOnline);
                  jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "chat-2.png");
              });
  	});	
  	
  	jQuery(".chat_options .options").live('click', function() {
              alert('Under Construction');
  	});
    
  	// Add short delay before first poll call. This avoids Chrome loading-icon bug. 
    setTimeout(function () {
      chatPoll();
    }, 500); 
    jQuery('#drupalchat .subpanel .chatboxcontent').live('mouseenter', function() {
      jQuery(this).css("overflow-y","auto");
  	//document.body.style.overflow='hidden';
    });
    jQuery('#drupalchat .subpanel .chatboxcontent').live('mouseleave', function() {
      jQuery(this).css("overflow-y","hidden");
  	//document.body.style.overflow='auto';
    });
    
    //Get thread history
    getThreadHistory();
  });


  function chatWith(chatboxtitle, chatboxname) {
      createChatBox(chatboxtitle, chatboxname);
      jQuery("#chatbox_"+chatboxtitle+" a:first").click(); //Toggle the subpanel to make active
      jQuery("#chatbox_"+chatboxtitle+" .chatboxtextarea").focus();
      updateLocalChatWindowList(chatboxtitle, 'open');
      
  }


  function createChatBox(chatboxtitle, chatboxname, chatboxblink) {
    
      if (jQuery("#chatbox_"+chatboxtitle).length > 0) {
          if (jQuery("#chatbox_"+chatboxtitle).css('display') == 'none') {
              jQuery("#chatbox_"+chatboxtitle).css('display', 'block');
          }
          jQuery("#chatbox_"+chatboxtitle+" .chatboxtextarea").focus();
          updateLocalChatWindowList(chatboxtitle, 'open');
          return;
      }
      else {
        updateLocalChatWindowList(chatboxtitle, 'min');
      }

      jQuery(" <li />" ).attr("id","chatbox_"+chatboxtitle)
      .addClass("chatbox")
      .html('<a href="#" class="chatboxhead"><span class="subpanel_title_text">'+chatboxname+'</span></a><div class="subpanel"><div class="subpanel_title"><span class="'+chatboxtitle+'" title="Close">x</span><span title = "Minimize" class="min">_</span><div class="status-1"></div>'+chatboxname+'</div><div class="chatboxcontent"></div><div class="drupalchat_userOffline">'+chatboxname+' is currently offline.</div><div class="chatboxinput"><textarea class="chatboxtextarea" onkeydown="return Drupal.drupalchat.checkChatBoxInputKey(event,this,\''+chatboxtitle+'\');"></textarea></div></div>')
      .prependTo(jQuery( "#mainpanel" ));

      if (chatboxblink == 1) {
          jQuery('#chatbox_'+chatboxtitle+' .chatboxhead').addClass("chatboxblink");
      }

      jQuery("#chatbox_"+chatboxtitle+" .chatboxtextarea").blur(function(){
          jQuery("#chatbox_"+chatboxtitle+" .chatboxtextarea").removeClass('chatboxtextareaselected');
      }).focus(function(){
          jQuery('#chatbox_'+chatboxtitle+' .chatboxhead').removeClass('chatboxblink');
          jQuery("#chatbox_"+chatboxtitle+" .chatboxtextarea").addClass('chatboxtextareaselected');
      });

      jQuery("#chatbox_"+chatboxtitle).click(function() {
          if (jQuery('#chatbox_'+chatboxtitle+' .chatboxcontent').css('display') != 'none') {
              jQuery("#chatbox_"+chatboxtitle+" .chatboxtextarea").focus();
              updateLocalChatWindowList(chatboxtitle, 'open');
          }
      });
      jQuery("#chatbox_"+chatboxtitle).show();
  }

  function chatPoll() {
      if(Drupal.settings.drupalchat.polling_method == '0') {
  	  setTimeout(function() {
          jQuery.getJSON(Drupal.settings.drupalchat.pollUrl, { drupalchat_last_timestamp: drupalchat.last_timestamp }, function(data) {
            processChatData(data);
          });
          /*setInterval(function() {
              jQuery.getJSON(Drupal.settings.drupalchat.pollUrl, { drupalchat_last_timestamp: drupalchat.last_timestamp }, function(data) {
                  processChatData(data);
              });*/
              /*jQuery.post(Drupal.settings.drupalchat.pollUrl,
                  {
                      drupalchat_last_timestamp: drupalchat.last_timestamp
                  },
                  processChatData
              );*/
        }, (Drupal.settings.drupalchat.refresh_rate) * 1000);
      }
      else if(Drupal.settings.drupalchat.polling_method == '1') {
          jQuery.getJSON(Drupal.settings.drupalchat.pollUrl, { drupalchat_last_timestamp: drupalchat.last_timestamp }, function(data) {
              processChatData(data);
          });
          /*jQuery.post(Drupal.settings.drupalchat.pollUrl,
              {
                  drupalchat_last_timestamp: drupalchat.last_timestamp
              },
              processChatData
          );*/
      }
  }

  function processChatData(data, history) {
      var drupalchat_messages = data;
  	//var drupalchat_messages = jQuery.getJSON(data);
          if((!drupalchat_messages.status || drupalchat_messages.status == 0)) {
  		if (drupalchat_messages.messages.length > 0) {
  			// Play new message sound effect
  			var obj = swfobject.getObjectById("drupalchatbeep");
  			if (obj && Drupal.settings.drupalchat.notificationSound === "1") {
  			  try {
  			    obj.drupalchatbeep(); // e.g. an external interface call
  		      }
  			  catch(e) {
  			  }
  			}			
  		}
  		jQuery.each(drupalchat_messages.messages, function(index, value) {
  		    var drupalselfmessage = false;
  			if(value.uid1 == Drupal.settings.drupalchat.uid) {
  			  drupalselfmessage = true;
  			}
  			//Add div if required.
  			if(value.uid2=="c-0") {
  			  drupalchatroom = true;
  			}
              else {
  			  drupalchatroom = false;
              }			
  			chatboxtitle = (drupalchatroom || drupalselfmessage)?value.uid2:value.uid1;
  			if (jQuery("#chatbox_"+chatboxtitle).length <= 0) {
  				createChatBox(chatboxtitle, drupalchatroom?"Public Chatroom":value.name, 1);
  			}
  			else if (jQuery("#chatbox_"+chatboxtitle+" .subpanel").is(':hidden')) {
  				if (jQuery("#chatbox_"+chatboxtitle).css('display') == 'none') {
  					jQuery("#chatbox_"+chatboxtitle).css('display','block');
  				}
  				jQuery('#chatbox_'+chatboxtitle+' .chatboxhead').addClass("chatboxblink");
  				jQuery('#chatbox_'+chatboxtitle+' .chatboxhead').live('click', function() {
  				  jQuery('#chatbox_'+chatboxtitle+' .chatboxhead').removeClass("chatboxblink");
  				  drupalchat.send_current_uid2 = chatboxtitle;
  				});
  				//jQuery("#chatbox_"+chatboxtitle+" a:first").click(); //Toggle the subpanel to make active
  				jQuery("#chatbox_"+chatboxtitle+" .chatboxtextarea").focus();
  			}
              if(value.uid1 == Drupal.settings.drupalchat.uid) {
                value.name = Drupal.settings.drupalchat.username;  
              }
  			if(jQuery("."+value.message_id)[0]){
  			  return;
              }			  
  			value.message = value.message.replace(/{{drupalchat_newline}}/g,"<br />");
  			value.message = Drupal.drupalchat.parseHTML(value.message);
        if(Drupal.settings.drupalchat.allowSmileys === "1") {
          value.message = emotify(value.message);
        }
  			if (jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent .chatboxusername a:last").html() == value.name) {
  				jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").append('<p class="' + value.message_id + '">'+value.message+'</p>');
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
          
          var formattedCurrentTime = hours+':'+minutes;
          
          if((typeof(history) === 'boolean') && history) {
            formattedCurrentTime = value.timestamp;
          }
          
          var output = '<div class="chatboxusername">';				
  				if(Drupal.settings.drupalchat.iup == "1") {
  				  output = output + '<div class="chatboxuserpicture"><img height="20" width="20" src="'+value.p+'" /></div>';
  				}
  				output = output + '<span class="chatboxtime">'+formattedCurrentTime+'</span><a href="'+Drupal.settings.basePath+'user/'+value.uid1+'">'+value.name+'</a></div><p class="' + value.message_id + '">'+value.message+'</p>';
  				jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").append(output);
  			}
  			jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent").scrollTop(jQuery("#chatbox_"+chatboxtitle+" .chatboxcontent")[0].scrollHeight);
  			
              if(typeof(jQuery.titleAlert)==="function") {
                jQuery.titleAlert(Drupal.settings.drupalchat.newMessage, {requireBlur:true, stopOnFocus:true, interval:800});
              }
  		});
  		
  	  jQuery('#chatpanel .subpanel ul').empty();
  	  jQuery('li[id^="chatbox_"]').each(function(){
  	    Drupal.drupalchat.changeStatus(this.id,0);
  	  });
  	  if(typeof(drupalchat_messages.buddylist)!=="undefined") {
        jQuery.each(drupalchat_messages.buddylist, function(key, value) {
          if (key != 'total') {
            if (key != Drupal.settings.drupalchat.uid) {
              var output = '<li class="status-' + value.status + '">';
            if(Drupal.settings.drupalchat.iup == "1") {
              output = output + '<img style="margin:0px 8px 0px 3px;padding:2px;float:left;height:24px;width:24px;" src="'+value.p+'">';
            }
            output = output + '<a class="' + key + '" href="#">' + value.name + '</a></li>';
              jQuery('#chatpanel .subpanel ul').append(output);
              Drupal.drupalchat.changeStatus('chatbox_'+key,1);
            }
          }
          else {
            jQuery('#chatpanel .online-count').html(value);
            if(value == 0) {
              jQuery('#chatpanel .subpanel ul').append(Drupal.settings.drupalchat.noUsers);
            }
          }
        });
        jQuery('#chatpanel .subpanel ul li:last-child').addClass('last');
  	  
        /*if (jQuery('#chatpanel .subpanel ul li').length <= 0) {
          jQuery('#chatpanel .subpanel ul').append(Drupal.settings.drupalchat.noUsers);
        }*/
        
        //Update Timestamp.
        drupalchat.last_timestamp = drupalchat_messages.last_timestamp;
      }        
  	}
  	//if (Drupal.settings.drupalchat.polling_method != '0') {
  		chatPoll();
  	//}
  }

  function closeChatBox(chatboxtitle) {
    updateLocalChatWindowList(chatboxtitle, 'close');
  	jQuery('#chatbox_'+chatboxtitle).css('display','none');
  }
  
  function updateLocalChatWindowList(chatboxtitle, state) {
    if(state === 'min') {
      drupalchat.open_chat_uids[chatboxtitle] = chatboxtitle;
      delete drupalchat.current_open_chat_window_ids[chatboxtitle];
    }
    else if(state === 'open') {
      drupalchat.open_chat_uids[chatboxtitle] = chatboxtitle;
      drupalchat.current_open_chat_window_ids[chatboxtitle] = chatboxtitle;
    }
    else {
      delete drupalchat.open_chat_uids[chatboxtitle];
      delete drupalchat.current_open_chat_window_ids[chatboxtitle];
    }
    var temp_open_chat_uids = [];
    for(var e in drupalchat.open_chat_uids) {
      temp_open_chat_uids.push(drupalchat.open_chat_uids[e]);
    }
    if(temp_open_chat_uids.join(",")!=='') {
      Drupal.drupalchat.Cookie.create('drupalchat_open_chat_uids', temp_open_chat_uids.join(","));
    }
    else {
      Drupal.drupalchat.Cookie.delete('drupalchat_open_chat_uids');
    }
    
    var temp_open_chat_window_uids = [];
    for(var e in drupalchat.current_open_chat_window_ids) {
      temp_open_chat_window_uids.push(drupalchat.current_open_chat_window_ids[e]);
    }
    if(temp_open_chat_window_uids.join(",")!=='') {
      Drupal.drupalchat.Cookie.create('drupalchat_current_open_chat_window_ids', temp_open_chat_window_uids.join(","));
    }
    else {
      Drupal.drupalchat.Cookie.delete('drupalchat_current_open_chat_window_ids');
    }
  }
  
  function getThreadHistory() {
    var temp_open_chat_uids = [];
    for(var e in drupalchat.open_chat_uids) {
      temp_open_chat_uids.push(drupalchat.open_chat_uids[e]);
    }
    
    jQuery.post(Drupal.settings.drupalchat.threadHistoryUrl, { drupalchat_open_chat_uids: temp_open_chat_uids.join(",") }, function(data) {
      var temp_open_ids = {};
      for(var i in drupalchat.current_open_chat_window_ids) {
        temp_open_ids[i] = drupalchat.current_open_chat_window_ids[i];
      }
      processChatData(data, true);
      for(var i in temp_open_ids) {
        chatWith(temp_open_ids[i], '');
      }
    });
  }

  Drupal.drupalchat.Cookie = {};
  /**
   * Create cookie
   */

  Drupal.drupalchat.Cookie.create = function(name, value, minutes, domain) {
    var expires = "";
    var t_domain = "";
    if(typeof(minutes)!=="undefined") {
      var date = new Date();
      date.setTime(date.getTime()+(minutes*60*1000));
      expires = "; expires="+date.toGMTString();
    }
    else {
      var date = new Date();
      date.setTime(date.getTime()+(30*24*60*60*1000));
      expires = "; expires="+date.toGMTString();
    }
    if((typeof(domain)!=="undefined")) {
      t_domain = "; domain=."+domain;
    }
    document.cookie = name+"="+value+expires+t_domain+"; path=/";
  };

  /**
   * Read cookie
   */

  Drupal.drupalchat.Cookie.read = function (name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for(var i=0;i < ca.length;i++) {
      var c = ca[i];
      while (c.charAt(0)==' ') c = c.substring(1,c.length);
      if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
    }
    return null;
  };

  /**
   * Delete cookie
   */

  Drupal.drupalchat.Cookie.delete = function (name) {
    Drupal.drupalchat.Cookie.create(name,"",-1);
  };
  
  
  //})(jQuery);

}

})();
