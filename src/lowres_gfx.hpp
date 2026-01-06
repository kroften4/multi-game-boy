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
    uint16_t *partBuf;
    int partBufLen;
    uint8_t *lowResBuf;
    int bufParts;
    int bufWidth;
    int bufHeight;
    int width;
    int height;
    int partHeight;

    Gfx(int width, int height, int bufParts = 16)
    {
        tft.init();
        tft.setAddrWindow(0, 0, width, height);
        tft.setRotation(1);

        this->width = width;
        this->height = height;

        this->bufParts = bufParts;
        partHeight = height / bufParts;
        partBufLen = width * partHeight;
        partBuf = new uint16_t[partBufLen];

        // resolution scaled down by 2
        bufWidth = width / 2;
        bufHeight = height / 2;
        lowResBuf = new uint8_t[bufWidth * bufHeight];
    }

    void setPixel(int x, int y, uint8_t color)
    {
        int pixelIdx = y * bufWidth + x;
        lowResBuf[pixelIdx] = color;
    }

    void fillRect(int x, int y, int w, int h, uint8_t color)
    {
        for (int xi = x; xi < x + w; xi++) {
            for (int yi = y; yi < y + h; yi++) {
                setPixel(xi, yi, color);
            }
        }
    }

    // void pushLine(int line_y)
    // {
    //     int y = line_y / 2;
    //     for (int x = 0; x < width; x++) {
    //         int color = tft.color8to16(lowResBuf[y * width + x]);
    //         tft.drawPixel(x * 2, line_y, color);
    //         tft.drawPixel(x * 2 + 1, line_y, color);
    //     }
    // }

    void writePartBuf(int y_start)
    {
        for (int y = 0; y < partHeight; y += 2) {
            for (int x = 0; x < width; x += 2) {
                uint16_t color = tft.color8to16(lowResBuf[(y_start + y) / 2 * bufWidth + x / 2]);
                partBuf[y * width + x] = color;
                partBuf[y * width + x + 1] = color;
            }
            // Serial.println("spread out line " + String(y));
            memcpy(partBuf + ((y + 1) * width), partBuf + (y * width), width);
            // Serial.println("copied to line " + String(y + 1));
        }
    }

    void push()
    {
        for (int y_sep = 0; y_sep < height; y_sep += partHeight) {
            // tft.setAddrWindow(0, y_sep, width, partHeight);
            // Serial.println("set addr window: " + String(0) + " " +
            //                String(y_sep) + " " + String(width) + " " +
            //                String(partHeight));
            writePartBuf(y_sep);
            // Serial.println("wrote to partBuf: " + String(y_sep) + " " +
            //                String(y_sep + partHeight));
            // tft.pushPixels(partBuf, partBufLen);
            tft.pushImage(0, y_sep, width, partHeight, partBuf);
        }
    }
};

} // namespace LowresGfx

#endif // __LOWRES_GFX_H__
