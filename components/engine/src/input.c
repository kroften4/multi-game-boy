#include "sdkconfig.h"
#include "soc/gpio_num.h"
#include <button_gpio.h>
#include <esp_check.h>
#include <esp_err.h>
#include <esp_log.h>
#include <iot_button.h>

static const char *TAG = "input";

enum game_buttons {
	GAME_BTN_UP,
	GAME_BTN_DOWN,
	GAME_BTN_LEFT,
	GAME_BTN_RIGHT,
	__GAME_BTN_LAST,
};
static bool keyboard[__GAME_BTN_LAST];
const bool *engine_get_keyboard_state()
{
	return keyboard;
}

static void up_btn_down_cb(void *arg, void *usr_data)
{
	keyboard[GAME_BTN_UP] = true;
}
static void up_btn_up_cb(void *arg, void *usr_data)
{
	keyboard[GAME_BTN_UP] = false;
}

static void down_btn_down_cb(void *arg, void *usr_data)
{
	keyboard[GAME_BTN_DOWN] = true;
}
static void down_btn_up_cb(void *arg, void *usr_data)
{
	keyboard[GAME_BTN_DOWN] = false;
}

static void left_btn_down_cb(void *arg, void *usr_data)
{
	keyboard[GAME_BTN_LEFT] = true;
}
static void left_btn_up_cb(void *arg, void *usr_data)
{
	keyboard[GAME_BTN_LEFT] = false;
}

static void right_btn_down_cb(void *arg, void *usr_data)
{
	keyboard[GAME_BTN_RIGHT] = true;
}
static void right_btn_up_cb(void *arg, void *usr_data)
{
	keyboard[GAME_BTN_RIGHT] = false;
}

struct game_button {
	gpio_num_t gpio;
	button_cb_t up_cb;
	button_cb_t down_cb;
};

esp_err_t game_button_create(struct game_button btn)
{
	const button_config_t btn_cfg = { 0 };
	const button_gpio_config_t btn_gpio_cfg = {
		.gpio_num = btn.gpio,
		.active_level = 1,
	};
	button_handle_t up_btn = NULL;
	ESP_RETURN_ON_ERROR(iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg,
												   &up_btn),
						TAG, "Failed to create button gpio device");

	ESP_RETURN_ON_ERROR(iot_button_register_cb(up_btn, BUTTON_PRESS_DOWN, NULL,
											   btn.down_cb, NULL),
						TAG, "Failed to register BUTTON_PRESS_DOWN callback");
	ESP_RETURN_ON_ERROR(iot_button_register_cb(up_btn, BUTTON_PRESS_UP, NULL,
											   btn.up_cb, NULL),
						TAG, "Failed to register BUTTON_PRESS_UP callback");
	return ESP_OK;
}

esp_err_t input_init()
{
	esp_err_t ret = game_button_create((struct game_button){
		CONFIG_ENGINE_BTN_UP_GPIO, up_btn_down_cb, up_btn_up_cb });
	ESP_RETURN_ON_ERROR(ret, TAG, "Failed to create UP button");

	ret = game_button_create((struct game_button){
		CONFIG_ENGINE_BTN_DOWN_GPIO, down_btn_down_cb, down_btn_up_cb });
	ESP_RETURN_ON_ERROR(ret, TAG, "Failed to create DOWN button");

	ret = game_button_create((struct game_button){
		CONFIG_ENGINE_BTN_LEFT_GPIO, left_btn_down_cb, left_btn_up_cb });
	ESP_RETURN_ON_ERROR(ret, TAG, "Failed to create LEFT button");

	ret = game_button_create((struct game_button){
		CONFIG_ENGINE_BTN_RIGHT_GPIO, right_btn_down_cb, right_btn_up_cb });
	ESP_RETURN_ON_ERROR(ret, TAG, "Failed to create RIGHT button");

	return ESP_OK;
}
