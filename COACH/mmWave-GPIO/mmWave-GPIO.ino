#include <WiFiNINA.h>           // wifi library for the MKR1010
#include <utility/wifi_drv.h>   // library to drive to RGB LED on the MKR1010

#define PresencePin  5

int presence = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PresencePin, INPUT);  

  // RGB LED's
  WiFiDrv::pinMode(25, OUTPUT); // G
  WiFiDrv::pinMode(26, OUTPUT); // R
  WiFiDrv::pinMode(27, OUTPUT); // B  

  blinkRGB(100,10,0,0,200);

}

void loop() {
  // put your main code here, to run repeatedly:
  presence = digitalRead(PresencePin);

  Serial.print("Presence State: ");
  Serial.println(presence);
  if(presence){
    blinkRGB(100,10,0,200,0);
  } else {
    blinkRGB(100,10,200,0,0);
  }

  //delay(1000);
}


void blinkRGB(int ms, int loops, int r, int g, int b){
  while(loops){
    WiFiDrv::analogWrite(25, r);
    WiFiDrv::analogWrite(26, g);
    WiFiDrv::analogWrite(27, b);
    delay(ms);
    WiFiDrv::analogWrite(25, 0);
    WiFiDrv::analogWrite(26, 0);
    WiFiDrv::analogWrite(27, 0);
    delay(ms);
    loops-=1;   
  }

}