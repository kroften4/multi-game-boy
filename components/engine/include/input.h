#ifndef __ENGINE_INPUT_H__
#define __ENGINE_INPUT_H__

#include <esp_err.h>
#include <soc/gpio_num.h>

enum game_buttons {
	GAME_BTN_UP,
	GAME_BTN_DOWN,
	GAME_BTN_LEFT,
	GAME_BTN_RIGHT,
	__GAME_BTN_LAST,
};
const bool *engine_get_keyboard_state();

struct game_button;

esp_err_t game_button_create(struct game_button btn);

esp_err_t input_init();

#endif // __ENGINE_INPUT_H__
