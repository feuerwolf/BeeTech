#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card  card;
SdVolume volume;
SdFile   root;

// BeeHive MainBoard
const int chipSelect = 3;

File myFile;


/*
 * return 1  = init OK
 * return 0  = init Error
 * return -1 = config not found
 */
int sdCard_init(){

  Serial.print("Initializing SD card..");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return 0;
  }
  Serial.println(".initialization success.");

  return 1;
 
}

void createLogFileName(){
  char stn_out[10];
  DateTime now = rtc.now();

  sprintf(logFileName_char, "%d_", stationNumber);  // station number
    
  sprintf(stn_out, "%d", now.year()-2000);
  strcat( logFileName_char ,stn_out);

  sprintf(stn_out, "%d",  now.month()); 
  strcat( logFileName_char ,stn_out);

  sprintf(stn_out, "%d.csv", now.day());
  strcat( logFileName_char ,stn_out);

}


void logDataToSdCard(){
  createLogFileName(); 
  createDataString();
  //Serial.print(logFileName_char);
  //Serial.print(" : ");  
  File dataFile = SD.open(logFileName_char, FILE_WRITE);
  if (dataFile) {
      dataFile.println(dataString); // this command APPENDS Data
      dataFile.close();      
      Serial.print("data saved to File: ");
      Serial.println(logFileName_char);
      }
  else{
      Serial.println("error opening logfile.txt");
      }
}

int sdCard_readConfigFile(){
 unsigned long fileSize = 0, i = 0, k = 0;
 char stn_out[100] = "";
 char *result_ptr = 0;
  
 // Check to see if the file exists:
  if (SD.exists("config.txt")) {
    Serial.println("config.txt found:");
 
    myFile = SD.open("config.txt", FILE_READ);
    fileSize = myFile.size();
    char bufferIntern[fileSize];
    
    if (myFile) { 
        for(i = 0; i< fileSize;i++){
          bufferIntern[i] = myFile.read();
          //Serial.write(bufferIntern[i]);
        }
        // search in File ---

        result_ptr = strstr(bufferIntern,"station_number");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           }
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      stn_out[i] = *result_ptr;                  
                      i++;
                      }
                 result_ptr++;                      
                 }          
           stn_out[i] = 0;
           stationNumber = atoi(stn_out);    
           Serial.print("station_number = ");
           Serial.println(stn_out);       
        }
        else
          Serial.print("ERROR reading: station_number"); 

         result_ptr = strstr(bufferIntern,"station_name");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           }          
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      stationName[i] = *result_ptr;                  
                      //Serial.write(stationName[i]);          
                      i++;
                      }
                 result_ptr++;                      
                 }
           stationName[i] = 0;
           Serial.print("station_name = ");
           Serial.println(stationName);                     
        }
        else
          Serial.print("ERROR reading: station_name");               

   

        result_ptr = strstr(bufferIntern,"sdcard_logging");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           }
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      stn_out[i] = *result_ptr;                  
                      i++;
                      }
                 result_ptr++;                      
                 }          
           stn_out[i] = 0;
           sdcard_logging = atoi(stn_out);    
           Serial.print("sdcard_logging = ");
           Serial.println(stn_out);       
        }
        else
          Serial.print("ERROR reading: sdcard_logging");                 
    

        result_ptr = strstr(bufferIntern,"sdcard_log_intervall");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           }
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      stn_out[i] = *result_ptr;                  
                      i++;
                      }
                 result_ptr++;                      
                 }          
           stn_out[i] = 0;
           sdcard_log_intervall = atoi(stn_out);    
           Serial.print("sdcard_log_intervall = ");
           Serial.println(stn_out);       
        }
        else
          Serial.print("ERROR reading: sdcard_log_intervall");                 
    



        result_ptr = strstr(bufferIntern,"calibration_factor");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           }
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      stn_out[i] = *result_ptr;                  
                      i++;
                      }
                 result_ptr++;                      
                 }          
           stn_out[i] = 0;
           calibration_factor_int = atoi(stn_out);    
           Serial.print("calibration_factor = ");
           Serial.println(stn_out);       
        }
        else
          Serial.print("ERROR reading: calibration_factor");    

        result_ptr = strstr(bufferIntern,"scale_offset");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           }
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      stn_out[i] = *result_ptr;                  
                      i++;
                      }
                 result_ptr++;                      
                 }          
           stn_out[i] = 0;
           WEIGHT_OFFSET_float = atof(stn_out);    
           Serial.print("scale_offset = ");
           Serial.println(stn_out);       
        }
        else
          Serial.print("ERROR reading: scale_offset");  

    }
    myFile.close();
  }
    
  else {
    Serial.println("config.txt doesn't exist.");
    return -1;
  }
  //Serial.println("");
  return 1;
  
}


void sdCard_CardInfo() {

  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    return ;
  } 
  
  Serial.println("Wiring is correct and a card is present.");

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
    return;
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
}
