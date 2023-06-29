void readSensors(int printresults){
  
  // CO2
  int sensorValue = analogRead(CO2SensorPin);
  float voltage = sensorValue*(3300/1023.0);
  if(voltage == 0 && printresults)
  {
    Serial.println("Fault");
  }
  else if(voltage < 400 && printresults)
  {
    Serial.println("preheating");
  }
  else
  {
    int voltage_difference=voltage-400;
    ppm=voltage_difference*50.0/16.0;
    if(printresults){  
      // Print Voltage
      Serial.print("voltage: ");
      Serial.print(voltage);
      Serial.print("mv - ");
      //Print CO2 concentration
      Serial.print("CO2 Concentration: ");
      Serial.print(ppm);
      Serial.println("ppm");
    }
  }  

}

void sendAvgMQTT(int printresults){

  // Read the sensor value and add it to the total
  totals[0] = totals[0] - readings[0][indx];     // Subtract the old reading from the total
  readings[0][indx] = ppm;   // Read the new sensor value
  totals[0] = totals[0] + readings[0][indx];     // Add the new reading to the total

  indx = (indx + 1) % numReadings;   // Increment the index and wrap around if necessary

  if (indx == 0) {
    // Calculate the average when all readings have been taken
    avgppm = totals[0] / numReadings;
    if(printresults){  
      Serial.print("Average ppm: ");
      Serial.print(avgppm);
    }


    if (!mqttClient.connected()) {
      reconnectMQTT();
    }
    mqttClient.loop();

    snprintf (msg, 50, "%.1f", avgppm);
    char topicCDS[50];
    strcpy(topicCDS, topic);
    strcat(topicCDS, "CDS");
    mqttClient.publish(topicCDS, msg);    

  }
}

void printSensorValues(){


  Serial.print(" ppm: "); 
  Serial.println(ppm); 
 
}
