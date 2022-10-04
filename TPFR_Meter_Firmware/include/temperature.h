#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "switches.h"
#include "modes.h"

extern LiquidCrystal lcd;
extern uint8_t updateLCD;
extern uint8_t curPos;

extern unsigned long currentMillis;
extern float c_temp, f_temp;
extern uint8_t ds18b20_Init_Flag;

extern void (*currentMode)(void);
void home();
void temp();
