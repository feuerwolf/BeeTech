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
 



// --- Temperature Sensors --------------------------------------

OneWire oneWire_Scale(ONE_WIRE_SENSOR_Scale);             // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor_Temp_Scale(&oneWire_Scale);      // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer_Scale;                         // arrays to hold device address

OneWire oneWire_BeeHive(ONE_WIRE_SENSOR_BeeHive);             // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensor_Temp_BeeHive(&oneWire_BeeHive);      // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer_BeeHive;                             // arrays to hold device address


// --------------------------------------------------------------



void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  /*  */
    
    Serial.println("Arduino Started");
    
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_R,OUTPUT);
    pinMode(LED_G,OUTPUT);
    pinMode(LED_B,OUTPUT);
    

    //framTest();
    //sdCardTest();
    testLeds();
    LED_G_ON;
    LED_B_OFF; 

   
    scale_initialize();
}

void loop(){

scale_realAll();

delay(3000);

  
}


void loopx()
{
  float weight_float = 0.0;
 
  weight_float = scale_readings();
 
  Serial.println(weight_float);
  
  delay(3000);

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
