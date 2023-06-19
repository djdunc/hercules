/*!
 * Testing on MKR1010
 * Duncan Wilson - June 2023

 * Light Sensor code based on sample at https://github.com/DFRobot/DFRobot_B_LUX_V30B 
 * DFRobot Gravity: Analog Sound Level Meter https://wiki.dfrobot.com/Gravity__Analog_Sound_Level_Meter_SKU_SEN0232
 * DFRobot Gravity: CO2 board - for 3.3v boards and v1.x sensors see: https://www.dfrobot.com/forum/topic/315493 
 * mmWave - using high low putput from GPIO2
 * HDC1080 - based on closedcubed library

 * Connections - GND and 5V to all sensors plus:
 * light - SCL and SDA (0x4A)
 * sound - A1
 * CO2 - A2
 * mmWave - GPIO2 on sensor to D6 on MKR1010
 * TMP36 on A0
 * Not used - HDC1080 - SCL and SDA (0x40)

 */

#include <DFRobot_B_LUX_V30B.h>
//#include <Wire.h>               // used for HDC1080 temp / hum sensor
//#include "ClosedCube_HDC1080.h"

DFRobot_B_LUX_V30B    myLux(13);//The sensor chip is set to 13 pins, SCL and SDA adopt default configuration
#define SoundSensorPin A1  //this pin read the analog voltage from the sound level meter
#define VREF  3.3  //voltage on AREF pin,default:operating voltage
#define CO2SensorPin A2  //this pin read the analog voltage from the sound level meter
#define PresencePin 7
#define tmp36Pin A0

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);  

  myLux.begin();
  pinMode(PresencePin, INPUT);
}

void loop() {

  // Light
  Serial.print("value: ");
  Serial.print(myLux.lightStrengthLux());
  Serial.println(" (lux).");

  // Sound
  float voltageValue,dbValue;
  voltageValue = analogRead(SoundSensorPin) / 1023.0 * VREF;
  dbValue = voltageValue * 50.0;  //convert voltage to decibel value
  Serial.print(dbValue,1);
  Serial.println(" dBA");

  // CO2
  int sensorValue = analogRead(CO2SensorPin);
  float voltage = sensorValue*(3300/1023.0);
  if(voltage == 0)
  {
    Serial.println("Fault");
  }
  else if(voltage < 400)
  {
    Serial.println("preheating");
  }
  else
  {
    int voltage_diference=voltage-400;
    float concentration=voltage_diference*50.0/16.0;
    // Print Voltage
    Serial.print("voltage: ");
    Serial.print(voltage);
    Serial.print("mv - ");
    //Print CO2 concentration
    Serial.print("CO2 Concentration: ");
    Serial.print(concentration);
    Serial.println("ppm");
  }  

  // Presence
  int PresenceState = digitalRead(PresencePin);
  Serial.print("Presence State: ");
  Serial.println(PresenceState);

/* This had a conflict with the lux library - Serial1 was always not available

  // Presence 
  if (Serial1.available()) {     // Check if there is data available on Serial1
    String receivedString = Serial1.readStringUntil('\n');   // Read the string until newline character
    Serial.println("Received String: " + receivedString);   // Print the received string to Serial monitor
  } else {
    Serial.println("no serial");
  }
*/

  // Temperature
  // Get the voltage reading from the TMP36
  int reading = analogRead(tmp36Pin);
  float tmp36voltage = reading * (3.3 / 1024.0);
  // Convert the voltage into the temperature in Celsius
  float tmp = (tmp36voltage - 0.5) * 100;
  Serial.print("Temperature: ");
  Serial.println(tmp);

  delay(1000);
}
