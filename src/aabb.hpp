#ifndef __AABB_H__
#define __AABB_H__

#include "TFT_eSPI.h"
#include "component_pool.hpp"
#include <cstdint>

struct AABB
{
    int color;
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
    int16_t dx;
    int16_t dy;
};

void AABB_draw(TFT_eSprite &buf, ComponentPool<AABB> &aabbs)
{
    for (int i = 0; i < aabbs.size; i++) {
        auto aabb = aabbs.components[i];
        buf.fillRect(aabb.x, aabb.y, aabb.w, aabb.h, buf.color16to8(aabb.color));
    }
}

void AABB_bounce(ComponentPool<AABB> &aabbs, int width, int height)
{
    for (uint16_t i = 0; i < aabbs.size; i++) {
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

void AABB_randomSpawn(ComponentPool<AABB> &aabbs, int amount,
                      int areaWidth, int areaHeight, int size)
{
    // int colors[] = { TFT_BLACK, TFT_NAVY, TFT_DARKGREEN, TFT_DARKCYAN,
    //                  TFT_MAROON, TFT_PURPLE, TFT_OLIVE, TFT_LIGHTGREY,
    //                  TFT_DARKGREY, TFT_BLUE, TFT_GREEN, TFT_CYAN, TFT_RED,
    //                  TFT_MAGENTA, TFT_YELLOW, TFT_WHITE, TFT_ORANGE,
    //                  TFT_GREENYELLOW, TFT_PINK, TFT_BROWN, TFT_GOLD, TFT_SILVER,
    //                  TFT_SKYBLUE, TFT_VIOLET };
    for (int i = 0; i < amount; i++) {
        int32_t posX = rand() % areaWidth - 30;
        int32_t posY = rand() % areaHeight - 30;
        int16_t velX = rand() % 8 + 1;
        int16_t velY = rand() % 8 + 1;
        AABB player = { default_4bit_palette[rand() % 16],
                        posX, posY,
                        size, size,
                        velX, velY };
        //                 colors[rand() % 24] };
        aabbs.add(player);
    }
}

#endif // __AABB_H__
