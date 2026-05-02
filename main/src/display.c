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

#include "config.h"
#include "display.h"
#include "esp_ili9486_panel.h"

static const char *TAG = "display";

bool onTransDone(esp_lcd_panel_io_handle_t panel_io,
				 esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
	TaskHandle_t draw_task = user_ctx;
	xTaskNotifyGive(draw_task);
	return true;
}

esp_err_t displayInit(esp_lcd_panel_handle_t *ret_panel, TaskHandle_t draw_task)
{
	// TODO: wire backlight to gpio and turn on only after init
	gpio_reset_pin(DISPLAY_PIN_RD);
	gpio_set_direction(DISPLAY_PIN_RD, GPIO_MODE_OUTPUT);
	gpio_set_level(DISPLAY_PIN_RD, 1);
	vTaskDelay(pdMS_TO_TICKS(10));

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
	ESP_RETURN_ON_ERROR(esp_lcd_new_i80_bus(&bus_config, &i80_bus), TAG,
						"Failed to initialize i80 bus");

	esp_lcd_panel_io_handle_t io_handle = NULL;
	esp_lcd_panel_io_i80_config_t io_config = {
		.on_color_trans_done = onTransDone,
		.user_ctx = draw_task,
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
			.reverse_color_bits = 0,
			.swap_color_bytes = 1,
		}
	};
	ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_i80(i80_bus, &io_config,
												 &io_handle),
						TAG, "Failed to initialize panel io");

	esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = DISPLAY_PIN_RST,
		// .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
		.bits_per_pixel = 16,
	};

	ESP_RETURN_ON_ERROR(esp_lcd_new_panel_ili9486(io_handle, &panel_config,
												  ret_panel),
						TAG, "Failed to initialize panel");
	ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(*ret_panel), TAG,
						"Failed to reset panel");
	ESP_RETURN_ON_ERROR(esp_lcd_panel_init(*ret_panel), TAG,
						"Panel init sequence failed");
	ESP_RETURN_ON_ERROR(esp_lcd_panel_disp_on_off(*ret_panel, true), TAG,
						"Failed to turn the display on");

	return ESP_OK;
}
