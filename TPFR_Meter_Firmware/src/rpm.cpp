#include "rpm.h"
static uint32_t pulseCount = 0, rotationPerMinute = 0;
static float rotationPerSecond = 0;


void rpm()
{
    TCCR1A = 0;
    // trigger on positive edge and prescaler of 64
    // timer clock of 250KHz and a period of 4uS
    TCCR1B = (1 << ICES1) | (1 << CS11) | (1 << CS10);
    TIMSK1 = (1 << ICIE1) | (1 << TOV1);

    if (input_capt_count == 2)
    {
        if (ICR_Val[0] < ICR_Val[1])
        {
            pulseCount = ICR_Val[1] - ICR_Val[0];
        }

        // timer counter has overflown
        if (ICR_Val[0] > ICR_Val[1])
        {
            // (timer1_ovf_count - 1) to ignore the first overflow
            pulseCount = (65535UL - ICR_Val[0]) + (timer1_ovf_count - 1) * 65535UL + ICR_Val[1];
        }

        // timer has a clock of 250KHz and a period of 4uS
        // therefore pulse count must be multiplied by 4 to get the period of the input signal
        pulseCount *= 4;
        rotationPerSecond = 1000000UL / pulseCount;
        rotationPerSecond += 1; // + 1 offset

        rotationPerMinute = rotationPerSecond * 60;
        input_capt_count = 0;
        timer1_ovf_count = 0;
        updateLCD = 1;
    }

    // update the lcd when the frequency has been calculated and it has been 500ms since the last update
    if (updateLCD && (currentMillis - lastLcdUpdateTIme) >= lcdUpdateInterval)
    {
        lcd.clear();
        lcd.print(F(" RPM: "));
        lcd.print(rotationPerMinute);
        lcd.setCursor(0, 1);
        lcd.print(F(" RPS: "));
        lcd.print(rotationPerSecond);
        rotationPerSecond = 0;
        rotationPerMinute = 0;
        updateLCD = 0;
        lastLcdUpdateTIme = currentMillis;
    }

    if (switches.backPressed)
    {
        TCCR1A = 0;
        TCCR1B = 0;
        TCCR1C = 0;
        TIMSK1 = 0;
        curPos = RPM - 1;
        updateLCD = 1;
        currentMode = home;
    }
}