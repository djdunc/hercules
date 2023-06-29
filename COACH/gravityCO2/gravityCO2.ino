/*!
 * Testing on MKR1010
 * Duncan Wilson - June 2023
 */


#define CO2SensorPin A2           // this pin read the analog voltage from the sound level meter
float ppm = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
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
