#pragma once

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include "home.h"
#include "switches.h"
#include "modes.h"

extern char lcdStr[15];
extern LiquidCrystal lcd;
extern uint8_t curPos;
extern uint8_t updateLCD;
extern void (*currentMode)(void);