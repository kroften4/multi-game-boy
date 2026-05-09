#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include <stddef.h>
#include <stdint.h>

#define RGB565(r, g, b) (uint16_t)(((r>>3) << 11) | ((g>>2) << 5) | b >> 3)

typedef uint16_t pixel_t;

struct bitmap {
	pixel_t *buf;
	int size_x;
	int size_y;
};

pixel_t *bitmap_get_coord(struct bitmap *bmp, size_t x, size_t y);

struct vec world_to_screen_coords(struct vec screen_pos,
								  struct vec world_coords);

/**
 * Draw a rect of `color` into `bmp`. `x`, `y` are in screen coords.
 */
void rast_fillrect(struct bitmap *bmp, int x, int y, int w,
				   int h, pixel_t color);

#endif // __RASTERIZER_H__
