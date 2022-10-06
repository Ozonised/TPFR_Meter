#include "pwm.h"

int32_t cpuClkOffset = 0;

static uint8_t current_PWM_Range;
static uint16_t increment_step;
static uint32_t currentMinFreq;
static uint32_t current_PWM_Freq;
static uint32_t currentMaxFreq;
static uint32_t PWM_Duty_Cycle;
static uint32_t prevIncrementTime;

PWM_Freq_Range const pwmFreq[4] PROGMEM = {
    {31, 1000, 1},
    {1000, 10000, 10},
    {10000, 40000, 100},
    {40000, 100000, 1000}};

static const char PWM_R0[] PROGMEM = " 31Hz - 1kHz";
static const char PWM_R1[] PROGMEM = " 1kHz - 10kHz";
static const char PWM_R2[] PROGMEM = " 10kHz - 40kHz";
static const char PWM_R3[] PROGMEM = " 40kHz - 100kHz";

PGM_P const pwmRangeMenu[] PROGMEM = {PWM_R0, PWM_R1, PWM_R2, PWM_R3};

void pwm();
static void set_PWM_Freq(uint32_t freq, uint32_t dutyCycle, uint8_t range);
static void set_PWM_DutyCycle(uint32_t dutyCycle);

void set_PWM_Freq(uint32_t freq, uint32_t dutyCycle, uint8_t range)
{
    uint16_t temp_ICR1 = 0;
    uint8_t prescaler;

    // Fast non-inverted PMW with ICR1 as top
    TCCR1A = _BV(COM1A1) | _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12);

    if (range)
    {
        // prescaler = 1 for frequency range 1kHz - 100kHz
        TCCR1B &= ~_BV(CS11);
        TCCR1B |= _BV(CS10);
        prescaler = 1;
    }
    else
    {
        // prescaler = 8 for frequency range 31Hz - 1kHz
        TCCR1B &= ~_BV(CS10);
        TCCR1B |= _BV(CS11);
        prescaler = 8;
    }

    temp_ICR1 = ((F_CPU / prescaler) / freq) - 1;

    // update the frequency only if ICR1 has changed
    // temp_ICR1 will not be equal to ICR1 if the frequency has changed
    if (temp_ICR1 != ICR1)
    {
        TCNT1 = 1;
        ICR1 = temp_ICR1;
    }
    OCR1A = (temp_ICR1 * dutyCycle) / 100;

    // PWM pin as output
    bitSet(DDRB, PWM_PIN);
}

void set_PWM_DutyCycle(uint32_t dutyCycle)
{
    OCR1A = (ICR1 * dutyCycle) / 100;
}

void PWM_Range_Select()
{
    if (updateLCD)
    {
        lcd.clear();
        switch (curPos)
        {
        case 0:
        case 1:
            strcpy_P(lcdStr, (PGM_P)pgm_read_word(&pwmRangeMenu[0]));
            lcd.print(lcdStr);
            strcpy_P(lcdStr, (PGM_P)pgm_read_word(&pwmRangeMenu[1]));
            lcd.setCursor(0, 1);
            lcd.print(lcdStr);
            lcd.setCursor(0, curPos);
            lcd.print('>');
            break;

        case 2:
        case 3:
            strcpy_P(lcdStr, (PGM_P)pgm_read_word(&pwmRangeMenu[2]));
            lcd.print(lcdStr);
            strcpy_P(lcdStr, (PGM_P)pgm_read_word(&pwmRangeMenu[3]));
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
        currentMode = pwm;
        current_PWM_Range = curPos;
        curPos = 0;

        currentMinFreq = pgm_read_dword(&pwmFreq[current_PWM_Range].MIN_FREQ); // minimum frequency for the selected range
        currentMaxFreq = pgm_read_dword(&pwmFreq[current_PWM_Range].MAX_FREQ); // maximum frequency for the selected range
        increment_step = pgm_read_word(&pwmFreq[current_PWM_Range].STEP);      // increment or decrement step size for the selected range
        current_PWM_Freq = currentMinFreq;                                     // default frequency
        PWM_Duty_Cycle = 50;                                                   // default duty cycle

        set_PWM_Freq(current_PWM_Freq, PWM_Duty_Cycle, current_PWM_Range);

        updateLCD = 1;
    }

    if (switches.backPressed)
    {
        currentMode = home;
        curPos = PWM_RANGE - 1; // cursor at row one
        updateLCD = 1;
    }
}

void pwm()
{
    static bool setFreq = 0;
    static bool setDuty = 0;
    static uint8_t pwmCur = 0;

    // increment or decrement pwmCur only when we are not in setFreq or setDuty mode
    if (!setFreq && !setDuty)
    {
        // decrement pwmCur only when it's greater than zero
        pwmCur -= switches.upPressed && pwmCur;
        // increment pwmCur only when it's less than 1
        pwmCur += switches.downPressed && pwmCur < 1;
    }

    if (switches.enterPressed)
    {
        switch (pwmCur)
        {
        case 0:
            // set frequency mode
            setDuty = 0;
            setFreq = !setFreq;
            break;

        case 1:
            // set duty cycle mode
            setFreq = 0;
            setDuty = !setDuty;
            break;

        default:
            break;
        }

        updateLCD = 1;
    }

    if (setFreq)
    {
        if (current_PWM_Freq < currentMaxFreq)
        {
            if (switches.upPressed)
                current_PWM_Freq += increment_step;

            // for long button press increment the current_PWM_Freq every 50ms
            if (switches.upLongPressed && (currentMillis - prevIncrementTime) >= 50)
            {
                current_PWM_Freq += increment_step;
                prevIncrementTime = currentMillis;
                updateLCD = 1;
            }
        }

        if (current_PWM_Freq > currentMinFreq)
        {
            if (switches.downPressed)
                current_PWM_Freq -= increment_step;

            // for long button press decrement the current_PWM_Freq every 50ms
            if (switches.downLongPressed && (currentMillis - prevIncrementTime) >= 50)
            {
                current_PWM_Freq -= increment_step;
                prevIncrementTime = currentMillis;
                updateLCD = 1;
            }
        }

        set_PWM_Freq(current_PWM_Freq, PWM_Duty_Cycle, current_PWM_Range);
    }

    if (setDuty)
    {
        PWM_Duty_Cycle += switches.upPressed && PWM_Duty_Cycle < 100;
        PWM_Duty_Cycle -= switches.downPressed && PWM_Duty_Cycle;

        if (switches.upLongPressed && (currentMillis - prevIncrementTime) >= 50)
        {
            PWM_Duty_Cycle += PWM_Duty_Cycle < 100;
            prevIncrementTime = currentMillis;
            updateLCD = 1;
        }

        if (switches.downLongPressed && (currentMillis - prevIncrementTime) >= 50)
        {
            PWM_Duty_Cycle -= PWM_Duty_Cycle > 0;
            prevIncrementTime = currentMillis;
            updateLCD = 1;
        }

        set_PWM_DutyCycle(PWM_Duty_Cycle);
    }

    if (updateLCD)
    {
        lcd.clear();
        lcd.print(F(" FREQ: "));

        // set the display resolution depending on the PWM range
        switch (current_PWM_Range)
        {
        case 0:
            lcd.print(current_PWM_Freq);
            lcd.print("Hz");
            break;

        case 1:
            lcd.print(current_PWM_Freq / 1000.0);
            lcd.print(F("kHz"));
            break;

        case 2:
            lcd.print(current_PWM_Freq / 1000.0, 1);
            lcd.print(F("kHz"));
            break;

        case 3:
            lcd.print(current_PWM_Freq / 1000);
            lcd.print(F("kHz"));
            break;

        default:
            break;
        }

        lcd.setCursor(0, 1);
        lcd.print(F(" Duty: "));
        lcd.print(PWM_Duty_Cycle);
        lcd.print('%');

        if (setFreq || setDuty)
        {
            // and on the 15th column 1st row when changing frequency
            // cursor on 15th column and 2nd row when changing duty cycle
            lcd.setCursor(15, pwmCur);
            lcd.print('<');
        }
        else
        {
            lcd.setCursor(0, pwmCur);
            lcd.print('>');
        }

        updateLCD = 0;
    }

    // go back to range select mode
    if (switches.backPressed)
    {
        bitClear(DDRB, PWM_PIN); // disable PWM output

        // reset timer1's registers
        TCNT1 = 0;
        ICR1 = 0;
        TCCR1B = 0;
        TCCR1A = 0;

        setFreq = 0;
        setDuty = 0;
        pwmCur = 0;
        curPos = current_PWM_Range;
        currentMode = PWM_Range_Select;
        updateLCD = 1;
    }
}