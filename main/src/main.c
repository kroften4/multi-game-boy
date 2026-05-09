#include <esp_log.h>

#include "cpong/client_main.h"

static const char *TAG = "main";

void app_main(void)
{
	int ret = client_main("localhost", "8889");
	if (ret != 0) {
		ESP_LOGE(TAG, "client_main exited with code %d", ret);
		return;
	}
	ESP_LOGI(TAG, "client_main exited with code %d", ret);
}
