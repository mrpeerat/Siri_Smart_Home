var firebase = require('firebase');

var app = firebase.initializeApp({ apiKey: "AIzaSyASKfcXK48XGwRbsSLzmQVwsEs7tj8Z-bQ",
authDomain: "nodejsmqttfirebase.firebaseapp.com",
databaseURL: "https://nodejsmqttfirebase.firebaseio.com",
projectId: "nodejsmqttfirebase",
storageBucket: "nodejsmqttfirebase.appspot.com",
messagingSenderId: "179975127878"});

db =app.database()

exports = db