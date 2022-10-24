#include "temperature.h"

static const uint8_t DS18B20_PIN = 11;
static uint8_t ds18b20_Init_Flag = 1;
static float c_temp, f_temp;
static unsigned long prevTempReadTime;
static const uint16_t TEMP_READ_INTERVAL = 1000;

OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);

void temp()
{
    if (ds18b20_Init_Flag)
    {
        ds18b20.begin();
        ds18b20.setResolution(12);
        ds18b20_Init_Flag = 0;
    }

    if ((currentMillis - prevTempReadTime) >= TEMP_READ_INTERVAL)
    {
        ds18b20.requestTemperatures();
        c_temp = ds18b20.getTempCByIndex(0);
        f_temp = ((c_temp * 9.0) / 5.0) + 32;
        updateLCD = 1;
        prevTempReadTime = currentMillis;
    }

    if (updateLCD)
    {
        lcd.clear();
        lcd.print(F("  Temperature"));
        lcd.setCursor(0, 1);
        lcd.print(c_temp);
        lcd.print(F("C  "));
        lcd.print(f_temp);
        lcd.print('F');
        updateLCD = 0;
    }

    if (switches.backPressed)
    {
        ds18b20_Init_Flag = 1;
        curPos = TEMP - 1;
        updateLCD = 1;
        currentMode = home;
    }
}