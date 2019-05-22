#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "coffee.h"

#define LOG_ERROR ESP_LOGE
#define LOG_INFO ESP_LOGI
#define LOG_DEBUG ESP_LOGD
#define LOW (0)
#define HIGH (1)
#define ONE_SECOND_IN_MS (1000)
#define SLEEP_BETWEEN_PRESSES (2)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))
#endif

static const char *TAG = "coffee_making";
enum coffee_temprature coffee_current_temprature = COFFEE_HOT;

typedef int (*press_button_func)(void *priv);

static int press_up_button(void *priv)
{
	LOG_INFO(TAG, "%s called", __func__);
	/* press B & D gpio */
	gpio_set_level(COFFEE_B_PORT, HIGH);
	gpio_set_level(COFFEE_D_PORT, HIGH);
	return 0;
}

static int press_down_button(void *priv)
{
	LOG_INFO(TAG, "%s called", __func__);
	/* press B & F gpio */
	gpio_set_level(COFFEE_B_PORT, HIGH);
	gpio_set_level(COFFEE_F_PORT, HIGH);
	return 0;
}

static int press_put_water_button(void *priv)
{
	LOG_INFO(TAG, "%s called", __func__);
	/* press B & F gpio */
	gpio_set_level(COFFEE_B_PORT, HIGH);
	gpio_set_level(COFFEE_F_PORT, HIGH);
	return 0;
}

static int press_enter_button(void *priv)
{
	LOG_INFO(TAG, "%s called", __func__);
	/* press A & D gpio */
	gpio_set_level(COFFEE_A_PORT, HIGH);
	gpio_set_level(COFFEE_D_PORT, HIGH);
	return 0;
}

static int press_escape_button(void *priv)
{
	LOG_INFO(TAG, "%s called", __func__);
	/* press A & F gpio */
	gpio_set_level(COFFEE_A_PORT, HIGH);
	gpio_set_level(COFFEE_F_PORT, HIGH);
	return 0;
}

static int press_menu_button(void *priv)
{
	LOG_INFO(TAG, "%s called", __func__);
	/* press G & F gpio */
	gpio_set_level(COFFEE_G_PORT, HIGH);
	gpio_set_level(COFFEE_F_PORT, HIGH);
	return 0;
}

static const size_t coffee_gpio_ports[] = {
	COFFEE_A_PORT,
	COFFEE_B_PORT,
	COFFEE_C_PORT,
	COFFEE_D_PORT,
	COFFEE_F_PORT,
	COFFEE_G_PORT
};

static const press_button_func press_buttons[MAX_PRESS_BUTTONS] = {
	[PRESS_UP_BUTTON] = press_up_button,
	[PRESS_DOWN_BUTTON] = press_down_button,
	[PRESS_PUT_WATER_BUTTON] = press_put_water_button,
	[PRESS_ENTER_BUTTON] = press_enter_button,
	[PRESS_ESCAPE_BUTTON] = press_escape_button,
	[PRESS_MENU_BUTTON] = press_menu_button,
};

static const char *button_names[MAX_PRESS_BUTTONS] = {
	[PRESS_UP_BUTTON] = "up button",
	[PRESS_DOWN_BUTTON] = "down button",
	[PRESS_PUT_WATER_BUTTON] = "put water"
};

static const char *coffee_temprature_names[COFFEE_NUMBER_OF_TEMPRATURE_OPTIONS] = {
	[COFFEE_WARM] = "warm",
	[COFFEE_HOT] = "hot",
	[COFFEE_EXTRA_HOT] = "extra hot"
};

static const char *coffee_sizes_names[COFFEE_SIZES_NUMBER_OF_OPTIONS] = {
	[COFFEE_SMALL] = "small",
	[COFFEE_NORMAL] = "medium",
	[COFFEE_BIG] = "big"
};

static const uint8_t coffee_sizes_pressing_counts[COFFEE_SIZES_NUMBER_OF_OPTIONS] = {
	[COFFEE_SMALL] = 6,
	[COFFEE_NORMAL] = 7,
	[COFFEE_BIG] = 8,
};

static void coffee_unpress_all(void)
{
	size_t gpio_port;

	for (gpio_port = 0;
	     gpio_port < ARRAY_SIZE(coffee_gpio_ports); ++gpio_port)
		gpio_set_level(gpio_port, LOW);
}

int coffee_init(void)
{
	LOG_INFO(TAG, "%s was called, initializing the gpios", __func__);

	gpio_pad_select_gpio(COFFEE_A_PORT);
	gpio_set_direction(COFFEE_A_PORT, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(COFFEE_B_PORT);
	gpio_set_direction(COFFEE_B_PORT, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(COFFEE_C_PORT);
	gpio_set_direction(COFFEE_C_PORT, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(COFFEE_D_PORT);
	gpio_set_direction(COFFEE_D_PORT, GPIO_MODE_OUTPUT);
	/* line selectors */
	gpio_pad_select_gpio(COFFEE_F_PORT);
	gpio_set_direction(COFFEE_F_PORT, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(COFFEE_G_PORT);
	gpio_set_direction(COFFEE_G_PORT, GPIO_MODE_OUTPUT);

	coffee_unpress_all();

	return 0;
}

int coffee_press_button(uint8_t button, void *priv)
{
	int ret = -1;

	if (button >= MAX_PRESS_BUTTONS) {
		ret = -1;
		LOG_ERROR(TAG, "invalid button press requested (%d)", button);
		goto err;
	}

	/*
	 * wait a bit for the coffee machine inner controller to recognise our
	 * press.
	 */
	vTaskDelay(100 / portTICK_PERIOD_MS);
	coffee_unpress_all();

	LOG_INFO(TAG, "requested a press of button %d, (%s)",
		button, button_names[button]);
	ret = press_buttons[button](NULL);
	LOG_DEBUG(TAG,
		"sleeping for %d seconds so the machine could detect the press",
		SLEEP_BETWEEN_PRESSES);
	vTaskDelay(
		SLEEP_BETWEEN_PRESSES * ONE_SECOND_IN_MS / portTICK_PERIOD_MS);

err:
	return ret;
}

int coffee_make_coffee(struct coffee_options *choice)
{
	int status = -1;
	uint8_t i;

	if (!choice) {
		LOG_ERROR(TAG, "given choice is NULL");
		goto error_no_choice;
	}

	if ((choice->size < 0) ||
	    (choice->size >= COFFEE_SIZES_NUMBER_OF_OPTIONS)) {
		LOG_ERROR(TAG, "size (%d) is out of bounds", choice->size);
		goto error_bad_coffee_size;
	}

	if ((choice->temprature < 0) ||
	    (choice->temprature >= COFFEE_NUMBER_OF_TEMPRATURE_OPTIONS)) {
		LOG_ERROR(TAG, "temprature (%d) is out of bounds",
			  choice->temprature);
		goto error_bad_coffee_temprature;
	}

	LOG_DEBUG(TAG, "making a %s and %s coffee",
		  coffee_temprature_names[choice->temprature],
		  coffee_sizes_names[choice->size]);

	(void)coffee_press_button(PRESS_MENU_BUTTON, NULL);

	for (i = 0; i < coffee_sizes_pressing_counts[choice->size]; ++i)
		(void)coffee_press_button(PRESS_PUT_WATER_BUTTON, NULL);

	status = 0;
	goto success;

success:
error_bad_coffee_size:
error_bad_coffee_temprature:
error_no_choice:
	return status;
}
