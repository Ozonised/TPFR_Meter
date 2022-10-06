#pragma once

#include <Arduino.h>

#define SWITCH_PORT PINC
#define BACK PC0
#define UP PC1
#define DOWN PC2
#define ENTER PC3

extern const uint8_t IS_BACK;
extern const uint8_t IS_UP;  
extern const uint8_t IS_DOWN; 
extern const uint8_t IS_ENTER;

struct ButtonState
{
    uint8_t backPressed : 1;
    uint8_t upPressed : 1;
    uint8_t downPressed : 1;
    uint8_t enterPressed : 1;

    uint8_t upLongPressed : 1;
    uint8_t downLongPressed : 1;

    uint8_t res : 2;

    uint8_t upPressCount;
    uint8_t downPressCount;
};

extern ButtonState switches;
