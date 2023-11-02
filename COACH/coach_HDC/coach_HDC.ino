/*!
 * Testing on MKR1010
 * Duncan Wilson - June 2023
 */

#include <Wire.h>               // used for HDC1080 temp / hum sensor
#include "Adafruit_HDC1000.h"
Adafruit_HDC1000 hdc = Adafruit_HDC1000();

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("hdc.begin");
  if (!hdc.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
}

void loop() {
  Serial.print(" - Temperature: ");
  Serial.print(hdc.readTemperature());
  Serial.print(" - Humidity: ");
  Serial.println(hdc.readHumidity());

  delay(1000);
}
