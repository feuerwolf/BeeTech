


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
    tft.println("");
    tft.print("Connecting GSM ... ");
    
    if (gsmAccess.begin(PINNUMBER) != GSM_READY) {
        Serial.println("error");
        while (true);
        }
    tft.println("OK");
    Serial.println("OK");

    Serial.println("Attaching to GPRS with your APN...");  // ATTACH GPRS --------------
    tft.print("Attaching APN ... ");
    if (gprsAccess.attachGPRS(GSM_APN, GSM_LOGIN, GSM_PASSWORT) != GPRS_READY) {
        Serial.println("ERROR");
        tft.println("ERROR");
    } 
    else {
        Serial.println(" OK");
        tft.println("OK");
        char proxy[100];   // READ PROXY THING ---
        //Serial.print("If your carrier uses a proxy, enter it, if not press enter: ");
        //readSerial(proxy);
        Serial.println(proxy);
        proxy[0] = '\0';
        // if user introduced a proxy, asks them for proxy port
        int pport;
        if (proxy[0] != '\0') {
            // read proxy port introduced by user
            char proxyport[10];
            //Serial.print("Enter the proxy port: ");
            readSerial(proxyport);
            // cast proxy port introduced to integer
            pport = (int) proxyport;
            use_proxy = true;
            //Serial.println(proxyport);
            }
    
        // CONNECTING WITH WEBSITE ----------------------------------------------
        Serial.print("Connecting and sending GET request to io.adafruit.com...");
        tft.print("Connect to Cloud ...");
        
        int res_connect;
    
        // if use a proxy, connect with it
        if (use_proxy) {
            res_connect = client.connect(proxy, pport);
            } 
        else{
            res_connect = client.connect(url, 80);
            }
    
        if (res_connect) {
            // make a HTTP 1.0 GET request (client sends the request)
            client.print("GET ");
      
            // if use a proxy, the path is example.org URL
            if (use_proxy) {
                client.print(urlproxy);
                } 
            else{
                 client.print(path);
                 }
      
            client.println(" HTTP/1.0");
            client.println();
            
            Serial.println("ok");
            tft.println("OK");
            } 
        else{
             // if you didn't get a connection to the server
            Serial.println("error");
        }
        Serial.print("Receiving response...");
        tft.println("Received response ... OK");
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
                        Serial.println("OK");
                        Serial.println("TEST COMPLETE!");         
                        test = false;
                  }
            }  
            // if the server's disconnected, stop the client:
            if (!client.connected()) {
                  Serial.println();
                  Serial.println("disconnecting");
                  tft.println("disconnecting ... OK");
                  client.stop();
                  test = false;
            }
        }
    }
    Serial.println(F("Adafruit MQTT"));
    tft.println("IoT Cloud init... OK");
    delay(5000);
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  //tft.print("Connecting to Cloud...");
  
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       //tft.println("Error");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  //tft.println("OK");
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
