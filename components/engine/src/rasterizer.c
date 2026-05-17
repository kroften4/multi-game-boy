#include "rasterizer.h"
#include "vec.h"
#include <esp_log.h>
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
	ESP_LOGD(TAG, "%zu;%zu in %zu;%zu", x, y, bmp->size_x, bmp->size_y);
	assert(x < bmp->size_x && y < bmp->size_y);
	size_t idx = bmp->size_x * y + x;
	assert(idx < bmp->size_x * bmp->size_y);
	return &(bmp->buf[idx]);
}

void rast_fillrect(struct bitmap *bmp, int x, int y, int w, int h,
				   pixel_t color)
{
	assert(w >= 0 && h >= 0);
	int x1 = x;
	int y1 = y;
	int x2 = x + w;
	int y2 = y + h;
	if (x1 >= bmp->size_x || y1 >= bmp->size_y || x2 < 0 || y2 < 0) {
		ESP_LOGD(TAG, "rect completely out of bmp (%zu;%zu): (%d;%d) - (%d;%d)",
				 bmp->size_x, bmp->size_y, x1, y1, x2, y2);
		return;
	}
	if (x2 > bmp->size_x)
		x2 = bmp->size_x;
	if (x1 < 0)
		x1 = 0;
	if (y2 > bmp->size_y)
		y2 = bmp->size_y;
	if (y1 < 0)
		y1 = 0;
	ESP_LOGD(TAG, "Restricted rect: %d;%d - %d;%d -> %d;%d - %d;%d", x, y, w, h,
			 x1, y1, x2 - x1, y2 - y1);
	w = x2 - x1;
	h = y2 - y1;

	pixel_t *curr = bitmap_get_coord(bmp, x1, y1);
	pixel_t *end = curr + w;
	for (int y_curr = y1; y_curr < y2; y_curr++) {
		while (curr != end) {
			*curr = color;
			curr++;
		}
		curr = curr - w + bmp->size_x;
		end = curr + w;
	}
}
