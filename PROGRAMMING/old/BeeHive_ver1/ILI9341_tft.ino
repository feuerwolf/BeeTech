

#define TFT_horizontal   1
#define TFT_vertical     0
#define TFT_horizontal2  3
#define TFT_vertical2    4

void init_tft(){


    tft.begin();

    tft.setRotation(TFT_horizontal2);

    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(3);
      
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
