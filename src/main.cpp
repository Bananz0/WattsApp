#include <time.h>
#include <TimeHandler.h>

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

//Moved all timer functions to PWMHandler for central management

AnalogueInput analogueInput;                        //Starts the ADC up in the AI (analog input) constructor    PORTA1
AnalogueOutput analogueOutput;                      //Starts the PWM up in the AO (analog output) constructor   PORTD7
DigitalInput digitalInput;                          //Start the Digital ISR                                     PORTC0-2
DigitalOutput digitalOutput;                        //very basic                                                PORTC3-7

DisplayHandler display;
TimeHandler timeHandler;

void updateStats(uint8_t frequency) {
    //Time Interrupt
    if (Counter % 10 == 0) {
        //Measure available wind turbine capacity and PV capacity then calculate total renewable power capacity
        energyStats.windTurbineCapacity = analogueInput.turbineCurrentCapacity();
        energyStats.pvCapacity = analogueInput.pvCurrentCapacity();

        energyStats.totalRenewablePower = energyStats.windTurbineCapacity + energyStats.pvCapacity;

        //Calculate average power and total energy consumption based on bus voltage and bus current (analogue output)
        energyStats.busbarVoltage = analogueInput.busbarVoltage();
        energyStats.busbarCurrent = analogueInput.busbarCurrent();
        energyStats.busbarPower = energyStats.busbarVoltage * energyStats.busbarCurrent;

        energyStats.totalEnergy = energyStats.averagePower * 100 / 1000;

        timeUTC = gmtime((time_t*)&utc); //Update time (hopefully)
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
    utc++;
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

    display.startDisplay(false);
    display.clearScreen();
    display.setBacklight(DisplayHandler::LIGHT);
    display.setOrientation(DisplayHandler::LANDSCAPE);
    //display.screenOff();

    //Boot and Initialization
    display.drawBootSequence();

    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        updateStats(0);

        //signOfLife();                                   //Blink LED every .5 sec to show sign of life
        //display.showBusbarScreen();


        pictorDrawS(reinterpret_cast<const unsigned char *>(timeHandler.returnTime()),display.center,WHITE,RED, Mash,2);


        //display.drawText(timeString);

        _delay_ms(100);

    }
}
