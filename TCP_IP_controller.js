var mqtt = require('mqtt')
var net = require('net');
var client_mqtt  = mqtt.connect('mqtt://www.jamesmooth.win')
var server = net.createServer();  
var text = "";
server.on('connection', handleConnection);

function handleConnection(conn) {  
  var remoteAddress = conn.remoteAddress + ':' + conn.remotePort;
  console.log('new client connection from %s', remoteAddress);

  conn.on('data', onConnData);
  conn.once('close', onConnClose);
  conn.on('error', onConnError);

  setInterval(function () { 
    conn.write(text)
    //console.log(text)
  }, 2000); 

  function onConnData(d) {
    var messageTCP = d.toString().split("-") //แยกข้อความ
    console.log('connection data from %s: %s', remoteAddress, d);
    //conn.write("1");
    //console.log(messageTCP[0].toString().lenght)
    if(messageTCP[0]=="offline_web"){ //ถ้าสั่งงานแบบออฟไลน์ จะส่งข้อความมา รูปแบบ = offline_web-Blub/1/true
      text=messageTCP[1]
      console.log(text)
    } else {
      console.log(messageTCP)
      client_mqtt.publish(messageTCP[0],messageTCP[1]) //ส่งการคอนเฟิมทั้งหมดไปยังเซิฟ เช่น [0] = หัวข้อที่จะส่งของ mqtt EX. Thing_Blub/1/Confirm [1] = คำสั่งเปิด/ปิด Ex. ข้อความคือ true/false 
      console.log("Confirm to "+messageTCP[0]+" Message = "+messageTCP[1])
      text=""
    }
  }

  function onConnClose() { 
    //console.log('connection from %s closed', remoteAddress);
  }

  function onConnError(err) {
    //console.log('Connection %s error: %s', remoteAddress, err.message);
  }
}

server.listen(8000, function() {  //ประกาศใช้งาน server
  console.log('server listening to %j', server.address());
});

client_mqtt.on('connect', function () {
  client_mqtt.subscribe('Hub_Home') // เป็นหัวข้อเดียวที่จะต้อง sub 
  client_mqtt.publish('Hub_Home_start', 'Ready') //บอกในเซิฟว่าhub พร้อมใช้งาน ให้รีเซ้ตค่าใน db เป็น off ให้หมด
})
 
client_mqtt.on('message', function (topic, message) {
  // message is Buffer
  console.log(message.toString()) // แสดงข้อความจาก mqtt
  text = message.toString()
})


