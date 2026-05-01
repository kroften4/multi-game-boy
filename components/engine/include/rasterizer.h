#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include <stddef.h>
#include <stdint.h>

typedef uint16_t pixel_t;

struct bitmap {
	pixel_t *buf;
	size_t size_x;
	size_t size_y;
};

pixel_t *bitmap_get_coord(struct bitmap *bmp, size_t x, size_t y);

struct vec world_to_screen_coords(struct vec screen_pos,
								  struct vec world_coords);

/**
 * Draw a rect of `color` into `bmp`. `x`, `y` are in screen coords.
 */
void rast_fillrect(struct bitmap *bmp, uint32_t x, uint32_t y, uint32_t w,
				   uint32_t h, pixel_t color);

#endif // __RASTERIZER_H__
