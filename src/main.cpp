#include "aabb.hpp"
#include "component_pool.hpp"
#include "gfx.hpp"
#include <Arduino.h>
#include <Esp.h>
#include <TFT_eSPI.h>

void draw();
void move();

ComponentPool<AABB> aabbs{ 2 };

int width = 480;
int height = 320;
Gfx::Gfx *screen;

void printEspInfo();

void setup(void)
{
    Serial.begin(9600);
    while (!Serial) {
    }
    printEspInfo();

    int size_x = 10;
    int size_y = 10;
    int dx = 2;
    int dy = 2;

    screen = new Gfx::Gfx(width, height);

    AABB player1 = { 90, 10, size_x, size_y, 1, 1, Gfx::RED };
    AABB player2 = { 80, 20, size_x, size_y, 2, 4, Gfx::ORANGE };
    AABB player3 = { 70, 30, size_x, size_y, 3, 3, Gfx::YELLOW };
    AABB player4 = { 60, 40, size_x, size_y, 4, 2, Gfx::GREEN };
    AABB player5 = { 50, 50, size_x, size_y, 1, 1, Gfx::BLUE };
    AABB player6 = { 40, 60, size_x, size_y, 2, 4, Gfx::BROWN };
    AABB player7 = { 30, 70, size_x, size_y, 3, 3, Gfx::RED };
    AABB player8 = { 20, 80, size_x, size_y, 4, 2, Gfx::ORANGE };
    AABB player9 = { 10, 90, size_x, size_y, 1, 1, Gfx::GREEN };
    AABB player10 = { 100, 0, size_x, size_y, dx - 1, dy - 1, Gfx::BLUE };
    AABB player11 = { 100, 0, size_x, size_y, dx * 4, dy * 4, Gfx::GREEN };
    AABB player12 = { 100, 0, size_x, size_y, dx * 2, dy * 2, Gfx::BLACK };
    aabbs.add(player1);
    aabbs.add(player2);
    aabbs.add(player3);
    aabbs.add(player4);
    aabbs.add(player5);
    aabbs.add(player6);
    aabbs.add(player7);
    aabbs.add(player8);
    aabbs.add(player9);
    aabbs.add(player10);
    aabbs.add(player11);
    aabbs.add(player12);
}

int interval = 0;

void loop()
{
    move();
    draw();

    delay(interval);
}

void move()
{
    for (int i = 0; i < aabbs.size; i++) {
        auto &aabb = aabbs.components[i];
        aabb.x += aabb.dx;
        aabb.y += aabb.dy;
        if (aabb.x < 0) {
            aabb.x = 0;
            aabb.dx = -aabb.dx;
        }
        if (aabb.x + aabb.w > width) {
            aabb.x = width - aabb.w;
            aabb.dx = -aabb.dx;
        }
        if (aabb.y + aabb.h > height) {
            aabb.y = height - aabb.h;
            aabb.dy = -aabb.dy;
        }
        if (aabb.y < 0) {
            aabb.y = 0;
            aabb.dy = -aabb.dy;
        }
    }
}

void draw()
{
    screen->fillRect(0, 0, screen->width, screen->height, Gfx::Palette::WHITE);
    // Serial.println("cleared canvas");
    for (int i = 0; i < aabbs.size; i++) {
        auto aabb = aabbs.components[i];
        screen->fillRect(aabb.x, aabb.y, aabb.w, aabb.h, aabb.color);
        // Serial.println("filled aabb " + String(i));
    }
    screen->push();
    // Serial.println("pushed to screen");
}

void printEspInfo()
{
    Serial.println(F("##################################"));
    Serial.println(F("ESP32 Information:"));
    Serial.printf("Internal Total Heap %d, Internal Used Heap %d, Internal Free Heap %d\n", ESP.getHeapSize(), ESP.getHeapSize() - ESP.getFreeHeap(), ESP.getFreeHeap());
    Serial.printf("Sketch Size %d, Free Sketch Space %d\n", ESP.getSketchSize(), ESP.getFreeSketchSpace());
    Serial.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", ESP.getPsramSize(), ESP.getFreePsram());
    Serial.printf("Chip Model %s, ChipRevision %d, Cpu Freq %d, SDK Version %s\n", ESP.getChipModel(), ESP.getChipRevision(), ESP.getCpuFreqMHz(), ESP.getSdkVersion());
    Serial.printf("Flash Size %d, Flash Speed %d\n", ESP.getFlashChipSize(), ESP.getFlashChipSpeed());
    Serial.println(F("##################################\n"));
}
