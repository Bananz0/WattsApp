#include "globalVariables.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "debug.h"
#include "uart.h"

#include "portHandler.h"

#include "AnalogueInput.h"
#include "AnalogueOutput.h"
#include "DigitalInput.h"
#include "DigitalOutput.h"

#include "DisplayHandler.h"

#define TARGET_TIME_MS 100
#define PRESCALER 64

AnalogueInput analogueInput;                        //Starts the ADC up in the AI (analog input) constructor    PORTA1
//AnalogueOutput analogueOutput;                      //Starts the PWM up in the AO (analog output) constructor   PORTD7
DigitalInput digitalInput;                          //Start the Digital ISR                                     PORTC0-2
DigitalOutput digitalOutput;                        //very basic                                                PORTC3-7

DisplayHandler display;

// Timer initialization function
void initializeTimer1() {
    TCCR1B |= (1 << WGM12);
    OCR1A = (F_CPU / (PRESCALER * 1000)) * TARGET_TIME_MS - 1;
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << CS11) | (1 << CS10);
}

void updateStats(uint8_t frequency) {
    //Time Interrupt
    if (Counter % 10 == 0) {
        //Measure available wind turbine capacity and PV capacity then calculate total renewable power capacity
        windTurbineCapacity = analogueInput.turbineCurrentCapacity();
        pvCapacity = analogueInput.pvCurrentCapacity();
        totalRenewablePower = windTurbineCapacity + pvCapacity;

        //Calculate average power and total energy consumption based on bus voltage and bus current (analogue output)
        busbarVoltage = analogueInput.busbarVoltage();
        busbarCurrent = analogueInput.busbarCurrent();
        averagePower = busbarVoltage * busbarCurrent;
        totalEnergy = averagePower * TARGET_TIME_MS / 1000;
    }
}

//ADC ISR
ISR(ADC_vect){
    ADCVoltage = (ADC * Vref) / 0x3FF;
    ADCConversionFlag = true;
}
//Counter ISR
ISR(TIMER1_COMPA_vect) {
    PORTC ^= (1 << PC7); //for testing - interrupt doesn't work with functions in it for some reason
    Counter++;
}
//Pin Change ISR
ISR(PCINT2_vect) {
    // Read Load Calls 1, 2 and 3
    digitalInput.checkLoadCallChanges();
}

int main() {
    finalizePorts();
    testLight(1);                              //Boot Light
    sei();                                               //Enable Global interrupts
    initializeTimer1();

    display.startDisplay(false);
    display.clearScreen();
    display.setBacklight(DisplayHandler::LIGHT);
    display.setOrientation(DisplayHandler::LANDSCAPE);
    //display.screenOff();

    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        updateStats(0);
        //signOfLife();                                   //Blink LED every .5 sec to show sign of life
        display.showBusbarScreen();

    }
}
