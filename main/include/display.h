#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <esp_err.h>
#include <esp_lcd_types.h>
#include <freertos/idf_additions.h>

esp_err_t displayInit(esp_lcd_panel_handle_t *ret_panel,
					  void *on_trans_done_user_ctx);

#endif // __DISPLAY_H__
