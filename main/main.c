#include "esp_heap_caps.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "hal/gpio_types.h"
#include <esp_err.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_types.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <soc/clk_tree_defs.h>
#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <stdint.h>
#include <esp_lcd_io_i80.h>

#include "config.h"
#include "esp_ili9486_panel.h"
#include "utils.h"

#define TAG "main"

static esp_err_t displayInit(esp_lcd_panel_handle_t *ret_panel)
{
	// TODO: wire backlight to gpio and turn on only after init
	ESP_LOGI(TAG, "Starting display");
	gpio_reset_pin(DISPLAY_PIN_RD);
	gpio_set_direction(DISPLAY_PIN_RD, GPIO_MODE_OUTPUT);
	gpio_set_level(DISPLAY_PIN_RD, 1);
	vTaskDelay(pdMS_TO_TICKS(10));
	ESP_LOGI(TAG, "Set RD ping inactive");

	esp_lcd_i80_bus_handle_t i80_bus = NULL;
	esp_lcd_i80_bus_config_t bus_config = {
		.clk_src = LCD_CLK_SRC_DEFAULT,
		.dc_gpio_num = DISPLAY_PIN_DC,
		.wr_gpio_num = DISPLAY_PIN_WR,
		.data_gpio_nums = {
			DISPLAY_PIN_D0,
			DISPLAY_PIN_D1,
			DISPLAY_PIN_D2,
			DISPLAY_PIN_D3,
			DISPLAY_PIN_D4,
			DISPLAY_PIN_D5,
			DISPLAY_PIN_D6,
			DISPLAY_PIN_D7,
		},
		.bus_width = 8,
		// transaction limit of 50 lines of 2-byte pixels
		.max_transfer_bytes = DISPLAY_H_RES * 50 * sizeof(uint16_t),
		.dma_burst_size = DISPLAY_DMA_BURST_SIZE,
	};
	ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));
	ESP_LOGI(TAG, "Created i80 bus");

	esp_lcd_panel_io_handle_t io_handle = NULL;
	esp_lcd_panel_io_i80_config_t io_config = {
		.cs_gpio_num = DISPLAY_PIN_CS,
		.pclk_hz = DISPLAY_PIXEL_CLOCK_HZ,
		.trans_queue_depth = 10,
		.dc_levels = {
			.dc_cmd_level = 0,
			.dc_dummy_level = 0,
			.dc_idle_level = 0,
			.dc_data_level = 1,
		},
		.lcd_cmd_bits = DISPLAY_CMD_BITS,
		.lcd_param_bits = DISPLAY_PARAM_BITS,
		.flags = {
			.cs_active_high = 0,
			.pclk_active_neg = 0,
			.pclk_idle_low = 0,
			// .reverse_color_bits = 0,
			// .swap_color_bytes = 1,
		}
	};
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));
	ESP_LOGI(TAG, "Created panel io");

	esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = DISPLAY_PIN_RST,
		.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
		.bits_per_pixel = 16,
	};

	ESP_ERROR_CHECK(
		esp_lcd_new_panel_ili9486(io_handle, &panel_config, ret_panel));
	ESP_LOGI(TAG, "Created panel");
	ESP_ERROR_CHECK(esp_lcd_panel_reset(*ret_panel));
	ESP_LOGI(TAG, "Reset successful");
	ESP_ERROR_CHECK(esp_lcd_panel_init(*ret_panel));
	ESP_LOGI(TAG, "Init successful");
	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(*ret_panel, true));
	ESP_LOGI(TAG, "Power on successful");

	return ESP_OK;
}

static esp_err_t displayBlink(void *)
{
	ESP_LOGI(TAG, "Task %s on core %d",
			 pcTaskGetName(xTaskGetCurrentTaskHandle()), xPortGetCoreID());
	esp_lcd_panel_handle_t panel = NULL;
	displayInit(&panel);
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
		ESP_LOGI(TAG, "Task %s on core %d",
				 pcTaskGetName(xTaskGetCurrentTaskHandle()), xPortGetCoreID());
		for (size_t color_idx = 0; color_idx < ARR_SIZE(colors); color_idx++) {
			for (uint16_t i = 0; i < rect_w * rect_h; i++) {
				color_data[i] = colors[color_idx];
			}
			x_start = color_idx * rect_w;
			y_start = color_idx * rect_h;
			x_end = x_start + rect_w;
			y_end = y_start + rect_h;
			esp_lcd_panel_draw_bitmap(panel, x_start, y_start, x_end, y_end,
									  color_data);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}
}

void app_main(void)
{
	ESP_ERROR_CHECK(displayBlink(NULL));
	// TaskHandle_t blinkHandle = NULL;
	// xTaskCreate(displayBlink, "displayBlink", 65536, NULL, 0, &blinkHandle);

	// while (1) {
	// 	ESP_LOGI(TAG, "Task %s on core %d",
	// 			 pcTaskGetName(xTaskGetCurrentTaskHandle()), xPortGetCoreID());
	// 	vTaskDelay(pdMS_TO_TICKS(5000));
	// }
}
