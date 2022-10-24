#include <Arduino.h>
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
#include "switches.h"
#include "modes.h"
#include "home.h"
#include "pwm.h"
#include "temperature.h"
#include "freqCounter.h"
#include "rpm.h"

unsigned long currentMillis;
unsigned long prevSwtReadTime;
const uint8_t SWT_READ_INTERVAL = 15;
uint8_t curntSwtState, prevSwtState;

uint8_t curPos;

const uint8_t E = 2, RS = 3, D4 = 4, D5 = 5, D6 = 6, D7 = 7;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

void (*currentMode)(void);

void setup()
{
  // Enabling Pulls ups
  PORTC |= _BV(BACK) | _BV(UP) | _BV(DOWN) | _BV(ENTER);
  lcd.begin(16, 2);
  // Serial.begin(9600);
  updateLCD = 1;
  currentMode = home;
}

void loop()
{
  currentMillis = millis();

  // read the switch state every 15ms
  if ((currentMillis - prevSwtReadTime) >= SWT_READ_INTERVAL)
  {
    curntSwtState = PINC & (_BV(BACK) | _BV(UP) | _BV(DOWN) | _BV(ENTER));

    // checking for long button presses
    // switch states are checked every 15ms
    // a long press has occured if the switch has been pressed for more than 225 ms

    // if the switch is currently pressed and was previously pressed as well and it has not been a long press
    if (curntSwtState == IS_UP && !bitRead(prevSwtState, UP) && !switches.upLongPressed)
    {
      switches.upLongPressed = ++switches.upPressCount >= 33; // ">= 33" because 33 * SWT_READ_INTERVAL = 500ms
    }
    // switch is not pressed
    else if (curntSwtState != IS_UP)
    {
      switches.upPressCount = 0;
      switches.upLongPressed = 0;
    }

    if (curntSwtState == IS_DOWN && !bitRead(prevSwtState, DOWN) && !switches.downLongPressed)
    {
      switches.downLongPressed = ++switches.downPressCount >= 15;
    }
    else if (curntSwtState != IS_DOWN)
    {
      switches.downPressCount = 0;
      switches.downLongPressed = 0;
    }

    prevSwtReadTime = currentMillis;
  }

  // the respective switch is pressed only when it is currently LOW and was previously HIGH
  switches.backPressed = curntSwtState == IS_BACK && bitRead(prevSwtState, BACK);
  switches.upPressed = curntSwtState == IS_UP && bitRead(prevSwtState, UP);
  switches.downPressed = curntSwtState == IS_DOWN && bitRead(prevSwtState, DOWN);
  switches.enterPressed = curntSwtState == IS_ENTER && bitRead(prevSwtState, ENTER);

  if (switches.upPressed || switches.downPressed)
    updateLCD = 1;

  prevSwtState = curntSwtState;

  // decrement curPos only when it's greater than zero
  curPos -= switches.upPressed && curPos;
  // increment curPos only when it's less than 3
  curPos += switches.downPressed && curPos < 3;

  currentMode();
}

// void printMenu(PGM_P const *str)
// {
//   lcd.clear();
//   switch (curPos)
//   {
//   case 0:
//   case 1:
//     strcpy_P(lcdStr, (PGM_P)pgm_read_word(&str[0]));
//     lcd.print(lcdStr);
//     strcpy_P(lcdStr, (PGM_P)pgm_read_word(&str[1]));
//     lcd.setCursor(0, 1);
//     lcd.print(lcdStr);
//     lcd.setCursor(0, curPos);
//     lcd.print('>');
//     break;

//   case 2:
//   case 3:
//     strcpy_P(lcdStr, (PGM_P)pgm_read_word(&str[2]));
//     lcd.print(lcdStr);
//     strcpy_P(lcdStr, (PGM_P)pgm_read_word(&str[3]));
//     lcd.setCursor(0, 1);
//     lcd.print(lcdStr);
//     lcd.setCursor(0, curPos % 2);
//     lcd.print('>');
//     break;

//   default:
//     break;
//   }
// }