#include "esp_heap_caps.h"
#include "rasterizer.h"
#include "vec.h"
#include <driver/gpio.h>
#include <esp_check.h>
#include <esp_err.h>
#include <esp_lcd_io_i80.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_types.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/idf_additions.h>
#include <freertos/projdefs.h>
#include <hal/gpio_types.h>
#include <soc/clk_tree_defs.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "config.h"
#include "display.h"
#include "display_test.h"
#include "rasterizer.h"
#include "utils.h"

static const char *TAG = "main";

struct game_obj {
	struct vec pos;
	struct vec size;
	pixel_t color;
};
pixel_t background_color = 0xFFFF;

struct game_obj game_objs[] = { { { 0, 120 }, { 10, 50 }, 0xF800 },
								{ { 300, 120 }, { 10, 50 }, 0x07e0 },
								{ { 120, 120 }, { 10, 10 }, 0x001F } };

#define LINES_IN_STRIPE DISPLAY_V_RES / 16
struct bitmap stripe_bmp = {};

void drawState(struct bitmap *bmp, struct vec screen_pos)
{
	rast_fillrect(bmp, 0, 0, bmp->size_x, bmp->size_y, background_color);
	for (size_t i = 0; i < ARR_SIZE(game_objs); i++) {
		struct game_obj *obj = &game_objs[i];
		struct vec obj_screen_pos =
			world_to_screen_coords(screen_pos, obj->pos);
		ESP_LOGI(TAG, "obj %d;%d -> %d;%d (screen %d;%d)", obj->pos.x,
				 obj->pos.y, obj_screen_pos.x, obj_screen_pos.y, screen_pos.x,
				 screen_pos.y);
		rast_fillrect(bmp, obj_screen_pos.x, obj_screen_pos.y, obj->size.x,
					  obj->size.y, obj->color);
	}
}

void drawRandomStripe(struct bitmap *bmp)
{
	rast_fillrect(bmp, 0, 0, bmp->size_x, bmp->size_y, rand() % 0xFFFF);
	// rast_fillrect(bmp, 0, 0, bmp->size_x, bmp->size_y, 0x07e0);
}

void pushFrame(esp_lcd_panel_handle_t *panel)
{
	const struct vec screen_pos = { 0, DISPLAY_V_RES };
	struct vec stripe_pos = screen_pos;
	uint16_t y_bottom = stripe_pos.y - DISPLAY_V_RES;
	pixel_t color = rand() % 0xFFFF;
	while (stripe_pos.y > y_bottom) {
		// drawRandomStripe(&stripe_bmp);
		rast_fillrect(&stripe_bmp, 0, 0, stripe_bmp.size_x, stripe_bmp.size_y,
					  color);
		// drawState(&stripe_bmp, stripe_pos);
		struct vec stripe_screen_pos =
			world_to_screen_coords(screen_pos, stripe_pos);
		// ESP_LOGI(TAG, "stripe %d;%d -> %d;%d", stripe_pos.x, stripe_pos.y,
		// 		 stripe_screen_pos.x, stripe_screen_pos.y);
		// ESP_LOGI(TAG, "Pushing stripe %d;%d - %d;%d", 0, stripe_screen_pos.y,
		// 		 DISPLAY_H_RES, stripe_screen_pos.y + LINES_IN_STRIPE);
		ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(
			*panel, 0, stripe_screen_pos.y, DISPLAY_H_RES,
			stripe_screen_pos.y + LINES_IN_STRIPE, stripe_bmp.buf));
		stripe_pos.y -= LINES_IN_STRIPE;
		// TODO: wait for DMA with double buffering
		// vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

static int interval_ms = 0;
void app_main(void)
{
	srand(0);
	stripe_bmp.size_x = DISPLAY_H_RES;
	stripe_bmp.size_y = LINES_IN_STRIPE;
	stripe_bmp.buf = heap_caps_malloc(stripe_bmp.size_x * stripe_bmp.size_y *
										  sizeof(stripe_bmp.buf[0]),
									  MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
	ESP_RETURN_VOID_ON_FALSE(stripe_bmp.buf != NULL, TAG,
							 "Failed to allocate stripe buffer");

	esp_lcd_panel_handle_t panel = NULL;
	ESP_ERROR_CHECK(displayInit(&panel));
	esp_lcd_panel_swap_xy(panel, true);
	ESP_LOGI(TAG, "Initialized display");

	while (1) {
		// displayTestSquares(&panel);
		pushFrame(&panel);
		// ESP_LOGI(TAG, "Pushed frame");
		vTaskDelay(pdMS_TO_TICKS(interval_ms));
	}

	// TaskHandle_t blinkHandle = NULL;
	// xTaskCreate(displayBlink, "displayBlink", 65536, NULL, 0, &blinkHandle);
}
