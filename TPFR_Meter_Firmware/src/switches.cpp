#include "switches.h"

const uint8_t IS_BACK = 0x0E;   // value on PORTC when the back button is pressed
const uint8_t IS_UP = 0x0D;     // value on PORTC when the up button is pressed
const uint8_t IS_DOWN = 0x0B;   // value on PORTC when the down button is pressed
const uint8_t IS_ENTER = 0x07;  // value on PORTC when the enter button is pressed

ButtonState switches;