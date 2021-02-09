#include <Wire.h>               // for later
#include <SPI.h>
#include <SD.h>

#include <MKRGSM.h>                   // Board
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FRAM_SPI.h"
#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_SENSOR_Scale   4  // Data wire is plugged into port 1 on the Arduino
#define ONE_WIRE_SENSOR_BeeHive 5  // Data wire is plugged into port 2 on the Arduino


#define HX_DOUT      1
#define HX_CLK       2

#define TFT_RST      18
#define TFT_DC       19
#define TFT_CS       7
#define TFT_EN       20

#define FRAM_CS      16

#define SDCARD_CS    6

#define LED_R        3
#define LED_G        11
#define LED_B        12

#define LED_R_OFF  digitalWrite(LED_R,LOW);   
#define LED_G_OFF  digitalWrite(LED_G,LOW);   
#define LED_B_OFF  digitalWrite(LED_B,LOW);   

#define LED_R_ON   digitalWrite(LED_R,HIGH);  
#define LED_G_ON   digitalWrite(LED_G,HIGH);  
#define LED_B_ON   digitalWrite(LED_B,HIGH);  
 

#define SEALEVELPRESSURE_HPA (1013.25)

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883          // use 8883 for SSL
#define AIO_USERNAME    "windline"
#define AIO_KEY         "dbdcdd631a66407ba5d063fbeebb5808"

#define GSM_APN         "TM"          // GSM
#define GSM_PIN         ""            // GSM
#define GSM_LOGIN       ""            // GSM
#define GSM_PASSWORT    ""            // GSM
const char PINNUMBER[] = "";          // GSM



// --- Temperature Sensors --------------------------------------

OneWire oneWire_Scale(ONE_WIRE_SENSOR_Scale);             // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor_Temp_Scale(&oneWire_Scale);      // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer_Scale;                         // arrays to hold device address

OneWire oneWire_BeeHive(ONE_WIRE_SENSOR_BeeHive);             // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor_Temp_BeeHive(&oneWire_BeeHive);      // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer_BeeHive;                             // arrays to hold device address


// --------------------------------------------------------------


Adafruit_ILI9341 tft   = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);  // using hardware SPI
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);                 // use hardware SPI


void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    /*  
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  */
    
    Serial.println("Arduino Started");
    
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_R,OUTPUT);
    pinMode(LED_G,OUTPUT);
    pinMode(LED_B,OUTPUT);
    pinMode(TFT_EN,OUTPUT);

    //framTest();
    //sdCardTest();
    testLeds();
    LED_G_ON;
    LED_B_OFF; 

    init_DS18B20_Temp_Scale();
    init_DS18B20_Temp_BeeHive();

    scale_initialize();
    
    digitalWrite(TFT_EN, HIGH);   // turn the LED on (HIGH is the voltage level)
    init_tft();  
    
  
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0,0);
   
    
}

void loop()
{
  float temp_beehive_float = 0.0;
  float temp_scale_float = 0.0;
  float weight_float = 0.0;
  float sound_float  = 558.0;

  
  sensor_Temp_Scale.requestTemperatures(); // Send the command to get temperatures
  temp_scale_float   = getTemperature_sensor_Scale(insideThermometer_Scale); // Use a simple function to print out the data
  sensor_Temp_BeeHive.requestTemperatures(); // Send the command to get temperatures
  temp_beehive_float = getTemperature_sensor_BeeHive(insideThermometer_BeeHive); // Use a simple function to print out the data
  weight_float = scale_readings();
  sound_float = (float)analogRead(A0);
  
 
  
  Serial.print(temp_beehive_float);
  Serial.print(", ");
  Serial.print(temp_scale_float);
  Serial.print(", ");
  Serial.print(weight_float);
  Serial.print(", ");
  Serial.print(sound_float);
  Serial.println(", ");

  tft.setCursor(0,60);
  tft.setTextColor(ILI9341_GREEN);   
 
  tft.println("");
  tft.print("BeeHive T: ");
  tft.println(temp_beehive_float);
  tft.print("Scale   T: ");
  tft.println(temp_scale_float);
  tft.print("Scale  kg: ");
  tft.println(weight_float);
  tft.print("Sound    : ");
  tft.println(sound_float);

  delay(3000);

  // erase strings on Display
  tft.setCursor(0,60);
  tft.setTextColor(ILI9341_BLACK);
 
  tft.println("");
  tft.print("BeeHive T: ");
  tft.println(temp_beehive_float);
  tft.print("Scale   T: ");
  tft.println(temp_scale_float);
  tft.print("Scale  kg: ");
  tft.println(weight_float);
  tft.print("Sound    : ");
  tft.println(sound_float);
}




void testLeds(){
  int i = 0;
  digitalWrite(LED_R, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_G, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_B, LOW);   // turn the LED on (HIGH is the voltage level)
    
  for(i = 0; i<10;i++){
      digitalWrite(LED_R, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_G, LOW);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_B, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(50);  
   
      digitalWrite(LED_R, LOW);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_G, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_B, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(50);  
  
      digitalWrite(LED_R, LOW);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_G, LOW);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_B, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(50);    
  }
  digitalWrite(LED_R, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_G, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_B, LOW);   // turn the LED on (HIGH is the voltage level)
  
}
