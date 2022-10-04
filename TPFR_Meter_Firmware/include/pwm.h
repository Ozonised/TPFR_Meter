#pragma once

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include "switches.h"
#include "modes.h"

/// @brief Minimun and maximum PWM frequency in the selected range
struct PWM_Freq_Range
{
    const uint32_t MIN_FREQ;
    const uint32_t MAX_FREQ;
    const uint16_t STEP;
};

#define PWM_PIN PB1

#define R0 0
#define R1 1
#define R2 2
#define R3 3

extern LiquidCrystal lcd;
extern uint8_t updateLCD;
extern uint8_t curPos;
extern char lcdStr[15];

extern uint8_t current_PWM_Range;
extern PWM_Freq_Range const pwmFreq[];
extern PGM_P const pwmRangeMenu[];

extern void (*currentMode)(void);
void home();
void PWM_Range_Select();
void pwm();
