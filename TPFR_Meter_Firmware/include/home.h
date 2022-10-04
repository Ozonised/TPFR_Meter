#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
#include "switches.h"
#include "pwm.h"
#include "temperature.h"

extern LiquidCrystal lcd;
extern uint8_t curPos;
extern uint8_t updateLCD;
extern char lcdStr[15];
extern void (*currentMode)(void);

void freq();
void rpm();
void home();
