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
float humidity_global = 50.0;
float pressure_global = 950.0;
float sound_global    = 30.0;
float weight_global   = 75.0;



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
    digitalWrite(TFT_EN, HIGH);   // turn the LED on (HIGH is the voltage level)
    init_tft();  
    initGsmMkr1400();

    tft.fillScreen(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0,0);
    
}

void loop()
{
  int i;
  float temp1C_float = 23.1;
  float temp2C_float = 19.0;
  float weight_float = 22.2;
  float sound_float  = 558.0;
  MQTT_connect();
  
  /*
  getValues();
  printValues();
  writevalues();
  */
  
  
  LED_G_OFF;
  LED_B_ON; 
  
  Serial.print(temp1C_float);
  Serial.print(", ");
  Serial.print(temp2C_float);
  Serial.print(", ");
  Serial.print(weight_float);
  Serial.print(", ");
  Serial.print(sound_float);
  Serial.println(", ");

  if (! bme280_temp.publish(temp1C_float)) 
      Serial.println("Failed to send Temperature");
   /*
  if (! bme280_humidity.publish(50)) 
      Serial.println("Failed Humidity");
  if (! bme280_pressure.publish(980)) 
      Serial.println("Failed Pressure");
  if (! hx711_weight.publish(30)) 
      Serial.println("Failed Weight");
  */
   
  tft.setCursor(0,60);
  tft.setTextColor(ILI9341_GREEN);   
  tft.println("uploading data... OK");
  tft.println("");
  tft.print("BeeHive T: ");
  tft.println(temp1C_float);
  tft.print("Scale   T: ");
  tft.println(temp2C_float);
  tft.print("Scale  kg: ");
  tft.println(weight_float);
  tft.print("Sound    : ");
  tft.println(sound_float);

  LED_G_ON;
  LED_B_OFF; // green for 10 sec
  delay(2000); 
  LED_B_ON; 
  LED_G_OFF;
  // erase strings on Display
  tft.setCursor(0,60);
  tft.setTextColor(ILI9341_BLACK);
  tft.println("uploading data... OK");
  tft.println("");
  
  delay(8000);
  tft.print("BeeHive T: ");
  tft.println(temp1C_float);
  tft.print("Scale   T: ");
  tft.println(temp2C_float);
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
