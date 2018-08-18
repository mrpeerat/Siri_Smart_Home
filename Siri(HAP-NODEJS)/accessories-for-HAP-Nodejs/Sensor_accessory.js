var Accessory = require('../').Accessory;
var Service = require('../').Service;
var Characteristic = require('../').Characteristic;
var uuid = require('../').uuid;
var mqtt = require('mqtt')
var client  = mqtt.connect('mqtt://...... your MQTT Server')
temp = 0
hu = 0

client.on('connect', function () {
  client.subscribe('Temp_sensor/1/status')
  client.subscribe('Humi_sensor/1/status')
  //client.publish('pinghome_siri', 'Outlet is Ready !')
})

client.on('message', function (topic, message) {
  // message is Buffer
  console.log(message.toString())
  var mes = message.toString();
  if(topic == "Temp_sensor/1/status"){
    SENSOR.currentTemperature = parseInt(mes)
  }
  else{
    SENSOR.currentHumidity = parseInt(mes)
  }
}) // end call back mqtt function


var SENSOR = {
  currentTemperature: temp,
  currentHumidity: hu,
  getTemperature: function() { 
    console.log("Getting the current temperature!");
    return SENSOR.currentTemperature;
  },
  getHumidity: function() { 
    console.log("Getting the current humidity!");
    return SENSOR.currentHumidity;
  }
}


// Generate a consistent UUID for our Temperature Sensor Accessory that will remain the same
// even when restarting our server. We use the `uuid.generate` helper function to create
// a deterministic UUID based on an arbitrary "namespace" and the string "sensor".
var sensorUUID = uuid.generate('hap-nodejs:accessories:sensor');

// This is the Accessory that we'll return to HAP-NodeJS that represents our fake lock.
var sensor = exports.accessory = new Accessory('Sensor', sensorUUID);

// Add properties for publishing (in case we're using Core.js and not BridgedCore.js)
sensor.username = "C4:5D:1A:1E:52:FA";
sensor.pincode = "031-45-154";

// Add the actual TemperatureSensor Service.
// We can see the complete list of Services and Characteristics in `lib/gen/HomeKitTypes.js`
sensor
  .addService(Service.TemperatureSensor)
  .getCharacteristic(Characteristic.CurrentTemperature)
  .on('get', function(callback) {
    
    // return our current value
    callback(null, SENSOR.getTemperature());
  });

// Add the Humidity Service
sensor
  .addService(Service.HumiditySensor)
  .getCharacteristic(Characteristic.CurrentRelativeHumidity)
  .on('get', function(callback) {

    // return our current value
    callback(null, SENSOR.getHumidity());
  });

// update readings every 10 seconds
setInterval(function() {

  // update the characteristic value so interested iOS devices can get notified
  sensor
    .getService(Service.TemperatureSensor)
    .setCharacteristic(Characteristic.CurrentTemperature, SENSOR.currentTemperature);
  
  sensor
    .getService(Service.HumiditySensor)
    .setCharacteristic(Characteristic.CurrentRelativeHumidity, SENSOR.currentHumidity);
  
}, 1);
