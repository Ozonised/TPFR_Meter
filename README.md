# TPFR_Meter
This is an ATmega328p based Temperature, PWM generator, Frequency and RPM meter. 
Often when creating circuits involving MOSFETs or motor drivers, I happen to find a need for a temperature sensor to measure the temperatures of my MOSFETs, a PWM generator to control the speed of motors or to test servos. Not only did have a need for a PWM generator but also a frequency counter when tinkering around with ne555 cicuits based oscillator circuits. Designing separate circuits to fullfill each of my needs would prove to time consuming, expensive and would require additional space to store all the devices. Thus, the idea of creating all purpose meter to measure temperature, generate PWM signals, measure Frequency and RPM struck me which resulted in me creating this multi purpose meter called the TPFR meter. 

# Why did I name it TPFR?
Well TPFR stands for Temperature(T), PWM(P), Frequency(F) and RPM(R) which is what this meter was designed to measure.

# FEATURES: 
## 1. TEMPERATURE
The temperature measurement is done with the help of a ds18b20 waterproof temperature sensor with a max measurable temperature of 125 degree celsius and a minimum of -55 degree celsius.

## 2. PWM
The PWM signals are generated using the timer1 of the 328p with a frequency range of 31Hz - 100KHz and a variable duty cycle from 0 - 100%.
The frequency range is subdivied into 4 ranges:

| Range | Frequency Range |
| :---: | :---: |
| 1 | 31Hz - 1KHz |
| 2 | 1KHz - 10KHz |
| 3 | 10KHz - 40KHz |
| 4 | 40KHz - 100KHz |
