#ifndef __GFX_H__
#define __GFX_H__

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <cstdint>
#include <cstring>
namespace Gfx
{

enum Palette : uint8_t
{
    BROWN,
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    WHITE,
    BLACK
};

const int colors[] = {
    TFT_BROWN,
    TFT_RED,
    TFT_ORANGE,
    TFT_YELLOW,
    TFT_GREEN,
    TFT_BLUE,
    TFT_WHITE,
    TFT_BLACK,
};

class Sprite
{
    int x;
    int y;
    int w;
    int h;
    int z;
    Palette color;
};

class Gfx
{
  public:
    TFT_eSPI tft = TFT_eSPI();
    uint8_t *buffer;
    int width;
    int height;

    Gfx(int width, int height) : width{ width }, height{ height }
    {
        tft.init();
        // TODO: palette size = 4 bits, so 2 pixels per byte
        buffer = new uint8_t[width * height / 2];
    }

    void setPixel(int x, int y, uint8_t color)
    {
        int pixelIdx = y * width + x;
        int byteIdx = pixelIdx / 2;
        if (pixelIdx % 2 == 0) {
            buffer[byteIdx] &= 0x0F;
            buffer[byteIdx] |= color << 4;
        } else {
            buffer[byteIdx] &= 0xF0;
            buffer[byteIdx] |= color;
        }
    }

    void fillRect(int x, int y, int w, int h, Palette colorIdx)
    {
        uint8_t color = static_cast<uint8_t>(colorIdx);
        for (int row = x; row < x + w; row++) {
            for (int col = y; col < y + h; col++) {
                setPixel(row, col, color);
            }
        }
    }

    void push()
    {
        for (int byteIdx = 0; byteIdx < width * height / 2; byteIdx++) {
            int pixelIdx = byteIdx * 2;
            uint8_t data = buffer[byteIdx];
            uint8_t colorIdx1 = (data >> 4) & 0x0F;
            uint8_t colorIdx2 = data & 0x0F;
            int color1 = colors[colorIdx1];
            int color2 = colors[colorIdx2];
            int x1 = pixelIdx / width;
            int y1 = pixelIdx % width;
            int x2 = (pixelIdx + 1) / width;
            int y2 = (pixelIdx + 1) % width;
            tft.drawPixel(x1, y1, color1);
            tft.drawPixel(x2, y2, color2);
        }
    }
};

} // namespace Gfx

#endif // __GFX_H__
