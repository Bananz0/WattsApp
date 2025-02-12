#include <time.h>
#include <TimeHandler.h>

#include "globalVariables.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "debug.h"

#include "portHandler.h"

#include "AnalogueInput.h"
#include "AnalogueOutput.h"
#include "DigitalInput.h"
#include "DigitalOutput.h"

#include "DisplayHandler.h"

#include "Loads.h"
#include "Sources.h"

//Moved all timer functions to PWMHandler for central management

AnalogueInput analogueInput;                        //Starts the ADC up in the AI (analog input) constructor    PORTA1
AnalogueOutput analogueOutput;                      //Starts the PWM up in the AO (analog output) constructor   PORTD7
DigitalInput digitalInput;                          //Start the Digital ISR                                     PORTC0-2
DigitalOutput digitalOutput;                        //very basic                                                PORTC3-7

Loads loads(&digitalOutput,&digitalInput);
Sources sources(&analogueInput, &analogueOutput,&digitalOutput);

DisplayHandler display(&loads,&sources);
TimeHandler timeHandler;

void updateStats() {
    //Time Interrupt - Moved the div/10 to main
    //Measure available wind turbine capacity and PV capacity then calculate total renewable power capacity
    sources.readTurbineCapacity();
    sources.readPvCapacity();

    //Calculate average power and total energy consumption based on bus voltage and bus current (analogue output)
    sources.readBusbarCurrent();
    sources.readBusbarVoltage();

    sources.calculateTotalEnergyandPower();

    sources.totalEnergy = sources.averagePower * 100 / 1000;
}

//ADC ISR
ISR(ADC_vect){
    ADCVoltage = (ADC * Vref) / 0x3FF;
    ADCConversionFlag = true;
}
//Counter ISR
ISR(TIMER1_COMPA_vect) {
   // PORTC ^= (1 << PC7); //for testing - interrupt doesn't work with functions in it for some reason
    Counter++;
    //utc++;
}
//Pin Change ISR
ISR(PCINT0_vect) {
    // Read Load Calls 1, 2 and 3
    loads.checkLoadCallChanges();
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

    uint16_t displayDuration = 4;
    uint16_t updateCounter = 0;
    Screen screen;
    uint8_t lastScreenUpdateSecond = -1;
    uint16_t lastCounter = 0;

    //CLion complains about forever while loop
    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        if ((Counter % 10 == 0) && (Counter != lastCounter)) {
            utc++;
            lastCounter = Counter;
        };
        timeUTC = gmtime((time_t*)&utc); //Update time (hopefully)
        pictorDrawS(reinterpret_cast<const unsigned char *>(timeHandler.returnTime()),display.timePos,WHITE,RED, Mash,1);
        updateCounter = Counter;

        if (updateCounter % 10 == 0) {
            updateStats();
            updateCounter = 0;
        }

        //cycle through the screens somehwow
        if (timeUTC->tm_sec % displayDuration == 0 && timeUTC->tm_sec != lastScreenUpdateSecond) {
            if (emergencyScreen) {
                //Display screen for 5 seconds
                screen = ERROR_SCREEN;
                emergencyScreen = false;
            } else if (!emergencyScreen) {
                screen = static_cast<Screen>((screenPage + 1) % ((int)SCREENCOUNT-1) ); //Subtraced 1 to never cycle to the error screebb
                lastScreenUpdateSecond = timeUTC->tm_sec;
            }
        }

        // if (sources.busbarVoltage > 2.5) {
        //     sources.requestMains(11);
        // }
        //Used for testing the error screen which hopefully will never have to happen

        display.carouselScreen(screen);


        //Implement Labview Algorithm
        //Default: Charge battery and turn all loads off.
        sources.requestMains(0);
        sources.chargeBattery();
        loads.turnLoadOff(1);
        loads.turnLoadOff(2);
        loads.turnLoadOff(3);


    }
}
