/*!
 * Testing on MKR1010
 * Duncan Wilson - June 2023

 * Light Sensor code based on sample at https://github.com/DFRobot/DFRobot_B_LUX_V30B 
 * DFRobot Gravity: Analog Sound Level Meter https://wiki.dfrobot.com/Gravity__Analog_Sound_Level_Meter_SKU_SEN0232
 * DFRobot Gravity: CO2 board - for 3.3v boards and v1.x sensors see: https://www.dfrobot.com/forum/topic/315493 
 * mmWave - using high low putput from GPIO2
 * HDC1080 - based on closedcubed library

 * Connections - GND and 5V to all sensors plus:
 * light - SCL pin 7 and SDA pin 8
 * sound - A1
 * CO2 - A2
 * mmWave - GPIO2 on sensor to D5 on MKR1010
 * TMP36 on A0 
 */

// set up connectivity
#include <WiFiNINA.h>           // wifi library for the MKR1010
#include <utility/wifi_drv.h>   // library to drive to RGB LED on the MKR1010
#include <PubSubClient.h>       // library for sending MQTT messages
#include "arduino_secrets.h"    // wifi and mqtt user / pass
#include <Wire.h>               // used for HDC1080 temp / hum sensor
#include "ClosedCube_HDC1080.h"

// set up sensors
#include <DFRobot_B_LUX_V30B.h>
DFRobot_B_LUX_V30B    myLux(13,7,8);  // The sensor chip is set to 13 pins, SCL and SDA adopt default configuration
#define SoundSensorPin A1         // this pin read the analog voltage from the sound level meter
#define VREF  3.3                 // voltage on AREF pin,default:operating voltage
#define CO2SensorPin A2           // this pin read the analog voltage from the sound level meter
#define PresencePin  5           // pin for mmWave sensor (note had conflicts when using D6 with wifinina)
ClosedCube_HDC1080 hdc1080;       // create HDC1080 sensor


// EDITABLE per sensor unit -- start
const int numReadings = 60;       // we take a measure every second, and send the average after this duration
const char topic[] = "personal/ucjtdjw/moorfields/bx1/";
// EDITABLE per sensor unit -- end


int presence = 0;
float sound = 0;
float ppm = 0;
float lux = 0;
float tmp = 0;
float hum = 0;
float avgpresence = 0;
float avgsound = 0;
float avgppm = 0;
float avglux = 0;
float avgtmp = 0;
float avghum = 0;


// Setup wifi / MQTT variables
char ssid[]    = SECRET_SSID;
char password[] = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

WiFiServer server(80);
WiFiClient wificlient;

const char* mqtt_server = "mqtt.cetools.org";
WiFiClient mkrClient;
PubSubClient mqttClient(mkrClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// variables for data capture
const int numberofsensors = 6;
float readings[numberofsensors][numReadings];  // Array to store the sensor readings 
int indx = 0;              // Current index in the array
float totals[numberofsensors];              // Sum of all the readings


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);  
  delay(1000);                     // give time for the serial connections to open
  Serial.println("Coach v3");

  Wire.begin();

  myLux.begin();
  delay(200);
  pinMode(PresencePin, INPUT);
	// Default settings: 
	//  - Heater off
	//  - 14 bit Temperature and Humidity Measurement Resolutions
	hdc1080.begin(0x40);

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED off 

  // RGB LED's
  WiFiDrv::pinMode(25, OUTPUT); // G
  WiFiDrv::pinMode(26, OUTPUT); // R
  WiFiDrv::pinMode(27, OUTPUT); // B  

  toggleRGB();
  
  // Initiate the connecting to wifi routine
  delay(2000); // wait a second before starting wifi
  WiFi.setHostname("BX1");
  startWifi();

  // Once connected to wifi establish connection to mqtt broker
  // personal/ucjtdjw/moorfields/
  mqttClient.setServer(mqtt_server, 1884);
  mqttClient.setCallback(callback);



  // Initialize the array to hold sensor readings with all zeros
  for (int i = 0; i < numberofsensors; i++) {
    totals[i] = 0.0;
    for (int j = 0; j < numReadings; j++) {
      readings[i][j] = 0.0;
    }
  }

}

void loop() {

  readSensors(0); // pass in 0 for no serial print or 1 to see readings
  //printSensorValues();
  sendAvgMQTT(1);  // pass in 0 for no serial print or 1 to see readings
  
  // it takes roughly 500ms to read all sensors so delay another 500 to make 1 sec cycle
  delay(500); 

}
