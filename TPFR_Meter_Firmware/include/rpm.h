#pragma once 

#include <Arduino.h>
#include "common.h"

extern volatile uint8_t input_capt_count, timer1_ovf_count;
extern volatile uint32_t ICR_Val[2];
extern const unsigned short lcdUpdateInterval;
extern uint32_t lastLcdUpdateTIme;


void rpm();