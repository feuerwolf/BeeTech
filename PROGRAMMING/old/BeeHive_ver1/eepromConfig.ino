




void getEepromConfigData(){
  int k = 100, i = 0;
  int serialNumber_return_int = 0;           // 1 byte
  int calibration_factor_return_int = 0;  // 2 bytes
  int address = 0;
  
  Serial.println("Reading config Data from from eeprom:");  
  EEPROM.get( addr_serialNumber, serialNumber_int );
  Serial.print("station serial number: ");
  Serial.println(serialNumber_int);

  EEPROM.get( addr_scaleVale, calibration_factor_int );
  Serial.print("scale Calibration factor: ");
  Serial.println(calibration_factor_int);

  Serial.print("station name: ");
  address = addr_stationName; 
  for( i = 0; i < k; i++ , address++){
       stationName[i] = EEPROM.read(address);
       //Serial.print(stationName[i]);
  }
  //Serial.println("");
  Serial.print("station name : ");
  Serial.println(stationName);

}



/*
 * write config data to eeprom
 * hardcoded at the moment, not used in software at the moment
 * ToDo: read values from keyboard
*/
void setEepromConfigData(){
  int k = 100, i = 0;
  int address = 0;

  // writes station Serial number
  EEPROM.put(addr_serialNumber, serialNumber_int);
  Serial.println("-> serial number saved to eeprom");
  Serial.print  ("   station serial number: ");
  Serial.println(serialNumber_int);

  // write calibration factor
  EEPROM.put(addr_scaleVale, calibration_factor_int);
  Serial.println("-> scale Calibration factor saved to eeprom");
  Serial.print  ("   scale calibration factor: ");
  Serial.println(calibration_factor_int);

  // write Station Name
  k = strlen(stationName);
  Serial.print("-> station Name lenght: ");
  Serial.println(k);
  Serial.print("   station Name: ");
  address = addr_stationName; 
  for( i = 0; i < k; i++ , address++){
       EEPROM.write(address, stationName[i]);
       Serial.print(stationName[i]);
  }
  EEPROM.write(address, 0); // write terminating 0 to end of string
  Serial.println("");
  Serial.println("   station Name saved to eeprom"); 
}
