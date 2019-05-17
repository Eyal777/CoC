#ifndef __COFFEE_H_INCLUDED
#define __COFFEE_H_INCLUDED

#include <stdint.h>

#define COFFEE_A_PORT
#define COFFEE_B_PORT
#define COFFEE_C_PORT
#define COFFEE_D_PORT
#define COFFEE_E_PORT
#define COFFEE_F_PORT
#define COFFEE_G_PORT

#define COFFEE_PUT_WATER_BUTTON
#define COFFEE_UP_BUTTON
#define COFFEE_DOWN_BUTTON

enum button_type {
	PRESS_UP_BUTTON,
	PRESS_DOWN_BUTTON,
	PRESS_PUT_WATER_BUTTON,
	MAX_PRESS_BUTTONS
};

#define COFFEE_INVALID_BUTTON MAX_PRESS_BUTTONS


enum temprature {
	HOT,
	EXTRA_HOT
};

struct coffee_options {
	enum temprature temprature;
};

int coffee_press_button(uint8_t button, void *priv);

int coffee_make_coffee(struct coffee_options options);

#endif
