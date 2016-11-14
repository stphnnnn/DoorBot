require('dotenv').config();

var Botkit = require('botkit');

var controller = Botkit.slackbot({
  debug: false,
  retry: 10
});

var malebot = controller.spawn({
  token: process.env.MALETOKEN
}).startRTM()
