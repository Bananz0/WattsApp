#include "globalVariables.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "debug.h"

#include "portHandler.h"

#include "AnalogueInput.h"
#include "AnalogueOutput.h"
#include "DigitalInput.h"
#include "DigitalOutput.h"

#define TARGET_TIME_MS 100
#define PRESCALER 64
// #define F_CPU 12000000 This is already defined in the CMake

volatile uint32_t Counter=0;//Only used for time interrupt

// Timer initialization function
void initializeTimer1() {
    TCCR1B |= (1 << WGM12);
    OCR1A = (F_CPU / (PRESCALER * 1000)) * TARGET_TIME_MS - 1;
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << CS11) | (1 << CS10);
}

AnalogueInput analogueInput;                        //Starts the ADC up in the AI (analog input) constructor
AnalogueOutput analogueOutput;                      //Starts the PWM up in the AO (analog output) constructor
DigitalInput digitalInput;                          //Start the Digital ISR
DigitalOutput digitalOutput;                        //very basic

//ADC ISR
ISR(ADC_vect){
    ADCVoltage = (float)(ADC * Vref) / 0x3FF;
    ADCConversionFlag = 1;
}
//Counter ISR
ISR(TIMER1_COMPA_vect) {
    Counter++;
    //Time Interrupt
    if (Counter % 10 == 0) {
        //Measure available wind turbine capacity and PV capacity then calculate total renewable power capacity
        float windTurbineCapacity = analogueInput.turbineCurrentCapacity();
        float pvCapacity = analogueInput.pvCurrentCapacity();
        float totalRenewablePower = windTurbineCapacity + pvCapacity;

        //Calculate average power and total energy consumption based on bus voltage and bus current (analogue output)
        float busbarVoltage = analogueInput.busbarVoltage();
        float busbarCurrent = analogueInput.busbarCurrent();
        float averagePower = busbarVoltage * busbarCurrent;
        float totalEnergy = averagePower * TARGET_TIME_MS / 1000;
    }
}
//Pin Change ISR
ISR(PCINT2_vect) {
    // Read Load Calls 1, 2 and 3
    digitalInput.checkLoadCallChanges();
}

int main() {
    sei();                                              //Enable Global interrupts

    finalizePorts();

    testLight(1);

    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        //signOfLife();                                 //Blink LED every .5 sec to show sign of life

        testOutputPin('B', 0);

        uint16_t current = analogueInput.busbarCurrent();
        analogueOutput.setMainsCapacity(current);

    }
}
