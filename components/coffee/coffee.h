#ifndef __COFFEE_H_INCLUDED
#define __COFFEE_H_INCLUDED

#include <stdint.h>

#include "driver/gpio.h"

#define COFFEE_A_PORT GPIO_NUM_16
#define COFFEE_B_PORT GPIO_NUM_17
#define COFFEE_C_PORT GPIO_NUM_18
#define COFFEE_D_PORT GPIO_NUM_19
/* this will be Vcc */
#define COFFEE_E_PORT
#define COFFEE_F_PORT GPIO_NUM_21
#define COFFEE_G_PORT GPIO_NUM_22
/* This would be the ground */
#define COFFEE_H_PORT

enum button_type {
	PRESS_UP_BUTTON,
	PRESS_DOWN_BUTTON,
	PRESS_PUT_WATER_BUTTON,
	PRESS_ENTER_BUTTON,
	PRESS_ESCAPE_BUTTON,
	PRESS_MENU_BUTTON,
	MAX_PRESS_BUTTONS
};

#define COFFEE_INVALID_BUTTON MAX_PRESS_BUTTONS


enum coffee_temprature {
	COFFEE_WARM,
	COFFEE_HOT,
	COFFEE_EXTRA_HOT,
	COFFEE_NUMBER_OF_TEMPRATURE_OPTIONS
};

enum coffee_size {
	COFFEE_SMALL,
	COFFEE_NORMAL,
	COFFEE_BIG,
	COFFEE_SIZES_NUMBER_OF_OPTIONS
};

struct coffee_options {
	enum coffee_temprature temprature;
	enum coffee_size size;
};

extern enum coffee_temprature coffee_current_temprature;

int coffee_init(void);

int coffee_press_button(uint8_t button, void *priv);

int coffee_make_coffee(struct coffee_options *choice);

#endif
