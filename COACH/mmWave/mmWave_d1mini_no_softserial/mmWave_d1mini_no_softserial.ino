#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wifi and MQTT
#include "arduino_secrets.h" 
/*
**** please enter your sensitive data in the Secret tab/arduino_secrets.h
**** using format below
#define SECRET_SSID "ssid name"
#define SECRET_PASS "ssid password"
#define SECRET_MQTTUSER "user name - eg student"
#define SECRET_MQTTPASS "password";
 */

const char* ssid     = SECRET_SSID;
const char* pass     = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const char* mqtt_server = "mqtt.cetools.org";
int        port     = 1884;


void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);

  // Initiate the connecting to wifi routine
  startWifi();

  // Once connected to wifi establish connection to mqtt broker
  client.setServer(mqtt_server, port);
  
}

void loop() {
  int presence = digitalRead(D0);
  digitalWrite(LED_BUILTIN, !presence); // need the opposite since low = led on!
  Serial.print("presence:");
  Serial.println(presence);
  if(WiFi.status() != WL_CONNECTED){
    startWifi();
  } else {
    Serial.println("wifi ok");
  }
  sendMQTT(presence);
  delay(1000);
}


// This function is used to set-up the connection to the wifi
// using the user and passwords defined in the secrets file
// It then prints the connection status to the serial monitor

void startWifi(){
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  // check to see if connected and wait until you are
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// This function is used to make sure the arduino is connected
// to an MQTT broker before it tries to send a message and to 
// keep alive subscriptions on the broker (ie listens for inTopic)

void reconnect() {
  // Try 5 times to reconnect
  int counter = 0;
  while (counter < 5) {    // while not (!) connected....
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266D1Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      break;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    counter++;
  }
  Serial.println("Didnt manage to reconnect, checking wifi...");
}

// This function sends (publishes) a message to a MQTT topic
// once a connection is established with the broker. It sends
// an incrementing variable called value to the topic:
// "student/CASA0014/plant/ucjtdjw"

void sendMQTT(int val) {

  if (!client.connected()) {
    reconnect();
  }else{
    client.loop();
    snprintf (msg, 2, "%ld", val);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("personal/ucjtdjw/mmwave", msg);
  }

}
