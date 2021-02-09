


void processSensorData(){
  beehive_t beehive;
  int num_beehive = 0, i = 0, cnt = 0;
  const int MAX_LENGTH = 20;
  char stnIntern[MAX_LENGTH];
  char *ptr = 0;
  
  i = 0;
  cnt = 0;
  ptr = &inputStream[0];  
  // incoming string will look like = 0,24.00,23.50,0.00,55,
  // extract beehinve number
  while( *ptr != ','){ 
      stnIntern[i] = *ptr;      
      ptr++;    
      i++;
      cnt++;
      if(cnt==MAX_LENGTH){
           Serial.println("Error decoding num_beehive");
           return;
      }      
  }
  *ptr = '*';   
  ptr++;   
  stnIntern[i] = 0;
  i = 0;
  cnt = 0;
  num_beehive = atoi(stnIntern);     
  /*Serial.print("num_beehive = ");
  Serial.println(num_beehive);
  Serial.println(stnIntern);*/

  // extract temp 1
  while( *ptr != ','){ 
      stnIntern[i] = *ptr;      
      ptr++;    
      i++;
      cnt++;
      if(cnt==MAX_LENGTH){
           Serial.println("Error decoding temp1_float");
           return;
      }
   }
  *ptr = '*';      
  ptr++;
  stnIntern[i] = 0;
  i = 0;
  cnt = 0;
  beehive.temp1_float = atof(stnIntern);     
  /*Serial.print("beehive.temp1 = ");
  Serial.println(beehive.temp1_float);
  Serial.println(stnIntern);*/

  // extract temp 2
  while( *ptr != ','){ 
      stnIntern[i] = *ptr;      
      ptr++;    
      i++;
      cnt++;      
      if(cnt==MAX_LENGTH){
           Serial.println("Error decoding tem21_float");
           return;
      }      
  }
  *ptr = '*';
  ptr++;
  stnIntern[i] = 0;
  i = 0;
  cnt = 0;
  beehive.temp2_float = atof(stnIntern);     
  /*Serial.print("beehive.temp2 = ");
  Serial.println(beehive.temp2_float);
  Serial.println(stnIntern);*/

  // extract weight
  while( *ptr != ','){ 
      stnIntern[i] = *ptr;      
      ptr++;    
      i++;
      cnt++;      
      if(cnt==MAX_LENGTH){
           Serial.println("Error decoding weight_float");
           return;
      }      
  }
  *ptr = '*';
  ptr++;      
  stnIntern[i] = 0;
  i = 0;
  cnt = 0;
  beehive.weight_float = atof(stnIntern);     
  /*
  Serial.print("beehive.weight = ");
  Serial.println(beehive.weight_float);
  Serial.println(stnIntern);*/

  // extract weight
  while( *ptr != ','){ 
      stnIntern[i] = *ptr;      
      ptr++;    
      i++;
      cnt++;      
      if(cnt==MAX_LENGTH){
           Serial.println("Error decoding sound_int");
           return;
      }      
  }
  *ptr = '*';
  ptr++;      
  stnIntern[i] = 0;
  i = 0;
  cnt = 0;
  beehive.sound_int = atoi(stnIntern);     
  /*
  Serial.print("beehive.sound = ");
  Serial.println(beehive.sound_int);
  Serial.println(stnIntern);
  */

  BEEHIVE[num_beehive] = beehive;

  /*
  Serial.println("Decoded received Data:");
  Serial.print(num_beehive);
  Serial.print(",");
  Serial.print(BEEHIVE[num_beehive].temp1_float);
  Serial.print(",");
  Serial.print(BEEHIVE[num_beehive].temp2_float);  
  Serial.print(",");
  Serial.print(BEEHIVE[num_beehive].weight_float);
  Serial.print(",");
  Serial.print(BEEHIVE[num_beehive].sound_int);
  Serial.print(",");
  Serial.println("");
  */  
}




void getNetworkSensorData(int num_beehive){

  Serial1.write(num_beehive); // Hardware Serial Port
  Serial1.write(rtc.getDay());
  Serial1.write(rtc.getMonth());
  Serial1.write(rtc.getYear());
  Serial1.write(rtc.getHours());
  Serial1.write(rtc.getMinutes());
  Serial1.write(rtc.getSeconds());
  Serial1.write(0x0D); 
}




void getLocalSensorData(int num_beehive){
  float WEIGHT_SENSOR_float = 0.0;

  if( TEMP1_CON == SENSOR_CONNECTED){
      sensor1.requestTemperatures(); // Send the command to get temperatures
      BEEHIVE[num_beehive].temp1_float = getTemperature_sensor1(insideThermometer1); // Use a simple function to print out the data
      }
  if( TEMP2_CON == SENSOR_CONNECTED){
      sensor2.requestTemperatures(); // Send the command to get temperatures
      BEEHIVE[num_beehive].temp2_float = getTemperature_sensor2(insideThermometer2); // Use a simple function to print out the data
      }
  if( SCALE_CON == SENSOR_CONNECTED){
      WEIGHT_SENSOR_float = scale_readings();
      //BEEHIVE[num_beehive].weight_float = scale_readings();
      }
  BEEHIVE[num_beehive].weight_float = WEIGHT_OFFSET_float + WEIGHT_SENSOR_float;
  /*
  if(BEEHIVE[num_beehive].weight_float < 0.0)
     BEEHIVE[num_beehive].weight_float = 0.0;        // in case correction factor from SD CArd is slightly different
  */
  
  BEEHIVE[num_beehive].sound_int = getSound(analogSoundPin) ; // analogRead(analogSoundPin);      
}
