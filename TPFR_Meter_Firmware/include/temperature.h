#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "home.h"
#include "switches.h"
#include "modes.h"

extern LiquidCrystal lcd;
extern uint8_t updateLCD;
extern uint8_t curPos;

extern unsigned long currentMillis;

extern void (*currentMode)(void);
void temp();
