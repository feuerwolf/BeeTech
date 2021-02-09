#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>      // for later
#include <Wire.h>               // for later

#define ONE_WIRE_SENSOR_1 6  // Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_SENSOR_2 7  // Data wire is plugged into port 2 on the Arduino

#define TFT_RST      8
#define TFT_DC       9
#define TFT_CS       10
#define TFT_EN       15


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

int serialNumber_int = 0;           // 1 byte
int calibration_factor_int = 0;   // 2 bytes
char stationName[100] = "";    // max 100 bytes

const int addr_serialNumber = 0;    // 1  byte
const int addr_scaleVale    = 2;    // 2 bytes
const int addr_stationName  = 10;   // 100 bytes


void setup()
{
    //Serial.begin(9600);
    Serial.begin(9600, SERIAL_8N1);
    delay(1000);
    Serial.println("Arduino Started");

    neopix.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

    //neopix.setPixelColor(0, neopix.Color(0, 150, 0));  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    setNeoPixColor_GREEN;
    neopix.setBrightness(20); // Set BRIGHTNESS to about 1/5 (max = 255)
    neopix.show();   // Send the updated pixel colors to the hardware.

    getEepromConfigData();

    init_DS18B20_Temp1();
    init_DS18B20_Temp2();

    pinMode(TFT_EN, OUTPUT);
    digitalWrite(TFT_EN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500); 
    init_tft();
    
    tft.fillScreen(ILI9341_BLACK);
    showRandomCircles();
    tft.fillScreen(ILI9341_BLACK);
  
    tft.setCursor(0,0);
    tft.setTextColor(ILI9341_GREEN);   
    tft.println(stationName);
    tft.print("Number ");
    tft.println(serialNumber_int);
    tft.setCursor(0,60);
    setNeoPixColor_BLUE;


    
}

void loop()
{
  float temp1C_float = 0.0;
  float temp2C_float = 0.0;
  
  sensor1.requestTemperatures(); // Send the command to get temperatures
  temp1C_float = printTemperature_sensor1(insideThermometer1); // Use a simple function to print out the data
  sensor2.requestTemperatures(); // Send the command to get temperatures
  temp2C_float = printTemperature_sensor2(insideThermometer2); // Use a simple function to print out the data
   
  tft.setCursor(0,60);
  tft.setTextColor(ILI9341_GREEN);   
  tft.print("BeeHive : ");
  tft.println(temp1C_float);
  tft.print("Scale   : ");
  tft.println(temp2C_float);
  setNeoPixColor_GREEN;

  delay(5000); 
  setNeoPixColor_BLUE;
  delay(200); 
  // erase strings on Display
  //tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0,60);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("SENSOR 1: ");
  tft.println(temp1C_float);
  tft.print("SENSOR 2: ");
  tft.println(temp2C_float);
   
}
