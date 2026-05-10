#include <esp_check.h>
#include <esp_err.h>
#include <esp_log.h>

#include "cpong/client_main.h"
#include "sdkconfig.h"
#ifndef CONFIG_IDF_TARGET_LINUX
#include "simple_wifi.h"
#endif

static const char *TAG = "main";

void app_main(void)
{
#ifndef CONFIG_IDF_TARGET_LINUX
	ESP_RETURN_VOID_ON_ERROR(simple_wifi_init(), TAG, "Failed to init wifi");
	ESP_RETURN_VOID_ON_ERROR(simple_wifi_connect(CONFIG_WIFI_SSID,
												 CONFIG_WIFI_PASSWORD),
							 TAG, "Failed to connect to wifi");
#endif

	int ret = client_main("192.168.3.39", "8889");
	if (ret != 0) {
		ESP_LOGE(TAG, "client_main exited with code %d", ret);
		return;
	}
	ESP_LOGI(TAG, "client_main exited with code %d", ret);
}
