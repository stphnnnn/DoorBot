require('dotenv').config();

var Botkit = require('botkit');
var express = require('express');
var app = express();

var controller = Botkit.slackbot({
  debug: false,
  retry: 10
});

var malebot = controller.spawn({
  token: process.env.MALETOKEN
}).startRTM()

app.get('/open', function (req, res) {
  console.log('Toilet free')
  res.sendStatus(200);
});

app.get('/close', function (req, res) {
  console.log('Toilet busy')
  res.sendStatus(200);
});

app.listen(3000);
