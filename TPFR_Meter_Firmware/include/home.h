#pragma once

#include "common.h"

#include "pwm.h"
#include "temperature.h"
#include "rpm.h"
#include "freqCounter.h"

extern LiquidCrystal lcd;
extern uint8_t curPos;
extern uint8_t updateLCD;
extern char lcdStr[15];
extern void (*currentMode)(void);

void home();
