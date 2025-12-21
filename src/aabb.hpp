#ifndef __AABB_H__
#define __AABB_H__

#include <TFT_eSPI.h>

struct AABB
{
    int x;
    int y;
    int w;
    int h;
    int dx;
    int dy;
    int color;
};

#endif // __AABB_H__
