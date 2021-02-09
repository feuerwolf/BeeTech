#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <OneWire.h>
#include <DallasTemperature.h>



#define ONE_WIRE_SENSOR_1 6  // Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_SENSOR_2 5  // Data wire is plugged into port 2 on the Arduino

#define TFT_RST      8
#define TFT_DC       9
#define TFT_CS       10


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);  // using hardware SPI

OneWire oneWire1(ONE_WIRE_SENSOR_1);        // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor1(&oneWire1);  // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer1;      // arrays to hold device address

OneWire oneWire2(ONE_WIRE_SENSOR_2);        // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor2(&oneWire2);  // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer2;      // arrays to hold device address



void setup()
{
    Serial.begin(9600);
    delay(1000);
    Serial.println("Arduino Started");
    
    init_DS18B20_Temp1();
    init_DS18B20_Temp2();
    
    init_tft();
    
    tft.fillScreen(ILI9341_BLACK);
    showRandomCircles();

    tft.fillScreen(ILI9341_BLACK);
}

void loop()
{
  float temp1C_float = 0.0;
  float temp2C_float = 0.0;
  
  sensor1.requestTemperatures(); // Send the command to get temperatures
  temp1C_float = printTemperature_sensor1(insideThermometer1); // Use a simple function to print out the data
  sensor2.requestTemperatures(); // Send the command to get temperatures
  temp2C_float = printTemperature_sensor2(insideThermometer2); // Use a simple function to print out the data
   
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_GREEN);   
  tft.print("SENSOR 1: ");
  tft.println(temp1C_float);
  tft.print("SENSOR 2: ");
  tft.println(temp2C_float);

  delay(5000); 
  // erase strings on Display
  //tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("SENSOR 1: ");
  tft.println(temp1C_float);
  tft.print("SENSOR 2: ");
  tft.println(temp2C_float);
   
}
