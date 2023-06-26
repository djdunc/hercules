/*!
 * Testing on MKR1010
 * Duncan Wilson - June 2023
 */

#include <DFRobot_B_LUX_V30B.h>
#include <Wire.h>               // used for HDC1080 temp / hum sensor
#include "Adafruit_HDC1000.h"
DFRobot_B_LUX_V30B    myLux(13,7,8);//The sensor chip is set to 13 pins, SCL and SDA adopt default configuration
Adafruit_HDC1000 hdc = Adafruit_HDC1000();

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
  delay(1000);
}
