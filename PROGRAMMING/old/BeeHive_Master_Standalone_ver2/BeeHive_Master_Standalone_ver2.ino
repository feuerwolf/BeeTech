#include <Wire.h>               // for later
#include <SPI.h>
#include <SD.h>
//#include "SPI.h"
#include "Adafruit_MQTT.h"            // MQTT
#include "Adafruit_MQTT_Client.h"     // MQTT
#include <MKRGSM.h>                   // Board
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FRAM_SPI.h"

#define TFT_RST      18
#define TFT_DC       19
#define TFT_CS       7
#define TFT_EN       17

#define FRAM_CS      16

#define SDCARD_CS    6

#define LED_R        3
#define LED_G        11
#define LED_B        12


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
GSM gsmAccess;                        // GSM access: include a 'true' parameter for debug enabled
GPRS gprsAccess;                      // GPRS access

GSMClient client;  // Client service for TCP connection
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); 

//SENSOR BME280
Adafruit_MQTT_Publish bme280_temp     = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish bme280_humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish bme280_pressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Pressure");
Adafruit_MQTT_Publish hx711_weight    = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Weight");

//SENSOR I2S MICROPHONE
Adafruit_MQTT_Publish i2s_sound = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Sound");


void MQTT_connect();

Adafruit_ILI9341 tft   = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);  // using hardware SPI
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);                 // use hardware SPI


float temp_global     = 21.0;
float humidity_global = 0.0;
float pressure_global = 0.0;
float sound_global    = 0.0;
float weight_global   = 0.0;



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

    digitalWrite(TFT_EN, LOW);     

    initGsmMkr1400();
}

void loop()
{
  int i;
  MQTT_connect();
  
  /*
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on (HIGH is the voltage level)
  //digitalWrite(LED_G, HIGH);          // turn the LED on (HIGH is the voltage level)
  delay(1000);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);     // turn the LED off by making the voltage LOW
  //digitalWrite(LED_G, LOW);           // turn the LED on (HIGH is the voltage level)
  delay(1000);     
*/
/*
  getValues();
  printValues();
  writevalues();
*/
  Serial.print("\nSending bme280_temp val ");         //publish bme280_temp
  if (! bme280_temp.publish(temp_global)) 
      Serial.println("Failed Temperature");
  else 
      Serial.println("Temp OK!");
  /*
  Serial.print("\nSending bme280_humidity val ");      //publish bme280_humidity
  if (! bme280_humidity.publish(50)) 
      Serial.println("Failed Humidity");
  else 
      Serial.println("Humidity OK!");

  Serial.print("\nSending bme280_pressure val ");       //publish bme280_pressure
  if (! bme280_pressure.publish(980)) 
      Serial.println("Failed Pressure");
  else 
      Serial.println("Pressure OK!");

  Serial.print("\nSending hx711_weight val ");       //publish hx711_weight
  if (! hx711_weight.publish(30)) 
      Serial.println("Failed Weight");
  else 
      Serial.println("Weight OK!");
*/
  delay(5000);



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
