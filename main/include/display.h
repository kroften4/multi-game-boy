#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <freertos/idf_additions.h>
#include <esp_lcd_types.h>
#include <esp_err.h>

esp_err_t displayInit(esp_lcd_panel_handle_t *ret_panel, TaskHandle_t draw_task);

#endif // __DISPLAY_H__
