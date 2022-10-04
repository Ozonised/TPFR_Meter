#pragma once

#include <Arduino.h>

#define SWITCH_PORT PINC
#define BACK PC0
#define UP PC1
#define DOWN PC2
#define ENTER PC3

// value on PORTC when the specified button is pressed

extern const uint8_t IS_BACK;          // value on PORTC when the back button is pressed
extern const uint8_t IS_UP;            // value on PORTC when the up button is pressed
extern const uint8_t IS_DOWN;          // value on PORTC when the down button is pressed
extern const uint8_t IS_ENTER;         // value on PORTC when the enter button is pressed

struct ButtonState
{
    uint8_t backPressed : 1;
    uint8_t upPressed : 1;
    uint8_t downPressed : 1;
    uint8_t enterPressed : 1;

    uint8_t upLongPressed : 1;
    uint8_t downLongPressed : 1;

    uint8_t res : 2;
};

extern ButtonState switches;
