#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card  card;
SdVolume volume;
SdFile   root;

// BeeHive MainBoard
const int chipSelect = 6;

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
  Serial.println("initialization success");

  return 1;
}



void createLogFileName( int hive_num){
  char stn_out[20];

  sprintf(logFileName_char, "%d_", hive_num);  // station number
  
  sprintf(stn_out, "%d", (int)rtc.getYear());  // station number
  strcat( logFileName_char ,stn_out);
 
  sprintf(stn_out, "%d", (int) rtc.getMonth());  // station number
  strcat( logFileName_char ,stn_out);

  sprintf(stn_out, "%d.csv",(int) rtc.getDay());  // station number
  strcat( logFileName_char ,stn_out);

  Serial.print("LogfileName ");
  Serial.println(logFileName_char);

}


void logDataToSdCard( int hive_num){
  createLogFileName(hive_num); 
  createDataString(0);
  File dataFile = SD.open(logFileName_char, FILE_WRITE);
  if (dataFile) {
      dataFile.println(dataString); // this command APPENDS Data
      dataFile.close();
      Serial.println(dataString);
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
          //Serial.write(bufferIntern[i]);    // print buffer to serial port
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

        result_ptr = strstr(bufferIntern,"station_mode");
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
           operationMode_int = atoi(stn_out);    
           Serial.print("station_mode = ");
           Serial.println(stn_out);       
        }
        else
          Serial.print("ERROR reading: station_mode");         


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

        // -----------------------------------------------
        result_ptr = strstr(bufferIntern,"SIM_PIN");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           }          
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      SIM_PIN[i] = *result_ptr;                  
                      //Serial.write(SIM_PIN[i]);          
                      i++;
                      }
                 result_ptr++;                      
                 }
           SIM_PIN[i] = 0;
           Serial.print("SIM_PIN = ");
           Serial.println(SIM_PIN);                     
        }
        else
          Serial.print("ERROR reading: SIM_PIN");        
        
        result_ptr = strstr(bufferIntern,"GSM_LOGIN");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           }
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      GSM_LOGIN[i] = *result_ptr;                  
                      //Serial.write(GSM_LOGIN[i]);          
                      i++;
                      }
                 result_ptr++;                      
                 }
           GSM_LOGIN[i] = 0;
           Serial.print("GSM_LOGIN = ");
           Serial.println(GSM_LOGIN);                     
        } 
        else
          Serial.print("ERROR reading: GSM_LOGIN");        
          
        result_ptr = strstr(bufferIntern,"GSM_APN");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           } 
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      GSM_APN[i] = *result_ptr;                  
                      //Serial.write(GSM_APN[i]);          
                      i++;
                      }
                 result_ptr++;                      
                 }
           GSM_APN[i] = 0; 
           Serial.print("GSM_APN = ");
           Serial.println(GSM_APN);                    
        }
        else
          Serial.print("ERROR reading: GSM_APN");                
        
        result_ptr = strstr(bufferIntern,"GSM_PASSWORD");
        if(result_ptr > 0){
           while( *result_ptr != '='){ 
                  result_ptr++;
           }
           i = 0;
           while( *result_ptr != ';'){ // extract value                  
                  if( (*result_ptr != ' ') && (*result_ptr != ';') && (*result_ptr != '=')){
                      GSM_PASSWORD[i] = *result_ptr;                  
                      //Serial.write(GSM_PASSWORD[i]);          
                      i++;
                      }
                 result_ptr++;                      
                 }
           GSM_PASSWORD[i] = 0;
           Serial.print("GSM_PASSWORD = ");
           Serial.println(GSM_PASSWORD);                     
        } 
        else
          Serial.print("ERROR reading: GSM_PASSWORD");                

                 
        // Database Logging --------------------------------
  
        result_ptr = strstr(bufferIntern,"databaseLog");
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
           databaseLog_int = atoi(stn_out);    
           Serial.print("databaseLog = ");
           Serial.println(stn_out);       
        }
        else
          Serial.print("ERROR reading: databaseLog"); 

        result_ptr = strstr(bufferIntern,"upload_timing");
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
           upload_timing = atoi(stn_out);    
           Serial.print("upload_timing = ");
           Serial.println(stn_out);       
        }
        else
          Serial.print("ERROR reading: databaseLog"); 

        result_ptr = strstr(bufferIntern,"INSERT_SQL");
        if(result_ptr > 0){
           while( *result_ptr != '"'){ 
                  result_ptr++;
           }
           *result_ptr = '*';
           i = 0;
           while( *result_ptr != '"'){ // extract value 
                result_ptr++;                  
                INSERT_SQL[i] = *result_ptr;
                //Serial.write(INSERT_SQL[i]);                  
                i++;                      
                }          
           INSERT_SQL[i-1] = 0;              
           Serial.print("INSERT_SQL = ");
           Serial.println(INSERT_SQL);       
        }
        else
          Serial.print("ERROR reading: databaseLog"); 

      // Network connection settings   
      //=3;    // number of beehives in total

        result_ptr = strstr(bufferIntern,"network_enabled");
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
           network_enabled = atoi(stn_out);    
           Serial.print("network_enabled = ");
           Serial.println(network_enabled);       
        }
        else
          Serial.print("ERROR reading: network_enabled"); 

        result_ptr = strstr(bufferIntern,"number_beehives");
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
           number_beehives = atoi(stn_out);    
           Serial.print("number_beehives = ");
           Serial.println(number_beehives);       
        }
        else
          Serial.print("ERROR reading: network_enabled"); 

                 
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
