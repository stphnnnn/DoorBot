require('dotenv').config();

var Botkit = require('botkit');

var malebotID;

var controller = Botkit.slackbot({
  debug: false,
  retry: 10
});

var malebot = controller.spawn({
  token: process.env.MALETOKEN
});

function start_rtm() {
  malebot.startRTM(function(err, malebot, payload) {
    if (err) {
      console.log('Failed to start RTM')
      return setTimeout(start_rtm, 60000);
    }
    console.log("RTM started!");
    malebotID = malebot.identifyBot().id;
  });
}

controller.on('rtm_close', function(malebot, err) {
  start_rtm();
});

start_rtm();

controller.on('presence_change', function(malebot, res, err) {
  if (res.user == malebotID) {
    console.log(res.presence);
  }
});
