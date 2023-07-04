
void startWifi(){
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
    
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // check to see if connected and wait until you are
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(600);
    Serial.print(".");
    blinkRGB(200, 2, 20, 150, 150);
    counter++;
    if(counter > 100){
      NVIC_SystemReset();   // Perform a system reset after 100*600 = 60seconds
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  blinkRGB(200, 5, 0, 200, 0);
}

// This function is used to make sure the arduino is connected
// to an MQTT broker before it tries to send a message and to 
// keep alive subscriptions on the broker (ie listens for inTopic)

void reconnectMQTT() {
  if (WiFi.status() != WL_CONNECTED){
    startWifi();
  } else {
    //Serial.println(WiFi.localIP());
  }
  // Loop until we're reconnected
  while (!mqttClient.connected()) {    // while not (!) connected....
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "BX1";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      // ... and subscribe to messages on broker
      mqttClient.subscribe("personal/ucjtdjw/moorfields/in/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


// The callback function is called when an incoming message is received
// from the MQTT broker (ie the inTopic message)/ In this demo if the first
// character of the message has the value "1" we turn an LED on. Any other value 
// results in the LED being turned off

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  toggleRGB();
}


