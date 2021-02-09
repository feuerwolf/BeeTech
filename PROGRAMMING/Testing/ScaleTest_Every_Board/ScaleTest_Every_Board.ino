#include <Wire.h>               // for later
#include <SPI.h>
#include <SD.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>


#define HX_DOUT      16
#define HX_CLK       17

#define ONE_WIRE_SENSOR_Scale   4  // Data wire is plugged into port 1 on the Arduino
#define ONE_WIRE_SENSOR_BeeHive 5  // Data wire is plugged into port 2 on the Arduino


#define TFT_RST      8
#define TFT_DC       9
#define TFT_CS       10
#define TFT_EN       15


#define SDCARD_CS    3

 


// --- Temperature Sensors --------------------------------------

OneWire oneWire_Scale(ONE_WIRE_SENSOR_Scale);             // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor_Temp_Scale(&oneWire_Scale);      // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer_Scale;                         // arrays to hold device address

OneWire oneWire_BeeHive(ONE_WIRE_SENSOR_BeeHive);             // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor_Temp_BeeHive(&oneWire_BeeHive);      // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer_BeeHive;                             // arrays to hold device address


// --------------------------------------------------------------


Adafruit_ILI9341 tft   = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);  // using hardware SPI


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
    
  
    pinMode(TFT_EN,OUTPUT);
    digitalWrite(TFT_EN, HIGH);
    
    
    //sdCardTest();


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
  tft.println("uploading data... OK");
  tft.println("");
  tft.print("BeeHive T: ");
  tft.println(temp_beehive_float);
  tft.print("Scale   T: ");
  tft.println(temp_scale_float);
  tft.print("Scale  kg: ");
  tft.println(weight_float);
  tft.print("Sound    : ");
  tft.println(sound_float);


  // erase strings on Display
  tft.setCursor(0,60);
  tft.setTextColor(ILI9341_BLACK);
  tft.println("uploading data... OK");
  tft.println("");
  
  delay(3000);
  tft.print("BeeHive T: ");
  tft.println(temp_beehive_float);
  tft.print("Scale   T: ");
  tft.println(temp_scale_float);
  tft.print("Scale  kg: ");
  tft.println(weight_float);
  tft.print("Sound    : ");
  tft.println(sound_float);
}
