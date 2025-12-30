#ifndef __LOWRES_GFX_H__
#define __LOWRES_GFX_H__

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <cstdint>
#include <cstring>

namespace LowresGfx
{

class Sprite
{
    int x;
    int y;
    int w;
    int h;
    int z;
    int color;
};

class Gfx
{
  public:
    TFT_eSPI tft = TFT_eSPI();
    uint8_t *buffer;
    int width;
    int height;

    Gfx(int width, int height)
    {
        tft.init();
        // resolution scaled down by 2
        this->width = width / 2;
        this->height = height / 2;
        buffer = new uint8_t[this->width * this->height];
    }

    void setPixel(int x, int y, uint8_t color)
    {
        int pixelIdx = y * width + x;
        buffer[pixelIdx] = color;
    }

    void fillRect(int x, int y, int w, int h, uint8_t color)
    {
        for (int xi = x; xi < x + w; xi++) {
            for (int yi = y; yi < y + h; yi++) {
                setPixel(xi, yi, color);
            }
        }
    }

    void pushLine(int height)
    {
        int y = height / 2;
        for (int x = 0; x < width; x++) {
            int color = tft.color8to16(buffer[y * width + x]);
            tft.drawPixel(x * 2, height, color);
            tft.drawPixel(x * 2 + 1, height, color);
        }
    }

    void push()
    {
        for (int y = 0; y < height; y++) {
            pushLine(y * 2);
            pushLine(y * 2 + 1);
        }
    }
};

} // namespace LowresGfx

#endif // __LOWRES_GFX_H__
