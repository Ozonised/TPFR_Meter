#include "switches.h"

// value on PORTC when the specified button is pressed
const uint8_t IS_BACK = 0x0E;
const uint8_t IS_UP = 0x0D;
const uint8_t IS_DOWN = 0x0B;
const uint8_t IS_ENTER = 0x07;

ButtonState switches;