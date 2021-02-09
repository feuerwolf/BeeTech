// https://github.com/ChuckBell/MySQL_Connector_Arduino/wiki/Common-Solutions


void InitModem(){
  
  
  int cnt = 0;
  Serial.println("Connecting to Network");
    tft.println("network connect?");
  // connection state
  bool connected = false;

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  
  while (!connected) {
    if ((gsmAccess.begin(SIM_PIN) == GSM_READY) &&
        (gprsAccess.attachGPRS(GSM_APN, GSM_LOGIN, GSM_PASSWORD) == GPRS_READY)) {
      connected = true;
      Serial.println("GPRS connected");
      tft.println(" success!");
    } else {
      Serial.println("GPRS Not connected");
      cnt++;
      delay(1000);
      if(cnt > 10)
        break;
    }
  }

  // ---------------------------------------------

  Ethernet.begin(mac_addr);
  Serial.println("Connecting...");

  if (conn.connect(server_addr, 3306, user, password)) {
     Serial.println("... Connection success!");
     tft.println("Database connected!");
  }
  else
    Serial.println("Database failed");
 
}


void uploadData2Mysql(int num_beehive){
  char *result_ptr = 0;
  char stn_out[20];
  char INSERT_SQL_temporary[150];
  //Serial.println("preparing data for upload 2 Database");
  // char INSERT_SQL[200] = "INSERT INTO k139859_test_arduino.test_data (location,hive,temp1,temp2,weight,sound) VALUES ('Zuin',1,10.0,24.0,50.0,30.0)";
  
  strcpy( INSERT_SQL_temporary, INSERT_SQL);
  // Serial.println(INSERT_SQL_temporary);
  Serial.println("---");
  
  result_ptr = strstr(INSERT_SQL_temporary,"VALUES");
  if(result_ptr > 0){
 
     *result_ptr = 0;

     strcat(INSERT_SQL_temporary , "VALUES ('");
     
     strcat( INSERT_SQL_temporary ,stationName);     // station name
     
     sprintf(stn_out, "',%.0f,", (float) num_beehive);  // station number
     strcat( INSERT_SQL_temporary ,stn_out);

     sprintf(stn_out, "%.1f,", BEEHIVE[num_beehive].temp1_float);      // temp1
     strcat( INSERT_SQL_temporary ,stn_out);

     sprintf(stn_out, "%.1f,", BEEHIVE[num_beehive].temp2_float);      // temp2
     strcat( INSERT_SQL_temporary ,stn_out);

     sprintf(stn_out, "%.1f,", BEEHIVE[num_beehive].weight_float);     // weight
     strcat( INSERT_SQL_temporary ,stn_out);

     sprintf(stn_out, "%.0f)", (float)BEEHIVE[num_beehive].sound_int);  // sound
     strcat( INSERT_SQL_temporary ,stn_out);          
     
     Serial.println(INSERT_SQL_temporary);    
  }
  else
     Serial.print("ERROR creating database string"); 
  
  Serial.println("---");

  // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Execute the query
  cur_mem->execute(INSERT_SQL_temporary);
  // Note: since there are no results, we do not need to read any data
  // Deleting the cursor also frees up memory used
  delete cur_mem;

}


void testMysql(){
  Serial.println("Recording data");

  // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Execute the query
  cur_mem->execute(INSERT_SQL);
  // Note: since there are no results, we do not need to read any data
  // Deleting the cursor also frees up memory used
  delete cur_mem;
}
