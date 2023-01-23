# TPFR_Meter
![TPFR_Meter](/Images/Thumbnail_1.png)

This is an ATmega328p based Temperature, PWM generator, Frequency and RPM meter. 
Often when creating circuits involving MOSFETs or motor drivers, I happen to find a need for a temperature sensor to measure the temperatures of my MOSFETs, a PWM generator to control the speed of motors or to test servos. Not only did have a need for a PWM generator but also a frequency counter when tinkering around with ne555 cicuits based oscillator circuits. Designing separate circuits to fullfill each of my needs would prove to time consuming, expensive and would require additional space to store all the devices. Thus, the idea of creating all purpose meter to measure temperature, generate PWM signals, measure Frequency and RPM struck me which resulted in me creating this multi purpose meter called the TPFR meter. 

# Why did I name it TPFR?
Well TPFR stands for Temperature(T), PWM(P), Frequency(F) and RPM(R) which is representative of the functionality of this meter.

# FEATURES: 
## 1. TEMPERATURE
![TEMPERATURE](/Images/Temperature.jpg)

The temperature measurement is done with the help of a ds18b20 waterproof temperature sensor with a max measurable temperature of 125 degree celsius and a minimum of -55 degree celsius.

## 2. PWM
![PWM](/Images/PWM.png)

The PWM signals are generated using the timer1 of the 328p with a frequency range of 31Hz - 100KHz and a variable duty cycle from 0 - 100%.
The frequency range is subdivied into 4 ranges:

| Range | Frequency Range |
| :---: | :---: |
| 1 | 31Hz - 1KHz |
| 2 | 1KHz - 10KHz |
| 3 | 10KHz - 40KHz |
| 4 | 40KHz - 100KHz |

## 3. FREQUENCY COUNTER
![FREQUENCY_COUNTER](/Images/frequency_2.png)

The frequency counter mode as the name implies measures the frequency of the input signal with an accuracy of 99.8%. The error% is usually around 0.02% - 0.01%. The maximum error% that I recorded was 0.2%. I did not know the maximum measurable frequency since my test setup could only reach a maximum frequency of 100Khz.

## 4. RPM
![RPM](/Images/RPM_2.png)

This mode uses the timer1's input capture register to measure the period of the input signal and then computes the revolution per minute(RPM) and the revolution per second(RPS). The maximum measurable RPM is in the vicinity of 100,000 RPM(approx). Although I do not have the equipment to measure the actual maximum RPM.

 ### Measuring RPM:
 1. To measure the RPM, an [IR proximity sensor](https://hackster.imgix.net/uploads/attachments/471152/obstacle-avoidance-tracking-infrared-sensor-module_S9CxCA7UqZ.jpg?auto=compress&w=900&h=675&fit=min&fm=jpg) is used.
 2. The RPM of the object to measured, a wheel in my case, is covered by a black tape with a white spot on it. The white spot is needed to reflect the IR light which in turn triggers the output of the IR sensor.
 
 ### Note: 
 The output of the IR sensor must be pulled up to VCC using a pull up resistor of 4.7K or so. This is because the IR sensor has an open collector output ,i.e, the output can only be pulled LOW so a pull up resistor is needed to avoid a floating pin.
