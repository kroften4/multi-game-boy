#include <freertos/FreeRTOS.h>
#include <esp_heap_caps.h>
#include <esp_lcd_types.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_io_i80.h>
#include <esp_log.h>

#include "utils.h"
#include "display_test.h"

static const char *TAG = "display_test";

esp_err_t displayTestSquares(esp_lcd_panel_handle_t *panel)
{
	uint16_t rect_w = 20;
	uint16_t rect_h = 20;
	size_t rect_buf_size = rect_w * rect_h * 2;
	uint16_t *color_data =
		heap_caps_malloc(rect_buf_size, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
	if (color_data == NULL) {
		return ESP_ERR_NO_MEM;
	}
	uint16_t colors[] = { 0xf800, 0x07e0, 0x001f, 0x0000, 0xffff };
	size_t x_start = 100;
	size_t y_start = 0;
	size_t x_end = x_start + rect_w;
	size_t y_end = y_start + rect_h;
	while (1) {
		for (size_t color_idx = 0; color_idx < ARR_SIZE(colors); color_idx++) {
			ESP_LOGI(TAG, "color_idx %zu: %#06x", color_idx, colors[color_idx]);
			for (size_t i = 0; i < rect_w * rect_h; i++) {
				color_data[i] = colors[color_idx];
			}
			x_start = color_idx * rect_w;
			y_start = color_idx * rect_h;
			x_end = x_start + rect_w;
			y_end = y_start + rect_h;
			esp_lcd_panel_draw_bitmap(*panel, x_start, y_start, x_end, y_end,
									  color_data);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}
}
