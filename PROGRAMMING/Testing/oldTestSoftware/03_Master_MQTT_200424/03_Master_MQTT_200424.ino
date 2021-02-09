
#include "Adafruit_MQTT.h"            // MQTT
#include "Adafruit_MQTT_Client.h"     // MQTT
#include "HX711.h"                    // HX711
#include <MKRGSM.h>                   // Board
#include <Adafruit_GFX.h>             // Display
#include <Adafruit_SSD1306.h>         // Display
#include <Wire.h>                     // Wire
#include <Adafruit_Sensor.h>          // BME Sensor
#include <Adafruit_BME280.h>          // BME Sensor

#define calibration_factor -23750 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define SCREEN_WIDTH 128              // OLED display width, in pixels
#define SCREEN_HEIGHT 64              // OLED display height, in pixels
#define OLED_RESET     -1              // Reset pin # (or -1 if sharing Arduino reset pin)
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883          // use 8883 for SSL
#define AIO_USERNAME    "windline"
#define AIO_KEY         "dbdcdd631a66407ba5d063fbeebb5808"
#define BME_SCK       13              // BME Sensor
#define BME_MISO      12              // BME Sensor
#define BME_MOSI      11              // BME Sensor
#define BME_CS        10              // BME Sensor
#define SEALEVELPRESSURE_HPA (1013.25)
#define DOUT          5               // for HX711
#define CLK           4               // for HX711
#define GSM_APN         "TM"          // GSM
#define GSM_PIN         ""            // GSM
#define GSM_LOGIN       ""            // GSM
#define GSM_PASSWORT    ""            // GSM
const char PINNUMBER[] = "";          // GSM
GSM gsmAccess;                        // GSM access: include a 'true' parameter for debug enabled
GPRS gprsAccess;                      // GPRS access

HX711 scale;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Display
Adafruit_BME280 bme;                  // I2C

unsigned long delayTime;              // Sensor

float temp_global     = 0.0;
float humidity_global = 0.0;
float pressure_global = 0.0;
float sound_global    = 0.0;
float weight_global   = 0.0;

String oktext         = "Starting serial Monitor"; // messages for serial monitor response
String errortext      = "ERROR";
char url[]            = "io.adafruit.com"; // URL and path (for example: example.org)
char urlproxy[]       = "https://io.adafruit.com";
char path[]           = "/";
String response       = "";// variable for save response obtained
bool use_proxy        = false;// use a proxy


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

void setup() {
  Serial.begin(115200);
  delay(10);

  // DISPLAY CONFIG BLOCK ----------------------------------------------------------
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  writestarting();
  delay(2000);
  display.clearDisplay();               // Clear the buffer
  
  //  SENSOR CONFIG BLOCK ---------------------------------------------------

  Serial.println(F("BME280 test"));     // BME
  bool status;
  status = bme.begin();  
  if (status == 0) 
      Serial.println("Could not find BME280 sensor, oh no!");
  else
      Serial.println("found Sensor BME280 - all is fine");

  delayTime = 1000;
  Serial.println();
 Serial.println("now start:");
 scale.begin(DOUT, CLK);              // SCALE
  Serial.println("done begin:");
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
   Serial.println("done calibration:");
 scale.tare(1); //Assuming there is no weight on the scale at start up, reset the scale
  Serial.println("done tare:");


  // GSM CONNECTION BLOCK -------------------------------------------------
    Serial.print("Connecting GSM network...");
    writeconnectingGSM();
    if (gsmAccess.begin(PINNUMBER) != GSM_READY) {
    Serial.println(errortext);
    while (true);
    }
    Serial.println(oktext);
    Serial.println("Attaching to GPRS with your APN...");  // ATTACH GPRS --------------
    writeconnectingAPN();
    if (gprsAccess.attachGPRS(GSM_APN, GSM_LOGIN, GSM_PASSWORT) != GPRS_READY) {
    Serial.println(errortext);
    } else {
    Serial.println(oktext);
    
    char proxy[100];   // READ PROXY THING ---
    Serial.print("If your carrier uses a proxy, enter it, if not press enter: ");
    //readSerial(proxy);
    Serial.println(proxy);
    proxy[0] = '\0';
    // if user introduced a proxy, asks them for proxy port
    int pport;
    if (proxy[0] != '\0') {
      // read proxy port introduced by user
      char proxyport[10];
      Serial.print("Enter the proxy port: ");
      readSerial(proxyport);
      // cast proxy port introduced to integer
      pport = (int) proxyport;
      use_proxy = true;
      Serial.println(proxyport);
    }

    // CONNECTING WITH WEBSITE ----------------------------------------------
    Serial.print("Connecting and sending GET request to io.adafruit.com...");
    int res_connect;

    // if use a proxy, connect with it
    if (use_proxy) {
      res_connect = client.connect(proxy, pport);
    } else {
      res_connect = client.connect(url, 80);
    }

    if (res_connect) {
      // make a HTTP 1.0 GET request (client sends the request)
      client.print("GET ");

      // if use a proxy, the path is example.org URL
      if (use_proxy) {
        client.print(urlproxy);
      } else {
        client.print(path);
      }

      client.println(" HTTP/1.0");
      client.println();
      Serial.println(oktext);
    } else {
      // if you didn't get a connection to the server
      Serial.println(errortext);
    }
    Serial.print("Receiving response...");

    bool test = true;
    while (test) {
      // if there are incoming bytes available
      // from the server, read and check them
      if (client.available()) {
        char c = client.read();
        response += c;

        // cast response obtained from string to char array
        char responsechar[response.length() + 1];
        response.toCharArray(responsechar, response.length() + 1);

        // if response includes a "200 OK" substring
        if (strstr(responsechar, "200 OK") != NULL) {
          Serial.println(oktext);
          Serial.println("TEST COMPLETE!");
          test = false;
        }
      }

      // if the server's disconnected, stop the client:
      if (!client.connected()) {
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();
        test = false;
      }
    }
  }
    Serial.println(F("Adafruit MQTT"));
    writeGSMconnected();

}



void loop() {
  int i;
  MQTT_connect();
  getValues();
  printValues();
  writevalues();

  Serial.print("\nSending bme280_temp val ");         //publish bme280_temp
  if (! bme280_temp.publish(temp_global)) 
      Serial.println("Failed Temperature");
  else 
      Serial.println("Temp OK!");
  
  Serial.print("\nSending bme280_humidity val ");      //publish bme280_humidity
  if (! bme280_humidity.publish(humidity_global)) 
      Serial.println("Failed Humidity");
  else 
      Serial.println("Humidity OK!");

  Serial.print("\nSending bme280_pressure val ");       //publish bme280_pressure
  if (! bme280_pressure.publish(pressure_global)) 
      Serial.println("Failed Pressure");
  else 
      Serial.println("Pressure OK!");

  Serial.print("\nSending hx711_weight val ");       //publish hx711_weight
  if (! hx711_weight.publish(weight_global)) 
      Serial.println("Failed Weight");
  else 
      Serial.println("Weight OK!");

  delay(60000);
}


void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

// + SENSOR STUFF ------------------------------------------------------------------

void getValues(){
  float temp_intern     = 0.0;
  float humidity_intern = 0.0;
  float pressure_intern = 0.0;
  float sound_intern    = 0.0;
  float weight_intern   = 0.0;


  temp_intern           = bme.readTemperature();
  pressure_intern       = bme.readPressure() / 100.0;
  humidity_intern       = bme.readHumidity();
  weight_intern         = scale.get_units();
  
  temp_global = temp_intern;
  pressure_global = pressure_intern;
  humidity_global = humidity_intern;
  weight_global = weight_intern;
    
}




void printValues() {
    Serial.print("Temperature = ");
    Serial.print(temp_global);
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(pressure_global);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(humidity_global);
    Serial.println(" %");

    Serial.print("Weight = ");
    Serial.print(weight_global);
    Serial.println(" Kg");

    Serial.println();
}


int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}


void writevalues(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Sending Data..."));
  
  display.println(F(""));

  display.print(pressure_global);
  display.println(" muM");
  
  display.println(F(""));
  
  display.print("Temp: ");
  display.println(temp_global);
  display.print("Humi: ");
  display.println(humidity_global);
  display.print("Kg:   ");
  display.print(weight_global);
  
 
  display.display();
  delay(2000);
}
