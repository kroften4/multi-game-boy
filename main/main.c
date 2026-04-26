#include "config.h"
#include "esp_err.h"
#include "soc/clk_tree_defs.h"
#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <stdint.h>
#include <stdio.h>
#include <esp_lcd_io_i80.h>

static void taskBlink(void *)
{
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
		// transaction limit of 100 lines of 2-byte pixels
		.max_transfer_bytes = DISPLAY_H_RES * 100 * sizeof(uint16_t),
		.dma_burst_size = DISPLAY_DMA_BURST_SIZE,
	};
	ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));

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
	};
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));

	while (1) {
		printf("Task %s on core %d\n",
			   pcTaskGetName(xTaskGetCurrentTaskHandle()), xPortGetCoreID());
	}
}

void app_main(void)
{
	TaskHandle_t blinkHandle = NULL;
	xTaskCreate(taskBlink, "taskBlink", 2048, NULL, 0, &blinkHandle);
}
