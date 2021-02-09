#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_CLK  13

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

void setup()
{
    tft.begin();
    tft.fillScreen(ILI9341_BLACK);

    showRandomCircles();
    delay(3000);
}

void loop()
{
    for(byte i=0; i<4; i++) {
        tft.setRotation(i);
        showText();
        delay(2500);
    }
}

void showRandomCircles()
{
    for(byte i=0; i<255; i++) {
        tft.drawCircle(random(tft.width()), random(tft.height()), random(20), ILI9341_WHITE);
    }
}

unsigned long showText()
{
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
    tft.println("Hello, World!");
    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
    tft.println(1234.56);
    tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
    tft.println(0xDEADBEEF, HEX);
    tft.println();
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(5);
    tft.println("Inferno");
    tft.setTextSize(2);
    tft.println("Nel mezzo del cammin,");
    tft.setTextSize(1);
    tft.println("di nostra vita");
    tft.println("mi ritrovai per una selva oscura,");
    tft.println("ché la diritta via era smarrita.");
    tft.println("Mentre ch’i’ rovinava in basso loco,");
    tft.println("dinanzi a li occhi mi si fu offerto");
    tft.println("chi per lungo silenzio parea fioco.");
}
