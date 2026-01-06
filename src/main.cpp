#include "aabb.hpp"
#include "component_pool.hpp"
#include "esp32-hal.h"
#include "utils.hpp"
#include <Arduino.h>
#include <Esp.h>
#include <TFT_eSPI.h>
#include <cstdlib>
#include <sys/types.h>

#define AABBS_CAP 4608
static ComponentPool<AABB> aabbs{ AABBS_CAP };

static int width = 480;
static int height = 320;

static TFT_eSPI tft = TFT_eSPI();
static TFT_eSprite buf = TFT_eSprite(&tft);

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
    }
    printEspInfo();

    srand(0);
    AABB_randomSpawn(aabbs, AABBS_CAP, width, height, 5);

    tft.init();
    tft.setRotation(3);
    buf.setColorDepth(4);
    buf.createSprite(width, height);
}

static int interval = 200;
static int fpsCounter = 0;
static int currFps = 0;
static ulong fpsStart = 0;

void loop()
{
    fpsCounter++;
    ulong currMillis = millis();
    if (currMillis - fpsStart >= 1000) {
        currFps = fpsCounter;
        fpsCounter = 0;
        fpsStart = currMillis;
    }

    buf.fillSprite(TFT_WHITE);
    AABB_bounce(aabbs, width, height);
    AABB_draw(buf, aabbs);
    buf.setCursor(0, 0);
    buf.print("FPS: " + String(currFps));
    buf.setTextSize(2);
    buf.pushSprite(0, 0);

    // delay(interval);
}
