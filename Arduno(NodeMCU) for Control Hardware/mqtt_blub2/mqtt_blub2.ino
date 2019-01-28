#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Update these with values suitable for your network.

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";
int count = 0;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  digitalWrite(12,HIGH); 
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
  Serial.println(command);
  check_function(Newtopic,command);
  Serial.println();
}

void check_function(String Newtopic,String command){
    Serial.println("Check function");
    if(command == "Bulb/2/true"){ // Blub_2_true
      Serial.print("Bulb 2 is on!\n");
      delay(500);
      digitalWrite(13, LOW);
      client.publish("Thing_Bulb/2/Confirm","true");
      }   
    else if(command == "Bulb/2/false"){ // Blub_2_false
      Serial.print("Bulb 2 is off!\n");
      delay(500);
      digitalWrite(13, HIGH);
      client.publish("Thing_Bulb/2/Confirm","false");
      } // end if&else fan_acc
}

void reconnect() {
  // Loop until we're reconnected
  digitalWrite(12,LOW); 
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Blub_2")) {
      Serial.println("connected");
      client.subscribe("Hub_Home");
      
      digitalWrite(12,HIGH);
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
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(12,LOW); 
      delay(500);
      Serial.print(".");
    }
    if(count==0){
      client.publish("Message_from_thing","Blub 2 ready!");
      count++;
      }
    client.loop(); 
    digitalWrite(14,HIGH);
}

