#include "stdio.h"
#include "RTClib.h"



RTC_Millis rtc;
                              // 0  1  2  3  4  5  6
char Buffer_Softserial[100] =  {01,16,01,21,19,51,15};  

void setup () {
    Serial.begin(9600);

    // following line sets the RTC to the date & time this sketch was compiled
    rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));


    rtc.adjust(DateTime((uint16_t)Buffer_Softserial[3] + 2000,  // year
                        (uint8_t) Buffer_Softserial[2],     // month
                        (uint8_t) Buffer_Softserial[1],     // day
                        (uint8_t) Buffer_Softserial[4],     // hour
                        (uint8_t) Buffer_Softserial[5],     // min 
                        (uint8_t) Buffer_Softserial[6]));   // sec

   // 001 016 001 021 019 051 015
}



void loop() {
    printTime();
    delay(3000);
}



void printTime(){    
    
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}
