// SLAVE3 SKETCH

#include <SoftwareSerial.h>                     // Library für Software-Serial
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "HX711.h"                              // Library für Waage (HX711 Amplifier)
#define I2C_ADDRESS 0x3C
#define calibration_factor -5640                // Wird mit Calibration Scale Sketch definiert!

SSD1306AsciiWire oled;
HX711 scale;

#define digiPin_0 2                             // binär Adresse für die Adressfindung vom Slave
#define digiPin_1 3
#define digiPin_2 4
#define digiPin_3 5

#define DOUT  11                                // Data pin for Scale!
#define CLK  12                                 // Clock pin for Scale!

/*
 * byte 8x stellen == 8 x 0 oder 1 kombination = max 0 bis 256
 * int 16x stellen == 2 X 8 (2 BYTES)  = max 2^16
 * Float 32 stellen = max 2^32
 * 
 * geschätzte kilos zwischen 10kg und 100kg
 * --> mit byte zur verfügung zahlen bereich 0 - 256
 * bsp:
 * 5.6 kg = 56 = 5 kg und 600 g
 * 10.5 kg   = 105  = 10 kg 100 gramm
 * 88.223 kg = 882 error wei max 256
 * 
 */

/* mit serial write kann nur 1 byte (8bit) übertragen werden
 * char   = 0b 1010 1010 = 1 byte = 8bit = zahlenbereich 0 - 255
 * int   =  0b 1010 1010 1010 1010 = 2 byte 16 bit = zahlenbereich 0 bis 2^16
 * float =  0b 1010 1010 1010 1010 1010 1010 1010 1010 = 4 byte = 32 bit = zahlenbereich 0 bis 2^32
 * 
 * char ergebnis = 3 * 3 + 1;   // 12 
 * char ergebnis = (3 * 3) + 1; // 10 
 */

char temperatureData = 27; // 0x
char humidityData    = 80;
char soundData       = 70;
float weightData_float = 50.0;

int SensorNumberGlobal = 0;

SoftwareSerial softSerial(10, 11); // RX, TX

int calculateSensorNumber() {                   // Calculate your own Sensor number
  int numberOfSensor = 0;

  numberOfSensor = digitalRead(digiPin_3);
  numberOfSensor = numberOfSensor << 1;

  numberOfSensor = numberOfSensor | digitalRead(digiPin_2);
  numberOfSensor = numberOfSensor << 1;

  numberOfSensor = numberOfSensor | digitalRead(digiPin_1);
  numberOfSensor = numberOfSensor << 1;

  numberOfSensor = numberOfSensor | digitalRead(digiPin_0);

  return numberOfSensor;
 
}

void setup() {
  Serial.begin(9600); // Serial Monitor via USB
  Serial1.begin(9600); // Hardware Serial Port (gesteckt) at Pin1=TX, Pin0=RX

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(digiPin_0, INPUT);
  pinMode(digiPin_1, INPUT);
  pinMode(digiPin_2, INPUT);
  pinMode(digiPin_3, INPUT);

  blinkdelay(3);
  Serial.print("starting up...");
  Serial.println("waiting for stuff");

  softSerial.begin(9600);  // Software Serial Port at Pin10=RX, Pin11=TX
  softSerial.println("Hello, I am the new Serial!");

  SensorNumberGlobal = calculateSensorNumber();

  Serial.println("Slave started...");
  Serial.print("Sensor Number = ");
  Serial.println(SensorNumberGlobal);

  wireBegin();
  oled.setFont(System5x7);
  oled.clear();
  oled.println("display...");
  oled.print("...fucking works");
  delay(1000);
  oled.clear();

}

void loop() {
  byte weigtInByte = 0;
  int weightInInt = 0;
  char * ptr;
  
  //temperatureData += 1;            // oder: temperaturedata = temperatureData + 1;
  //humidityData += 1;
  //soundData += 1;

  if (softSerial.available()) {         // if anything comes in SoftwareSerial (from slave)
    Serial1.write(softSerial.read());   // read it and send it out on Serial1 (to master)
  }

  if (Serial1.available()) {            // If anything comes in Serial1 (from master)
    char inChar = (char)Serial1.read();
    if (inChar == SensorNumberGlobal) { // if it's us
    //if (inChar == 0x31) { // if it's us
      blinkfast();
      Serial.println("Received request - oh shit it's me");
      // step 1 getsensordata and send them back
      //getWeightData();

      
      Serial1.write(temperatureData);  // 0d 27 == 0x 1B == 0b 0011 0011 == 1 Byte
      Serial.print(temperatureData);
      
      Serial1.write(humidityData);
      Serial.print(humidityData);
      
      Serial1.write(soundData);
      Serial.print(soundData);
      
      weightData_float = 53.5;  // in 0b 01000010 01010110 00000000 00000000
                                // in 0x    42       56       00       00
                                // in 0d    66       86       00       00 
      
      ptr = (char) &weightData_float;
      Serial1.write(ptr); 
      Serial.print(ptr);  // test on terminal     
      ptr++;
      Serial1.write(ptr); 
      Serial.print(ptr);        
      ptr++;
      Serial1.write(ptr);
      Serial.print(ptr);         
      ptr++;
      Serial1.write(ptr);
      Serial.print(ptr); 
              
      Serial1.write(0x0D);             // character return (neue Line)

      
      /*
        weightInInt = (int) weightData_float * 10; // make 66.4234 kg to weihgt value of 664.234 in 100g steps 
                                           // and cut off nachkommazahlen 
                                           // now value is something lieke 664 in 100g steps == 66.4kg
                                           // 664 DEZ == 0b 00000010 10011000      * 
       * 
      Serial1.write(temperatureData);  // 0d 27 == 0x 1B == 0b 0011 0011 == 1 Byte
      Serial1.write(',');             // send comma
      Serial1.write(humidityData);
      Serial1.write(0x2C);             // send comma
      Serial1.write(soundData);
      Serial1.write(0x2C);             // send comma
      // transmitt weight
      // Serial1.write((int)weightData);
      // byte = 0b 00000000
      // int  = 0b 01000010 00011000
      weigtInByte = (byte) weightInInt;
      Serial1.write(weigtInByte);         // hier wird: 00011000 gesendet
      
      weigtInByte = (byte) (8 >> weightInInt);  // hier wird 00011000 nach rechs "hinaus" geschoben und 01000010 bleibt
      Serial1.write( weigtInByte);   // 01000010 steht in weightData
      
      Serial1.write(0x2C);             // send comma 
      Serial1.write(0x0D);             // character return (neue Line)
      */
    }
    else{
      softSerial.write(inChar);       // forward inChar to next sensor
      Serial.println("Received request - it's not me");
    } 
  }
  
//display Data on OLED-Display, then carry on
  displayData();
  delay(5000);
  
}

// blinks five times in one second
void blinkfast() {
    for (int i = 0; i <= 5; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                        // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  }
}

// blinks n seconds 
void blinkdelay(int delayInSeconds) {
  for (int i = 0; i < delayInSeconds; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                        // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(500);
  }
}

void wireBegin() {
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
}

void displayData() {
  oled.clear();
  oled.print("Temperature: ");
  oled.println(temperatureData);
  oled.print("Humidity: ");
  oled.println(humidityData);
  oled.print("Sound: ");
  oled.println(soundData);
  oled.print("Weight:" );
  oled.print(weightData_float);
  
}

void getWeightData() {
    // reading average data from scale  
    scale.begin(DOUT, CLK);
    scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch 
    scale.tare();
   
    weightData_float = scale.get_units(5);

}
