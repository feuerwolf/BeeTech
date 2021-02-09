/*

 */

#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN
const int chipSelect = 3;


// EEPROM ---
const int serialNumber_int = 1;           // 1 byte
const int calibration_factor_int = -22050;   // 2 bytes
const char stationName[100] = "BeeHive_Lab1\x00";    // max 100 bytes

const int addr_serialNumber = 0;    // 1  byte
const int addr_scaleVale    = 2;    // 2 bytes
const int addr_stationName  = 10;   // 100 bytes
// --- 

void setup() {
  int k = 0, i = 0;
  
  int serialNumber_return_int = 0;           // 1 byte
  char stationName_return[100] = "";       // max 100 bytes
  int calibration_factor_return_int = 0;  // 2 bytes
  int address = 0;

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("BeeHive SD Card - Epprom Config Started");


// --- SD Card -------------------------------------------------------------------------------------------

  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.println();
  Serial.print("Card type:         ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }

  Serial.print("Clusters:          ");
  Serial.println(volume.clusterCount());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(volume.blocksPerCluster());

  Serial.print("Total Blocks:      ");
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);


// --- EEPROM ---------------------------------------------------------------------------

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
