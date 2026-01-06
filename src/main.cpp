#include "aabb.hpp"
#include "component_pool.hpp"
#include "utils.hpp"
#include <Arduino.h>
#include <Esp.h>
#include <TFT_eSPI.h>
#include <cstdlib>

#define AABBS_CAP 4096
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
    AABB_randomSpawn(aabbs, AABBS_CAP, width, height);

    tft.init();
    tft.setRotation(1);
    buf.setColorDepth(4);
    buf.createSprite(width, height);
}

static int interval = 200;

void loop()
{
    AABB_bounce(aabbs, width, height);
    AABB_render(buf, aabbs);

    // delay(interval);
}
