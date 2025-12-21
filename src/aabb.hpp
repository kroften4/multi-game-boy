#ifndef __AABB_H__
#define __AABB_H__

#include "gfx.hpp"
struct AABB
{
    int x;
    int y;
    int w;
    int h;
    int dx;
    int dy;
    Gfx::Palette color;
};

#endif // __AABB_H__
