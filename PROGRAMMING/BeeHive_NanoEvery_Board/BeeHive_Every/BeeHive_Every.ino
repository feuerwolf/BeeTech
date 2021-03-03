#include <SPI.h>
#include <SD.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>      // for later
#include <Wire.h>               // for later
#include "stdio.h"
#include "RTClib.h"


#define FIRMWARE_VER 1


#define MODE_MASTER  0
#define MODE_MASTER_DISABLED 1

#define ONE_WIRE_SENSOR_1 6  // Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_SENSOR_2 7  // Data wire is plugged into port 2 on the Arduino

#define TFT_RST      8
#define TFT_DC       9
#define TFT_CS       10
#define TFT_EN       15

#define HX_DOUT      16
#define HX_CLK       17

#define PIN_NEOPIX   21   // Pin connected
#define NUMPIXELS    1    // number of connected neopixels

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);  // using hardware SPI

Adafruit_NeoPixel neopix(NUMPIXELS, PIN_NEOPIX, NEO_GRB + NEO_KHZ800);

#define setNeoPixColor_RED    neopix.setPixelColor(0,neopix.Color(255,0,0));neopix.show();  
#define setNeoPixColor_GREEN  neopix.setPixelColor(0,neopix.Color(0,255,0));neopix.show();
#define setNeoPixColor_BLUE   neopix.setPixelColor(0,neopix.Color(0,0,255));neopix.show();


OneWire oneWire1(ONE_WIRE_SENSOR_1);   // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor1(&oneWire1);  // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer1;      // arrays to hold device address

OneWire oneWire2(ONE_WIRE_SENSOR_2);    // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor2(&oneWire2);  // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer2;      // arrays to hold device address

// --- Time -------------

RTC_Millis rtc;  // software lib

// internal time struct
typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
} timeStruct_t;

// --- Buttons ----------

const int SWITCH  =  20;  // interrupt 28   
const int BUTTON1 =  18;  // interrupt 2
const int BUTTON2 =  19;  // interrupt 3

// --- Analog ------------
const int analogSoundPin = A0;    // select the input pin for the potentiometer

// --- SoftSerial---------
SoftwareSerial softSerial(4, 5); // RX = D4, TX = D5

// --- Modes--- ----------
int operationMode_int = 0;         // logging disabled/enabled
int operationModeOld_int = 0;       
int serialNumber_int = 0;           
int calibration_factor_int = 0;   
int stationNumber = 1;    
char stationName[100] = ""; 
int sdcard_logging=0;    
int sdcard_log_intervall=60;   

const int addr_serialNumber = 0;    // 1  byte
const int addr_scaleVale    = 2;    // 2 bytes
const int addr_stationName  = 10;   // 100 bytes

#define SENSOR_DISCONNECTED   0
#define SENSOR_CONNECTED      1
int TEMP1_CON = 0;
int TEMP2_CON = 0;
int SCALE_CON = 0;
int AUDIO_CON = 0;

// beehive struct
typedef struct {
  float temp1_float;
  float temp2_float;
  float weight_float;
  int   sound_int;
} beehive_t;

beehive_t BEEHIVE;

float WEIGHT_OFFSET_float = 0.0;

// --- Misc ------------------
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval_1000ms = 1000;      
const long interval_2000ms = 2000;      
const long interval_5000ms = 5000;      
const long interval_10000ms = 10000;      

char Buffer_Softserial[100] = "";         // a String to hold incoming data
char Buffer_Terminal[100] = "";         // a String to hold incoming data
bool TerminalComplete = false;  // whether the string is complete

String dataString;
char logFileName_char[20];

// Interrupt Service Routine (ISR)
void ISR_switch ()
{ 
     
  if (digitalRead (SWITCH) == HIGH){   // System enabled
      operationModeOld_int = operationMode_int;
      operationMode_int = MODE_MASTER_DISABLED;
      Serial.println("Pin High");
      Serial.println("System Disabled"); 
      Serial.print("System mode: "); 
      Serial.println(operationMode_int);       
  }
  else{   // System disabled
      operationMode_int = operationModeOld_int;
      Serial.println("Pin Low");
      Serial.println("System Enabled"); 
      Serial.print("System mode: "); 
      Serial.println(operationMode_int); 
  } 
} 


// Interrupt Service Routine (ISR)
void ISR_button1_Pressed ()
{ 

}  

// Interrupt Service Routine (ISR)
void ISR_button2_Pressed ()
{
  if (digitalRead (TFT_EN) == LOW){
    digitalWrite(TFT_EN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else{
    digitalWrite(TFT_EN, LOW);   // turn the LED on (HIGH is the voltage level)
  }
} 

void setup()
{
    // serial Port via USB
    Serial.begin(9600, SERIAL_8N1);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("");
    Serial.println("-----------------");
    Serial.println("- System Start  -");
    Serial.println("-----------------");
    
    // serial Port via Hardware Uart  at Pin16=TX, Pin17=RX
    Serial1.begin(9600); // Hardware Serial Port
    //Serial1.println("Hello, I am the hard Serial!");
    // Softserial receives Data from another Slave passes it to Master
    // Software Serial Port at RX = D4, TX = D5
    softSerial.begin(9600); 
    //softSerial.println("Hello, I am the soft Serial!");

    // --- Digital ---
    pinMode(TFT_EN, OUTPUT);
    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT); 
    pinMode(SWITCH , INPUT);
    attachInterrupt (digitalPinToInterrupt (BUTTON1), ISR_button1_Pressed, FALLING);  // attach interrupt handler
    attachInterrupt (digitalPinToInterrupt (BUTTON2), ISR_button2_Pressed, FALLING);  // attach interrupt handler
    attachInterrupt (digitalPinToInterrupt (SWITCH ), ISR_switch         , CHANGE);  // attach interrupt handler

    // --- NeoPix ---

    neopix.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    //neopix.setPixelColor(0, neopix.Color(0, 150, 0));  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    setNeoPixColor_GREEN;
    neopix.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
    neopix.show();   // Send the updated pixel colors to the hardware.
    
    Serial.println("---");
    serialNumber_int = getEepromSerialNumber();
    
    Serial.println("---");
    sdCard_init();
    sdCard_readConfigFile();
    Serial.println("---");
    
    digitalWrite(TFT_EN, LOW);
    //digitalWrite(TFT_EN, HIGH);
    init_tft();
    
    tft.fillScreen(ILI9341_BLACK);

    testLines(ILI9341_CYAN);

    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0,0);
    tft.setTextColor(ILI9341_GREEN);   
    tft.println(stationName);
    
    tft.print("Number ");
    tft.println(stationNumber);
    
    tft.print("Serial ");
    tft.println(serialNumber_int);

    tft.print("scale ");
    tft.println(calibration_factor_int);
    
    if( operationMode_int == MODE_MASTER)
        tft.println("Master mode");

    tft.print("FW ver:  ");
    tft.println(FIRMWARE_VER);
    
    delay(4000);
    
    rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
    
    setNeoPixColor_BLUE;
    
    TEMP1_CON = init_DS18B20_Temp1();
    TEMP2_CON = init_DS18B20_Temp2();
    Serial.println("---");
    SCALE_CON = scale_initialize();
    
    // ---------------------------------
    // LOG SCREEN
    // ---------------------------------
    
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0,0);
    tft.setTextColor(ILI9341_GREEN);   
    tft.println(stationName);
    tft.print("Bee Hive ");
    tft.println(stationNumber);
    tft.println("-----------");
    tft.setCursor(0,80);
    
    Serial.println("-----------------");
    Serial.println("- logging start -");
    Serial.println("-----------------");
    Serial.println("date,time,temp1C,temp2C,weight,sound,"); 

}


void loop()
{
  static int k = 0, i = 0;
  char inChar = 0;
  
  // --- Terminal User data -----------------------------
  if (Serial.available()) {          
      inChar = (char)Serial.read();      
      Buffer_Terminal[k] = inChar;
      k++;
      if (inChar == 0x0D) { // if (inChar == '\n') {
          Buffer_Terminal[k] = 0;            
          k = 0;
          processTerminalData(); 
      }
  }

  // --- Network: Data from another slave ---------------
  if (softSerial.available()) {         // if anything comes in SoftwareSerial (from slave)
    Serial1.write(softSerial.read());   // read it and send it out on Serial1 (to master)
  }

  // --- Network: Data from Master ----------------------  
  // Buffer_Softserial
  if (Serial1.available()) {                
      inChar = (char)Serial1.read();
      Buffer_Softserial[i] = inChar;
      i++;      
      if(inChar == 0x0D) { // if (inChar == '\n') {
          Buffer_Softserial[i] = 0;                      
          if(Buffer_Softserial[0] == stationNumber){
              Serial.print("Received request - it's me: ");
              Serial.println(Buffer_Softserial);
               
              rtc.adjust(DateTime((uint16_t)Buffer_Softserial[3] + 2000,  // year
                                  (uint8_t) Buffer_Softserial[2],     // month
                                  (uint8_t) Buffer_Softserial[1],     // day
                                  (uint8_t) Buffer_Softserial[4],     // hour
                                  (uint8_t) Buffer_Softserial[5],     // min 
                                  (uint8_t) Buffer_Softserial[6]));   // sec
              
              sendSensorData();   // sends data to master
              }
          else{
              Serial.print("forwarding request to station ");
              Serial.print(Buffer_Softserial[0] );
              Serial.print(" data: ");
              for(i = 0; i < 8; i++){
                  Serial.write(Buffer_Softserial[i] );
                  softSerial.write(Buffer_Softserial[i]);
                  }
              Serial.println("");                      
              }                     
          i = 0;
          }
      }


  
  // --- state machine ----------------------------------
  switch (operationMode_int) {  
    case MODE_MASTER: 
            loop_master(); 
            break;
    case MODE_MASTER_DISABLED: 
            loop_master_disabled(); 
            break;                 
   }  
}


void loop_master_disabled(){
  static int i = 0;

  
  // here is the delayed code
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval_2000ms){
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // code to be executed:
      
      getSensorData(); 
      printSensorData();
      if (digitalRead (TFT_EN) == HIGH)
          updateTFT();
      setNeoPixColor_RED;                  
  } 
}


void loop_master(){
  static int i = 0;
  static int cnt = 0, cntTo2sec = 0, cntToSdCardSave = 0;
  int seconds2update = 2;
  
   
  // here is the delayed code
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval_1000ms){
      previousMillis = currentMillis;
      cntTo2sec++;
      cntToSdCardSave++;
    
      // logging data to SD Card
      if((sdcard_logging == 1) && ( sdcard_log_intervall == cntToSdCardSave)){
          cntToSdCardSave = 0;          
          logDataToSdCard();
          }

      if(cntTo2sec == seconds2update){
          cntTo2sec = 0;
          getSensorData(); 
          printTime();
          printSensorData();
          if (digitalRead (TFT_EN) == HIGH)
              updateTFT();
          if(i == 0){
             setNeoPixColor_BLUE;
             i++;
          }
          else if(i == 1){  
             setNeoPixColor_GREEN;
             i= 0;
          }
      }                     
  } 
}




void sendSensorData(){
  Serial1.print(stationNumber);  // to Master
  Serial1.print(",");
  Serial1.print(BEEHIVE.temp1_float);    // to Master  
  Serial1.print(",");
  Serial1.print(BEEHIVE.temp2_float);    // to Master
  Serial1.print(",");
  Serial1.print(BEEHIVE.weight_float);   // to Master
  Serial1.print(",");
  Serial1.print(BEEHIVE.sound_int);      // to Master
  Serial1.print (",");  
  Serial1.write (0x0D);       // \n
}

void printSensorData(){
  Serial.print (stationNumber);    // to Terminal
  Serial.print (","); 
  Serial.print (BEEHIVE.temp1_float);    // to Terminal
  Serial.print (",");  
  Serial.print (BEEHIVE.temp2_float);    // to Terminal
  Serial.print (",");  
  Serial.print (BEEHIVE.weight_float);   // to Terminal
  Serial.print (",");  
  Serial.print (BEEHIVE.sound_int);      // to Terminal   
  Serial.print (",");  
  Serial.println ("");      // to Terminal   
}


void createDataString(){
  DateTime now = rtc.now();
  // string: station,behiveName,date,time,
  
  dataString = String(stationNumber);
  dataString += ",";  
  dataString += stationName;
  dataString += ","; 
  dataString += now.year()-2000;
  dataString += "/";
  dataString += now.month();
  dataString += "/";
  dataString += now.day();
  dataString += ",";
  
  dataString += now.hour();
  dataString += ":";
  dataString += now.minute();
  dataString += ":";
  dataString += now.second();
  dataString += ",";
  dataString += BEEHIVE.temp1_float;
  dataString += ",";  
  dataString += BEEHIVE.temp2_float;
  dataString += ",";  
  dataString += BEEHIVE.weight_float;
  dataString += ",";  
  dataString += BEEHIVE.sound_int;
  dataString += ",";  
}


void getSensorData(){
  float WEIGHT_SENSOR_float = 0.0;

  if( TEMP1_CON == SENSOR_CONNECTED){
      sensor1.requestTemperatures(); // Send the command to get temperatures
      BEEHIVE.temp1_float = getTemperature_sensor1(insideThermometer1); // Use a simple function to print out the data
      }
  if( TEMP2_CON == SENSOR_CONNECTED){
      sensor2.requestTemperatures(); // Send the command to get temperatures
      BEEHIVE.temp2_float = getTemperature_sensor2(insideThermometer2); // Use a simple function to print out the data
      }
  if( SCALE_CON == SENSOR_CONNECTED){
      WEIGHT_SENSOR_float = scale_readings();
      
      BEEHIVE.weight_float = WEIGHT_OFFSET_float + WEIGHT_SENSOR_float;
      /*
      if(BEEHIVE.weight_float < 0.0)
         BEEHIVE.weight_float = 0.0;        // in case correction factor from SD CArd is slightly different
     */
      }
  
  BEEHIVE.sound_int = getSound(analogSoundPin) ; // analogRead(analogSoundPin);      
  
}

void updateTFT(){
  static float temp1C_float = 0.0;
  static float temp2C_float = 0.0;
  static float weight_float = 0.0;
  static int   sound_int  = 0;
  static timeStruct_t timeNow, timeOld;
  DateTime now = rtc.now();
   
  // erase values
  tft.setCursor(0,80);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("BeeHive T: ");
  tft.println(temp1C_float);
  tft.print("Scale   T: ");
  tft.println(temp2C_float);
  tft.print("Scale  kg: ");
  tft.println(weight_float);
  tft.print("Sound dec: ");
  tft.println(sound_int);
  // time and date 
  tft.println("");
  tft.print(timeOld.day);
  tft.print("/");
  tft.print(timeOld.month);
  tft.print("/");
  tft.print(timeOld.year);
  tft.print(" ");
  tft.print(timeOld.hour);
  tft.print(":");
  tft.print(timeOld.min);
  tft.print(":");
  tft.println(timeOld.sec);

  // ---------------------------------------------

  // write new values
  tft.setCursor(0,80);
  tft.setTextColor(ILI9341_GREEN);   
  tft.print("BeeHive T: ");
  tft.println(BEEHIVE.temp1_float);
  tft.print("Scale   T: ");
  tft.println(BEEHIVE.temp2_float);
  tft.print("Scale  kg: ");
  tft.println(BEEHIVE.weight_float);
  tft.print("Sound dec: ");
  tft.println(BEEHIVE.sound_int);

  // current time and date
  timeNow.year  = now.year()-2000;
  timeNow.month = now.month();
  timeNow.day   = now.day();
  timeNow.hour  = now.hour();
  timeNow.min   = now.minute();
  timeNow.sec   = now.second();
  tft.println("");
  tft.print(timeNow.day);
  tft.print("/");
  tft.print(timeNow.month);
  tft.print("/");
  tft.print(timeNow.year);
  tft.print(" ");
  tft.print(timeNow.hour);
  tft.print(":");
  tft.print(timeNow.min);
  tft.print(":");
  tft.println(timeNow.sec);
  
  memcpy((void *)&timeOld, (void *)&timeNow, sizeof timeNow);
  
  temp1C_float = BEEHIVE.temp1_float;
  temp2C_float = BEEHIVE.temp2_float;
  weight_float = BEEHIVE.weight_float;
  sound_int  = BEEHIVE.sound_int;  
}
