#include "freqCounter.h"

static uint8_t gateTime = 100, freq_is_under_1KHz = 0;
const unsigned short lcdUpdateInterval = 500;
volatile uint32_t ICR_Val[2];
volatile uint8_t input_capt_count = 0, timer1_ovf_count = 0;
static uint32_t pulseCount, frequency;
uint32_t lastLcdUpdateTIme;

void freqCounter()
{
    TCCR1A = 0;

    /*
        For frequencies above 1KHz, timer1 is used as a counter to count the number pulses on T1 pin
        for a gate time of 100ms or 0.1s. While for frequencies below 1KHz, timer1's input capture unit
        is utilised.
    */
    if (!freq_is_under_1KHz)
    {

        uint32_t start = micros();
        TIMSK1 = (1 << TOV1);
        // clock source on T1 pin clock on rising edge
        TCCR1B = _BV(CS10) | _BV(CS11) | _BV(CS12);
        TCNT1 = 0;
        while (gateTime > 0 && !switches.backPressed)
        {
            while (gateTime > 0 && (micros() - start) >= 1000)
            {
                gateTime--;
                start += 1000;
            }
        }

        TCCR1B = 0;
        gateTime = 100;
        frequency = TCNT1 + timer1_ovf_count * 65535;
        frequency *= (1000 / gateTime);
        timer1_ovf_count = 0;
        updateLCD = 1;
    }
    else
    {
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
            frequency = 1000000UL / pulseCount;
            frequency += 1; // "+ 1" for calibration
            input_capt_count = 0;
            timer1_ovf_count = 0;
            updateLCD = 1;
        }
    }

    // update the lcd when the frequency has been calculated and it has been 500ms since the last update
    if (updateLCD && (currentMillis - lastLcdUpdateTIme) >= lcdUpdateInterval)
    {
        lcd.clear();
        lcd.print(F("   FREQUENCY"));
        lcd.setCursor(3, 1);
        lcd.print(frequency);
        lcd.print(F("Hz"));

        if (frequency < 1000)
            freq_is_under_1KHz = 1;
        else
            freq_is_under_1KHz = 0;

        updateLCD = 0;
        lastLcdUpdateTIme = currentMillis;
    }

    if (switches.backPressed)
    {
        TCCR1A = 0;
        TCCR1B = 0;
        TCCR1C = 0;
        TIMSK1 = 0;
        curPos = FREQ_COUNTER - 1;
        updateLCD = 1;
        currentMode = home;
    }
}

ISR(TIMER1_OVF_vect)
{
    timer1_ovf_count++;
}

ISR(TIMER1_CAPT_vect)
{
    ICR_Val[input_capt_count] = ICR1;
    if (input_capt_count)
    {
        TIMSK1 &= ~(_BV(TOV1) | _BV(ICIE1));
    }
    input_capt_count++;
}