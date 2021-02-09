/*
 * from 
 * Dalla Single Sensor example
 * 
 * 
 */



void init_DS18B20_Temp1(){

  Serial.println("Temperature IC Sensor 1");
  Serial.print("Locating devices...");
  sensor1.begin();
  Serial.print("Found ");
  Serial.print(sensor1.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensor1.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  if (!sensor1.getAddress(insideThermometer1, 0)) Serial.println("Unable to find address for Device 0"); 
  
  // show the addresses we found on the bus
  Serial.print("Device 1 Address: ");
  printAddress(insideThermometer1);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensor1.setResolution(insideThermometer1, 9);
 
  Serial.print("Device 1 Resolution: ");
  Serial.print(sensor1.getResolution(insideThermometer1), DEC); 
  Serial.println();

}


void init_DS18B20_Temp2(){

  Serial.println("Temperature IC Sensor 2");
  Serial.print("Locating devices...");
  sensor2.begin();
  Serial.print("Found ");
  Serial.print(sensor2.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensor2.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  if (!sensor2.getAddress(insideThermometer2, 0)) Serial.println("Unable to find address for Device 0"); 
  
  // show the addresses we found on the bus
  Serial.print("Device 2 Address: ");
  printAddress(insideThermometer2);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensor2.setResolution(insideThermometer2, 9);
 
  Serial.print("Device 2 Resolution: ");
  Serial.print(sensor2.getResolution(insideThermometer2), DEC); 
  Serial.println();

}


// -------------------------------------------------------------------------------------------------------------------




// function to print the temperature for a device
float printTemperature_sensor1(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));

  // method 2 - faster
  float tempC = sensor1.getTempC(deviceAddress);
  Serial.print("Temp Sensor_1 C: ");
  Serial.println(tempC);

  return tempC; 
}


// function to print the temperature for a device
float printTemperature_sensor2(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));

  // method 2 - faster
  float tempC = sensor2.getTempC(deviceAddress);
  Serial.print("Temp Sensor_2 C: ");
  Serial.println(tempC);

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
