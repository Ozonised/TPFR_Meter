#include "home.h"

uint8_t updateLCD;
char lcdStr[15];

static const char tempStr[] PROGMEM = " Temperature";
static const char pwmStr[] PROGMEM = " PWM";
static const char freqStr[] PROGMEM = " Frequency";
static const char rpmStr[] PROGMEM = " RPM";

PGM_P const lcdHome[] PROGMEM = {tempStr, pwmStr, freqStr, rpmStr};

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
