void readSensors(int printresults){
  
  // Light
  lux = myLux.lightStrengthLux();
  if(printresults){
    Serial.print("value: ");
    Serial.print(lux);
    Serial.println(" (lux).");
  }

  // Sound
  float voltageValue;
  voltageValue = analogRead(SoundSensorPin) / 1023.0 * VREF;
  sound = voltageValue * 50.0;  //convert voltage to decibel value
  if(printresults){  
    Serial.print(sound,1);
    Serial.println(" dBA");
  }

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

  // Presence
  presence = digitalRead(PresencePin);
  if(printresults){  
    Serial.print("Presence State: ");
    Serial.println(presence);
  }

  // Temperature
  // Get the voltage reading from the TMP36
  int reading = analogRead(tmp36Pin);
  float tmp36voltage = reading * (3.3 / 1024.0);
  // Convert the voltage into the temperature in Celsius
  tmp = (tmp36voltage - 0.5) * 100;
  if(printresults){  
    Serial.print("Temperature: ");
    Serial.println(tmp);
  }

/*
  // Temperature
  delay(20);
  tmp = hdc1080.readTemperature();
  if(printresults){  
    Serial.print("Temperature: ");
    Serial.println(tmp);
  }

  // Humidity
  delay(20);
  hum = hdc1080.readHumidity();
  if(printresults){  
    Serial.print("Humidity: ");
    Serial.println(hum);
  }
*/
}

void sendAvgMQTT(int printresults){

  // Read the sensor value and add it to the total
  totals[0] = totals[0] - readings[0][indx];     // Subtract the old reading from the total
  readings[0][indx] = presence;   // Read the new sensor value
  totals[0] = totals[0] + readings[0][indx];     // Add the new reading to the total

  totals[1] = totals[1] - readings[1][indx];     // Subtract the old reading from the total
  readings[1][indx] = sound;   // Read the new sensor value
  totals[1] = totals[1] + readings[1][indx];     // Add the new reading to the total

  totals[2] = totals[2] - readings[2][indx];     // Subtract the old reading from the total
  readings[2][indx] = ppm;   // Read the new sensor value
  totals[2] = totals[2] + readings[2][indx];     // Add the new reading to the total

  totals[3] = totals[3] - readings[3][indx];     // Subtract the old reading from the total
  readings[3][indx] = lux;   // Read the new sensor value
  totals[3] = totals[3] + readings[3][indx];     // Add the new reading to the total

  totals[4] = totals[4] - readings[4][indx];     // Subtract the old reading from the total
  readings[4][indx] = tmp;   // Read the new sensor value
  totals[4] = totals[4] + readings[4][indx];     // Add the new reading to the total

//  totals[5] = totals[5] - readings[5][indx];     // Subtract the old reading from the total
//  readings[5][indx] = hum;   // Read the new sensor value
//  totals[5] = totals[5] + readings[5][indx];     // Add the new reading to the total

  indx = (indx + 1) % numReadings;   // Increment the index and wrap around if necessary

  if (indx == 0) {
    // Calculate the average when all readings have been taken
    avgpresence = totals[0] / numReadings;
    if(printresults){  
      Serial.print("Average presence: ");
      Serial.print(avgpresence);
    }

    avgsound = totals[1] / numReadings;
    if(printresults){  
      Serial.print(" Average sound: ");
      Serial.print(avgsound);
    }

    avgppm = totals[2] / numReadings;
    if(printresults){  
      Serial.print(" Average ppm: ");
      Serial.print(avgppm);
    }

    avglux = totals[3] / numReadings;
    if(printresults){  
      Serial.print(" Average avglux: ");
      Serial.print(avglux);
    }

    avgtmp = totals[4] / numReadings;
    if(printresults){  
      Serial.print(" Average avgtmp: ");
      Serial.println(avgtmp);
    }

//    avghum = totals[5] / numReadings;
//    if(printresults){  
//      Serial.print(" Average avghum: ");
//      Serial.println(avghum);
//    }    

    if (!mqttClient.connected()) {
      reconnectMQTT();
    }
    mqttClient.loop();

    snprintf (msg, 50, "%.1f", avgpresence);
    char topicPCS[50];
    strcpy(topicPCS, topic);
    strcat(topicPCS, "PCS");
    mqttClient.publish(topicPCS, msg);     

    snprintf (msg, 50, "%.1f", avgsound);
    char topicSLS[50];
    strcpy(topicSLS, topic);
    strcat(topicSLS, "SLS");
    mqttClient.publish(topicSLS, msg); 

    snprintf (msg, 50, "%.1f", avgppm);
    char topicCDS[50];
    strcpy(topicCDS, topic);
    strcat(topicCDS, "CDS");
    mqttClient.publish(topicCDS, msg);    

    snprintf (msg, 50, "%.1f", avglux);
    char topicLPS[50];
    strcpy(topicLPS, topic);
    strcat(topicLPS, "LPS");
    mqttClient.publish(topicLPS, msg);    

    snprintf (msg, 50, "%.1f", avgtmp);
    char topicTPS[50];
    strcpy(topicTPS, topic);
    strcat(topicTPS, "TPS");
    mqttClient.publish(topicTPS, msg);

//    snprintf (msg, 50, "%.1f", avghum);
//    char topicHUM[50];
//    strcpy(topicHUM, topic);
//    strcat(topicHUM, "HMS");
//    mqttClient.publish(topicHUM, msg);

  }
}

void printSensorValues(){

  Serial.print(" presence: "); 
  Serial.print(presence); 
  Serial.print(" sound: "); 
  Serial.print(sound); 
  Serial.print(" ppm: "); 
  Serial.print(ppm); 
  Serial.print(" lux: "); 
  Serial.print(lux);    
  Serial.print(" tmp: "); 
  Serial.println(tmp);    
//  Serial.print(" hum: "); 
//  Serial.println(hum);    
}
