#include <Wire.h>               // for later
#include <SPI.h>
#include <SD.h>
//#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FRAM_SPI.h"

#define TFT_RST      18
#define TFT_DC       19
#define TFT_CS       7
#define TFT_EN       17

//#define FRAM_CS      16
uint8_t FRAM_CS = 16;

//#define SDCARD_CS    6
//const int chipSelect = 6;

#define LED_R        3
#define LED_G        11
#define LED_B        12


Adafruit_ILI9341 tft   = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);  // using hardware SPI
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);                 // use hardware SPI
uint16_t          addr = 0;



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
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(TFT_EN, OUTPUT);

    //framTest();
    //sdCardTest();
    //testLeds();
    
    digitalWrite(TFT_EN, HIGH);   // turn the LED on (HIGH is the voltage level)
    init_tft();

    /*
    tft.fillScreen(ILI9341_BLACK);
    showRandomCircles();
    tft.fillScreen(ILI9341_BLACK);
    FastLines(ILI9341_RED, ILI9341_BLUE);
    tft.fillScreen(ILI9341_BLACK);
    */
    showLines(ILI9341_CYAN);
    tft.fillScreen(ILI9341_BLACK);
    digitalWrite(TFT_EN, LOW);     
}

void loop()
{
  Serial.println("Arduino test"); 
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on (HIGH is the voltage level)
  //digitalWrite(LED_G, HIGH);          // turn the LED on (HIGH is the voltage level)
  delay(1000);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);     // turn the LED off by making the voltage LOW
  //digitalWrite(LED_G, LOW);           // turn the LED on (HIGH is the voltage level)
  delay(1000);     
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
