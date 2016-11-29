require('dotenv').config();

var Botkit = require('botkit');

var controller = Botkit.slackbot({
  debug: false,
  retry: 10
});

var bot = controller.spawn({
  token: process.env.MALETOKEN
});

function start_rtm() {
  bot.startRTM(function(err,bot,payload) {
    if (err) {
      console.log('Failed to start RTM')
      return setTimeout(start_rtm, 60000);
    }
    console.log("RTM started!");
  });
}

controller.on('rtm_close', function(bot, err) {
  start_rtm();
});

start_rtm();
