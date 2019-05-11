#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "coffee.h"

#define LOG_ERROR ESP_LOGE
#define LOG_INFO ESP_LOGI

static const char *TAG = "coffee_making";

typedef int (*press_button_func)(void *priv);

int press_up_button(void *priv)
{
	LOG_INFO(TAG, "%s called", __func__);
	return 0;
}

int press_down_button(void *priv)
{
	LOG_INFO(TAG, "%s called", __func__);
	return 0;
}

int press_put_water_button(void *priv)
{
	LOG_INFO(TAG, "%s called", __func__);
	return 0;
}

static const press_button_func press_buttons[MAX_PRESS_BUTTONS] = {
	[PRESS_UP_BUTTON] = press_up_button,
	[PRESS_DOWN_BUTTON] = press_down_button,
	[PRESS_PUT_WATER_BUTTON] = press_put_water_button
};

static const char *button_names[MAX_PRESS_BUTTONS] = {
	[PRESS_UP_BUTTON] = "up button",
	[PRESS_DOWN_BUTTON] = "down button",
	[PRESS_PUT_WATER_BUTTON] = "put water"
};

int coffee_press_button(uint8_t button)
{
	int ret = -1;

	if (button >= MAX_PRESS_BUTTONS) {
		ret = -1;
		LOG_ERROR(TAG, "invalid button press requested (%d)", button);
		goto err;
	}

	LOG_INFO(TAG, "requested a press of button %d, (%s)",
		button, button_names[button]);
	ret = press_buttons[button](NULL);

err:
	return ret;
}

int coffee_make_coffee(struct coffee_options choice)
{
	return 0;
}
