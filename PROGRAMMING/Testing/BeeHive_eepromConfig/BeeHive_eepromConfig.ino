/*

 */

#include <EEPROM.h>


const int serialNumber_int = 1;           // 1 byte
const int calibration_factor_int = -22050;   // 2 bytes
const char stationName[100] = "BeeHive_Lab1\x00";    // max 100 bytes

const int addr_serialNumber = 0;    // 1  byte
const int addr_scaleVale    = 2;    // 2 bytes
const int addr_stationName  = 10;   // 100 bytes


void setup() {
  int k = 0, i = 0;
  
  int serialNumber_return_int = 0;           // 1 byte
  char stationName_return[100] = "";       // max 100 bytes
  int calibration_factor_return_int = 0;  // 2 bytes
  int address = 0;

  Serial.begin(9600);
  delay(1000);
  Serial.println("BeeHive Epprom Config Started");

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


   /**/

  // ----------------------------------------------------------
  
  Serial.println("-------------------------------------------\x0D");
  Serial.println("now checking written values from eeprom:");
  //Serial.println("");
   
  EEPROM.get( addr_serialNumber, serialNumber_return_int );
  Serial.print("station serial number: ");
  Serial.println(serialNumber_return_int);

  EEPROM.get( addr_scaleVale, calibration_factor_return_int );
  Serial.print("scale Calibration factor: ");
  Serial.println(calibration_factor_return_int);

  Serial.print("station name: ");
  address = addr_stationName; 
  for( i = 0; i < k; i++ , address++){
       stationName_return[i] = EEPROM.read(address);
       Serial.print(stationName_return[i]);
  }
  Serial.println("");
  Serial.print("station name again: ");
  Serial.println(stationName_return);
 /* 

  EEPROM.get( addr_stationName, stationName_return_string );
  Serial.print("station name: ");
  Serial.println(stationName_return_string);
*/

 /**/ 
}


void loop() {
  


  delay(100);
}
