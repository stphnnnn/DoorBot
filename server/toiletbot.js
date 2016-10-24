require('dotenv').config();

var Botkit = require('botkit');
var express = require('express');
var app = express();

var malecontroller = Botkit.slackbot();
var femalecontroller = Botkit.slackbot();

var malebot = malecontroller.spawn({
  token: process.env.MALE
}).startRTM()

var femalebot = femalecontroller.spawn({
  token: process.env.FEMALE
}).startRTM()

app.get('/male/open', function (req, res) {
  malebot.startRTM();
  res.send('Male toilet free');
});

app.get('/male/close', function (req, res) {
  malebot.closeRTM();
  res.send('Male toilet busy');
});

app.get('/female/open', function (req, res) {
  femalebot.startRTM();
  res.send('Female toilet free');
});

app.get('/female/close', function (req, res) {
  femalebot.closeRTM();
  res.send('Female toilet busy');
});

app.listen(3000);
