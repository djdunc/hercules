/*!
   @file DFRobot_mmWave_Radar.ino
   @ Detect if there is object or human motion in measuring range; Allows for configuring sensing area, sensor output delay, and resetting sensor to factory settings.
   @n Experimental phenomenon: When the sensor is enabled, print 0 or 1 on the serial port: 0 for no motion detected in the sensing area, 1 for object/human movement detected. 
   @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @licence     The MIT License (MIT)
   @version  V1.0
   @date  2023-3-13
   @https://github.com/DFRobot
*/

#include <SoftwareSerial.h>
#include <DFRobot_mmWave_Radar.h>

int LED_BLINK = 2;

SoftwareSerial mySerial(D1, D2); //rx,tx
DFRobot_mmWave_Radar sensor(&mySerial);

void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);
  pinMode(LED_BLINK, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(4000);
  Serial.println("Setup");

  sensor.factoryReset();       //Reset to factory settings
  sensor.DetRangeCfg(0, 3);    //Set sensing distance, up to 9m 
  sensor.OutputLatency(0, 0);  //Set output delay 
  
  Serial.print("Done");

}

void loop() {
  int presence = sensor.readPresenceDetection();
  //int presence = digitalRead(D4);
  digitalWrite(LED_BUILTIN, !presence); // need the opposite since low = led on!
  Serial.print("presence:");
  Serial.println(presence);
  delay(1000);
}
