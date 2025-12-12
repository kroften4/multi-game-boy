#include "sys/unistd.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr_dvdlogo = TFT_eSprite(&tft);

void print_manifesto();

void setup(void)
{
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(0);

    // tft.setTextFont(1);
    // tft.setTextSize(2);
    // tft.setTextColor(TFT_WHITE, TFT_BLACK);
    // tft.setTextDatum(CC_DATUM);
    // tft.setTextWrap(true);
}

void loop()
{
    // tft.fillRect(0, 0, tft.width(), tft.height(), TFT_RED);
    // sleep(5);
    tft.fillRect(0, 0, tft.width(), tft.height(), TFT_GREEN);
    sleep(5);
    // tft.fillRect(0, 0, tft.width(), tft.height(), TFT_BLUE);
    // sleep(5);
}

