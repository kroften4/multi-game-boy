// https://developer.espressif.com/blog/getting-started-with-wifi-on-esp-idf/
#ifndef __SIMPLE_WIFI_H__
#define __SIMPLE_WIFI_H__

#include "esp_err.h"

esp_err_t simple_wifi_init(void);

esp_err_t simple_wifi_connect(char *wifi_ssid, char *wifi_password);

esp_err_t simple_wifi_disconnect(void);

esp_err_t simple_wifi_deinit(void);

#endif // __SIMPLE_WIFI_H__
