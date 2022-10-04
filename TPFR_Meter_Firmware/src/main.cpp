#include <Arduino.h>
#include <avr/pgmspace.h>
#include "pins.h"
#include "switches.h"
#include <modes.h>
#include "pwm.h"
#include "temperature.h"
#include <LiquidCrystal.h>

uint8_t updateLCD;

unsigned long currentMillis;
unsigned long prevSwtReadTime;
const uint8_t SWT_READ_INTERVAL = 15;
uint8_t curntSwtState, prevSwtState;

uint8_t curPos;

static const char tempStr[] PROGMEM = " Temperature";
static const char pwmStr[] PROGMEM = " PWM";
static const char freqStr[] PROGMEM = " Frequency";
static const char rpmStr[] PROGMEM = " RPM";

PGM_P const lcdHome[] PROGMEM = {tempStr, pwmStr, freqStr, rpmStr};

char lcdStr[15];

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

void home();
void freq();
void rpm();
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

  // read the switch state every SWT_READ_INTERVAL
  if ((currentMillis - prevSwtReadTime) >= SWT_READ_INTERVAL)
  {
    curntSwtState = PINC & (_BV(BACK) | _BV(UP) | _BV(DOWN) | _BV(ENTER));
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

void home()
{
  // updating the home screen contents
  if (updateLCD)
  {
    lcd.clear();
    switch (curPos)
    {
    case 0:
    case 1:
      strcpy_P(lcdStr, (PGM_P)pgm_read_word(&lcdHome[0]));
      lcd.print(lcdStr);
      strcpy_P(lcdStr, (PGM_P)pgm_read_word(&lcdHome[1]));
      lcd.setCursor(0, 1);
      lcd.print(lcdStr);
      lcd.setCursor(0, curPos);
      lcd.print('>');
      break;

    case 2:
    case 3:
      strcpy_P(lcdStr, (PGM_P)pgm_read_word(&lcdHome[2]));
      lcd.print(lcdStr);
      strcpy_P(lcdStr, (PGM_P)pgm_read_word(&lcdHome[3]));
      lcd.setCursor(0, 1);
      lcd.print(lcdStr);
      lcd.setCursor(0, curPos % 2);
      lcd.print('>');
      break;

    default:
      break;
    }
    updateLCD = 0;
  }

  if (switches.enterPressed)
  {
    switch (curPos + 1)
    {
    case 1:
      currentMode = temp;
      break;

    case 2:
      currentMode = PWM_Range_Select;
      break;

    case 3:
      currentMode = freq;
      break;

    case 4:
      currentMode = rpm;
      break;

    default:
      currentMode = home;
      break;
    }
    curPos = 0;
    updateLCD = 1;
  }
}

void rpm()
{

}

void freq()
{

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