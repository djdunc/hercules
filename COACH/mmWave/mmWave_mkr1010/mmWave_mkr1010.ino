// RX and TX on the mmWave sensor connected to pins 13/14 on MKR1010
// these are the standard hardware serial on the board
// and can be queried via Serial1

#include <WiFiNINA.h>           // wifi library for the MKR1010
#include <utility/wifi_drv.h>   // library to drive to RGB LED on the MKR1010

#include <DFRobot_mmWave_Radar.h>

DFRobot_mmWave_Radar sensor(&Serial1);


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  // RGB LED's
  WiFiDrv::pinMode(25, OUTPUT); // G
  WiFiDrv::pinMode(26, OUTPUT); // R
  WiFiDrv::pinMode(27, OUTPUT); // B  

  glowRed(100);
  delay(1000);

  sensor.factoryReset();     //Restore to the factory settings
  sensor.DetRangeCfg(0, 3);  //The detection range is as far as 9m
  sensor.OutputLatency(0, 0);

  glowBlue(100);
}

void loop() {

  int val = sensor.readPresenceDetection();
  if(val){
    glowGreen(100);
   } else {
    glowRed(100);
  }

  Serial.println(val);
}
 
void glowRed(int loops){
  while(loops){
    float step = millis()/1000.0;
    int value = 128.0 + 128 * sin( step * 2.0 * PI );
    WiFiDrv::analogWrite(25, value);
    WiFiDrv::analogWrite(26, 0);
    WiFiDrv::analogWrite(27, 0);
    delay(10);
    loops-=1;   
  }
}
 
void glowGreen(int loops){
  while(loops){
    float step = millis()/1000.0;
    int value = 128.0 + 128 * sin( step * 2.0 * PI );
    WiFiDrv::analogWrite(25, 0);
    WiFiDrv::analogWrite(26, value);
    WiFiDrv::analogWrite(27, 0);
    delay(10);
    loops-=1;   
  }
}

void glowBlue(int loops){
  while(loops){
    float step = millis()/1000.0;
    int value = 128.0 + 128 * sin( step * 2.0 * PI );
    WiFiDrv::analogWrite(25, 0);
    WiFiDrv::analogWrite(26, 0);
    WiFiDrv::analogWrite(27, value);
    delay(10);
    loops-=1;   
  }
}