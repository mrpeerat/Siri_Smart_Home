#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Update these with values suitable for your network.
int inputPin = 14;
const char* ssid = "PingWiFi";
const char* password = "ping123456789";
const char* mqtt_server = "jamesmooth.win";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  pinMode(inputPin, INPUT);
  pinMode(14,OUTPUT); 
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
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Blub_1")) {
      Serial.println("connected");
      //client.subscribe("Blub/1/");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
// the loop function runs over and over again forever
void loop() {
  if (!client.connected()) {
    reconnect();
    }
  client.loop(); 
  int value= digitalRead(inputPin);
  Serial.print(value);
  Serial.print("\n");
  if(value == HIGH){
    Serial.print("Sensor Detected!\n");
    client.publish("PIR_sensor/1/status", "true");
    client.publish("HubHome", "Bulb/1/true");
    delay(2000);
    client.publish("HubHome", "Bulb/2/true");
    client.publish("Camera_1", "take");
    delay(20000);
    client.publish("PIR_sensor/2/status", "false");
  }
  delay(500);
}
