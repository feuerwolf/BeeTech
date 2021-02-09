/*
 * from 
 * Dalla Single Sensor example
 * 
 * 
 */



void init_DS18B20_Temp_Scale(){

  Serial.println("Temperature IC Sensor 1");
  Serial.print("Locating devices...");
  sensor_Temp_Scale.begin();
  Serial.print("Found ");
  Serial.print(sensor_Temp_Scale.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensor_Temp_Scale.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  if (!sensor_Temp_Scale.getAddress(insideThermometer_Scale, 0)) Serial.println("Unable to find address for Device 0"); 
  
  // show the addresses we found on the bus
  Serial.print("Device 1 Address: ");
  printAddress(insideThermometer_Scale);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensor_Temp_Scale.setResolution(insideThermometer_Scale, 9);
 
  Serial.print("Device 1 Resolution: ");
  Serial.print(sensor_Temp_Scale.getResolution(insideThermometer_Scale), DEC); 
  Serial.println();

}


void init_DS18B20_Temp_BeeHive(){

  Serial.println("Temperature IC Sensor 2");
  Serial.print("Locating devices...");
  sensor_Temp_BeeHive.begin();
  Serial.print("Found ");
  Serial.print(sensor_Temp_BeeHive.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensor_Temp_BeeHive.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  if (!sensor_Temp_BeeHive.getAddress(insideThermometer_BeeHive, 0)) Serial.println("Unable to find address for Device 0"); 
  
  // show the addresses we found on the bus
  Serial.print("Device 2 Address: ");
  printAddress(insideThermometer_BeeHive);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensor_Temp_BeeHive.setResolution(insideThermometer_BeeHive, 9);
 
  Serial.print("Device 2 Resolution: ");
  Serial.print(sensor_Temp_BeeHive.getResolution(insideThermometer_BeeHive), DEC); 
  Serial.println();

}


// -------------------------------------------------------------------------------------------------------------------




// function to print the temperature for a device
float getTemperature_sensor_Scale(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));

  // method 2 - faster
  float tempC = sensor_Temp_Scale.getTempC(deviceAddress);
  //Serial.print("Temp Sensor_1 C: ");
  //Serial.println(tempC);

  return tempC; 
}


// function to print the temperature for a device
float getTemperature_sensor_BeeHive(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));

  // method 2 - faster
  float tempC = sensor_Temp_BeeHive.getTempC(deviceAddress);
  //Serial.print("Temp Sensor_2 C: ");
  //Serial.println(tempC);

  return tempC;
}



// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
