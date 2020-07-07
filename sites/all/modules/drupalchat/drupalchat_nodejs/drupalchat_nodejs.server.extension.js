/**
 * DrupalChat server extension for nodejs
 *
 * Add this extension name to the "extensions" in node.config.js
 */

var publishMessageToClient;
var drupalchat_users = {};
var drupalchat_names = {};
var ruser = 0;

exports.setup = function (config) {
  publishMessageToClient = config.publishMessageToClient;

  process.on('client-authenticated', function (sessionId, authData) {
    console.log('Auth - ' + authData.uid);
    //get others info
    if(authData.uid != 0) {
      for (var user in drupalchat_users) {
        if(authData.uid != drupalchat_users[user] && drupalchat_names[user]) {
          console.log('Getting - ' + user + ' with name - ' + drupalchat_names[user] + ', for ' + authData.uid);
          publishMessageToClient(sessionId, {type: 'userOnline', data: {uid: user, name: drupalchat_names[user]}, callback: 'drupalchatNodejsMessageHandler'});
        }
      }
      //create own channel
      publishMessageToClient(sessionId, {type: 'createChannel', data: 'create', callback: 'drupalchatNodejsMessageHandler'});
      //add me
      console.log('Added - ' + authData.uid);
      drupalchat_users[authData.uid] = sessionId;
    }
  })
  .on('message-published', function (message, i) {
    console.log('Msg - ' + message.type);
    if(message.type == 'sendName') {
      var data = JSON.parse(message.data);
      
      //update name
      console.log('Name ' + data.name);
      drupalchat_names[data.uid] = data.name;
      //send to others
      for (var user in drupalchat_users) {
        if(user != data.uid) {
          console.log('Sending - ' + data.uid + ' to ' + user);
          publishMessageToClient(drupalchat_users[user], {type: 'userOnline', data: {uid: data.uid, name: data.name}, callback: 'drupalchatNodejsMessageHandler'});
        }
      }
    }
  })
  .on('client-disconnect', function (sessionId) {
    for (var user in drupalchat_users) {
        if(drupalchat_users[user] == sessionId) {
          ruser = user;
          break;
        }
    }
    console.log('Out - ' + ruser);
    delete drupalchat_users[ruser];
    if(ruser!=0) {
      for (var user in drupalchat_users) {
        if(drupalchat_users[user] != sessionId) 
          publishMessageToClient(drupalchat_users[user], {type: 'userOffline', data: ruser, callback: 'drupalchatNodejsMessageHandler'});
      }
    }
  });
};

