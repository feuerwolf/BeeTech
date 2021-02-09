

/*
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883          // use 8883 for SSL
#define AIO_USERNAME    "windline"
#define AIO_KEY         "dbdcdd631a66407ba5d063fbeebb5808"

#define SEALEVELPRESSURE_HPA (1013.25)

#define GSM_APN         "TM"          // GSM
#define GSM_PIN         ""            // GSM
#define GSM_LOGIN       ""            // GSM
#define GSM_PASSWORT    ""            // GSM
const char PINNUMBER[] = "";          // GSM
GSM gsmAccess;                        // GSM access: include a 'true' parameter for debug enabled
GPRS gprsAccess;                      // GPRS access
*/

String oktext         = "Starting serial Monitor"; // messages for serial monitor response
String errortext      = "ERROR";
char url[]            = "io.adafruit.com"; // URL and path (for example: example.org)
char urlproxy[]       = "https://io.adafruit.com";
char path[]           = "/";
String response       = "";// variable for save response obtained
bool use_proxy        = false;// use a proxy

/*
GSMClient client;  // Client service for TCP connection
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); 

//SENSOR BME280
Adafruit_MQTT_Publish bme280_temp     = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish bme280_humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish bme280_pressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Pressure");
Adafruit_MQTT_Publish hx711_weight    = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Weight");

//SENSOR I2S MICROPHONE
Adafruit_MQTT_Publish i2s_sound = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Sound");
*/
//void MQTT_connect();


void initGsmMkr1400(){

  // GSM CONNECTION BLOCK -------------------------------------------------
    Serial.print("Connecting GSM network...");
    //writeconnectingGSM();
    if (gsmAccess.begin(PINNUMBER) != GSM_READY) {
    Serial.println(errortext);
    while (true);
    }
    Serial.println(oktext);
    Serial.println("Attaching to GPRS with your APN...");  // ATTACH GPRS --------------
    //writeconnectingAPN();
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
      Serial.println("ok");
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
    //writeGSMconnected();
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
