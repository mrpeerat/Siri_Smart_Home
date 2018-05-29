var Accessory = require('../').Accessory;
var Service = require('../').Service;
var Characteristic = require('../').Characteristic;
var uuid = require('../').uuid;
var mqtt = require('mqtt')
var client  = mqtt.connect('mqtt://jamesmooth.win:8000s')
var err = null; // in case there were any problems
client.on('connect', function () {
  client.subscribe('Thing_Plug/1/Confirm')
  //client.publish('pinghome_siri', 'Outlet is Ready !')
})

client.on('message', function (topic, message) {
  // message is Buffer
  console.log(message.toString())
  var mes = message.toString();
  //turn on the outlet
  if(mes === 'true'){
    
      outlet
         .getService(Service.Outlet)
         .getCharacteristic(Characteristic.On)
         .updateValue(true);
    
  } // end if turn on the outlet

   //turn off the outlet
   else {
    
    outlet
       .getService(Service.Outlet)
       .getCharacteristic(Characteristic.On)
       .updateValue(false);
  }//end else if turn off the outlet
}) // end call back mqtt function

// here's a fake hardware device that we'll expose to HomeKit
var FAKE_OUTLET = {
    setPowerOn: function(on) {
    console.log("Turning the outlet %s!...", on ? "on" : "off");
    if (on) {
          FAKE_OUTLET.powerOn = true;
          //client.publish('c_outlet_acc', 'Outlet is turn on !')
          client.publish('Thing_Plug/1/Order', 'Plug/1/true')
          if(err) { return console.log(err); }
          console.log("...outlet is now on.");
    } else {
          FAKE_OUTLET.powerOn = false;
          //client.publish('c_outlet_acc', 'Outlet is turn off !')
          client.publish('Thing_Plug/1/Order', 'Plug/1/false')
          if(err) { return console.log(err); }
          console.log("...outlet is now off.");
    }
  },
    identify: function() {
    console.log("Identify the outlet.");
    }
}

// Generate a consistent UUID for our outlet Accessory that will remain the same even when
// restarting our server. We use the `uuid.generate` helper function to create a deterministic
// UUID based on an arbitrary "namespace" and the accessory name.
var outletUUID = uuid.generate('hap-nodejs:accessories:Plug_1');

// This is the Accessory that we'll return to HAP-NodeJS that represents our fake light.
var outlet = exports.accessory = new Accessory('Plug_1', outletUUID);

// Add properties for publishing (in case we're using Core.js and not BridgedCore.js)
outlet.username = "1A:2B:3C:4D:5D:FF";
outlet.pincode = "031-45-154";

// set some basic properties (these values are arbitrary and setting them is optional)
outlet
  .getService(Service.AccessoryInformation)
  .setCharacteristic(Characteristic.Manufacturer, "SM")
  .setCharacteristic(Characteristic.Model, "Rev-1")
  .setCharacteristic(Characteristic.SerialNumber, "A2");

// listen for the "identify" event for this Accessory
outlet.on('identify', function(paired, callback) {
  FAKE_OUTLET.identify();
  callback(); // success
});

// Add the actual outlet Service and listen for change events from iOS.
// We can see the complete list of Services and Characteristics in `lib/gen/HomeKitTypes.js`
outlet
  .addService(Service.Outlet, "Plug_1") // services exposed to the user should have "names" like "Fake Light" for us
  .getCharacteristic(Characteristic.On)
  .on('set', function(value, callback) {
    FAKE_OUTLET.setPowerOn(value);
    callback(); // Our fake Outlet is synchronous - this value has been successfully set
  });

// We want to intercept requests for our current power state so we can query the hardware itself instead of
// allowing HAP-NodeJS to return the cached Characteristic.value.
outlet
  .getService(Service.Outlet)
  .getCharacteristic(Characteristic.On)
  .on('get', function(callback) {

    // this event is emitted when you ask Siri directly whether your light is on or not. you might query
    // the light hardware itself to find this out, then call the callback. But if you take longer than a
    // few seconds to respond, Siri will give up.

    var err = null; // in case there were any problems

    if (FAKE_OUTLET.powerOn) {
      console.log("Are we on? Yes.");
      callback(err, true);
    }
    else {
      console.log("Are we on? No.");
      callback(err, false);
    }
  }); 
