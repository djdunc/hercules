/*!
 * Testing on MKR1010
 * Duncan Wilson - June 2023
 */

#include <DFRobot_B_LUX_V30B.h>
#include <Wire.h>               // used for HDC1080 temp / hum sensor
#include "Adafruit_HDC1000.h"
DFRobot_B_LUX_V30B    myLux(13,7,8);//The sensor chip is set to 13 pins, SCL and SDA adopt default configuration
Adafruit_HDC1000 hdc = Adafruit_HDC1000();
#define CO2SensorPin A2           // this pin read the analog voltage from the sound level meter
float ppm = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("myLux.begin");
  myLux.begin();
  Serial.println("hdc.begin");
  if (!hdc.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
}

void loop() {
  Serial.print("Light: ");
  Serial.print(myLux.lightStrengthLux());
  Serial.print(" - Temperature: ");
  Serial.print(hdc.readTemperature());
  Serial.print(" - Humidity: ");
  Serial.println(hdc.readHumidity());

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
    int voltage_difference=voltage-400;
    ppm=voltage_difference*50.0/16.0;

    Serial.print("voltage: ");
    Serial.print(voltage);
    Serial.print("mv - ");
    //Print CO2 concentration
    Serial.print("CO2 Concentration: ");
    Serial.print(ppm);
    Serial.println("ppm");

  }  

  delay(1000);
}
