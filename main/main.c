#include "config.h"
#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <stdint.h>
#include <stdio.h>

static void taskBlink(void *)
{
	esp_rom_gpio_pad_select_gpio(GPIO_NUM_2);
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
	uint8_t led_state = 0;

	while (1) {
		printf("Task %s on core %d\n",
			   pcTaskGetName(xTaskGetCurrentTaskHandle()), xPortGetCoreID());
		led_state = !led_state;
		gpio_set_level(GPIO_NUM_2, led_state);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void app_main(void)
{
	TaskHandle_t blinkHandle = NULL;
	xTaskCreate(taskBlink, "taskBlink", 2048, NULL, 0, &blinkHandle);
}
