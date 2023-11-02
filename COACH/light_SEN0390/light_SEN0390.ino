/*!
 * Testing on MKR1010
 * Duncan Wilson - June 2023
 */

#include <DFRobot_B_LUX_V30B.h>
DFRobot_B_LUX_V30B    myLux(13,7,8);//The sensor chip is set to 13 pins, SCL and SDA adopt default configuration

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("myLux.begin");
  myLux.begin();
}

void loop() {
  Serial.print("Light: ");
  Serial.print(myLux.lightStrengthLux());
  delay(1000);
}
