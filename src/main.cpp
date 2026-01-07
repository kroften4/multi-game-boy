#include "aabb.hpp"
#include "button.hpp"
#include "component_pool.hpp"
#include "utils.hpp"
#include <Arduino.h>
#include <Esp.h>
#include <TFT_eSPI.h>
#include <cstdlib>
#include <esp32-hal-gpio.h>
#include <esp32-hal.h>
#include <sys/types.h>

#define BTN_LEFT  19
#define BTN_RIGHT 23
#define BTN_UP    21
#define BTN_DOWN  22

static uint debounceTime = 25;
static ButtonState leftBtn(BTN_LEFT, debounceTime);
static ButtonState rightBtn(BTN_RIGHT, debounceTime);
static ButtonState upBtn(BTN_UP, debounceTime);
static ButtonState downBtn(BTN_DOWN, debounceTime);

#define AABBS_CAP 4608
static ComponentPool<AABB> aabbs{ AABBS_CAP };

static int width = 480;
static int height = 320;

static TFT_eSPI tft = TFT_eSPI();
static TFT_eSprite buf = TFT_eSprite(&tft);

static int playerSpeed = 7;
static int playerId;

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
    }
    printEspInfo();

    srand(0);

    AABB player = {
        .color = default_4bit_palette[6],
        .x = 50,
        .y = 50,
        .w = 20,
        .h = 20,
    };
    playerId = aabbs.add(player);

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
    leftBtn.pollButton();
    rightBtn.pollButton();
    upBtn.pollButton();
    downBtn.pollButton();

    AABB *player = &(aabbs.components[playerId]);
    player->dx = 0;
    player->dy = 0;
    if (leftBtn.down) {
        player->dx -= playerSpeed;
    }
    if (rightBtn.down) {
        player->dx += playerSpeed;
    }
    if (downBtn.down) {
        player->dy += playerSpeed;
    }
    if (upBtn.down) {
        player->dy -= playerSpeed;
    }
    player->x += player->dx;
    player->y += player->dy;

    fpsCounter++;
    ulong currMillis = millis();
    if (currMillis - fpsStart >= 1000) {
        currFps = fpsCounter;
        fpsCounter = 0;
        fpsStart = currMillis;
    }

    buf.fillSprite(TFT_WHITE);
    AABB_draw(buf, aabbs);
    buf.setCursor(0, 0);
    buf.setTextSize(2);
    buf.print("FPS: " + String(currFps));
    buf.pushSprite(0, 0);

    // delay(interval);
}
