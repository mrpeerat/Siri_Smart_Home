#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN 12
// Update these with values suitable for your network.
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
// Update these with values suitable for your network.

const char* ssid = "PingWiFi";
const char* password = "";
const char* mqtt_server = "";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50],msg2[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String Newtopic = String(topic);
  payload[length] = '\0';
  String command = String((char*)payload);
  check_function(Newtopic,command);
  Serial.println();

}

void check_function(String Newtopic,String command){
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    client.publish("Temp_sensor/1/status", String(t).c_str());
    client.publish("Humi_sensor/1/status", String(h).c_str());
    Serial.print("TemandHumid is send!");
    if(command == "Blub/1/true"){ // Blub_1_true
        client.publish("Blub/1/", "true");
      }   
    else if(command == "Blub/1/false"){ // Blub_1_false
        client.publish("Blub/1/", "false"); 
      } // end if&else fan_acc
    else if(command == "Blub/2/true"){ // Blub_2_true
        client.publish("Blub/2/", "true");  
      } // end if&else light_acc
    else if(command == "Blub/2/false"){ // Blub_2_false
        client.publish("Blub/2/", "false");    
      }
    else if(command == "Plug/1/true"){ // Plug_1_true
        client.publish("Plug/1/", "true");   
      } // end if&else lock_acc
    else if(command == "Plug/1/false"){ // Plug_1_false
        client.publish("Plug/1/", "false");   
      }
    else if(command == "Plug/2/true"){ // Plug_2_true
        client.publish("Plug/2/", "true");    
      } // end if&else lock_acc
    else if(command == "Plug/2/false"){ // Plug_2_false
        client.publish("Plug/2/", "false");   
      }
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("PingESP")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    //  client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("Arduino_Home");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  }

