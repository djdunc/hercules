#define SoundSensorPin A1         // this pin read the analog voltage from the sound level meter
#define VREF  3.3                 // voltage on AREF pin,default:operating voltage


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Sound
  float voltageValue;
  voltageValue = analogRead(SoundSensorPin) / 1023.0 * VREF;
  float sound = voltageValue * 50.0;  //convert voltage to decibel value
  Serial.print(sound,1);
  Serial.println(" dBA");
}
