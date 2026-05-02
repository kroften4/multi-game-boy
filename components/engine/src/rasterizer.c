#include "rasterizer.h"
#include "esp_log.h"
#include "vec.h"
#include <stdint.h>

static const char *TAG = "rasterizer";

// TODO: where does this function really belong
struct vec world_to_screen_coords(struct vec screen_pos,
								  struct vec world_coords)
{
	struct vec res = vec_sub(world_coords, screen_pos);
	res.y = -res.y;
	return res;
}

pixel_t *bitmap_get_coord(struct bitmap *bmp, size_t x, size_t y)
{
	// ESP_LOGI(TAG, "%zu;%zu in %zu;%zu", x, y, bmp->size_x, bmp->size_y);
	assert(x < bmp->size_x && y < bmp->size_y);
	size_t idx = bmp->size_x * y + x;
	assert(idx < bmp->size_x * bmp->size_y);
	return &(bmp->buf[idx]);
}

void rast_fillrect(struct bitmap *bmp, uint32_t x, uint32_t y, uint32_t w,
				   uint32_t h, pixel_t color)
{
	uint32_t x1 = x;
	uint32_t y1 = y;
	uint32_t x2 = x + w;
	uint32_t y2 = y + h;
	if (x1 >= bmp->size_x || y1 >= bmp->size_y) // || x2 < 0 || y2 < 0)
		return;
	if (x2 > bmp->size_x)
		x2 = bmp->size_x;
	// if (x1 < 0)
	// 	x1 = 0;
	if (y2 > bmp->size_y)
		y2 = bmp->size_y;
	// if (y1 < 0)
	// 	y1 = 0;
	// ESP_LOGI(TAG, "Restricted rect: %zu;%zu - %zu;%zu -> %zu;%zu - %zu;%zu", x,
	// 		 y, w, h, x1, y1, x2 - x1, y2 - y1);
	w = x2 - x1;
	h = y2 - y1;

	pixel_t *curr = bitmap_get_coord(bmp, x1, y1);
	pixel_t *end = curr + w;
	for (uint32_t y_curr = y1; y_curr < y2; y_curr++) {
		while (curr != end) {
			*curr = color;
			curr++;
		}
		curr = curr - w + bmp->size_x;
		end = curr + w;
	}
}
