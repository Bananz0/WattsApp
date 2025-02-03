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
    ADCVoltage = (ADC * mVref / 1000) / 0x3FF;
    ADCConversionFlag = true;
}
//Counter ISR
ISR(TIMER1_COMPA_vect) {
    Counter++;
    //Time Interrupt
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

        uint16_t current = analogueInput.turbineCurrentCapacity();
        analogueOutput.setMainsCapacity(3.3);

    }
}
