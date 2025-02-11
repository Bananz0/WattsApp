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

DisplayHandler display;
TimeHandler timeHandler;

Loads loads(&digitalOutput,&digitalInput);



void updateStats(uint8_t frequency) {
    //Time Interrupt - Moved the div/10 to main
    //Measure available wind turbine capacity and PV capacity then calculate total renewable power capacity
    energyStats.windTurbineCapacity = analogueInput.turbineCurrentCapacity();
    energyStats.pvCapacity = analogueInput.pvCurrentCapacity();

    energyStats.totalRenewablePower = energyStats.windTurbineCapacity + energyStats.pvCapacity;

    //Calculate average power and total energy consumption based on bus voltage and bus current (analogue output)
    energyStats.busbarVoltage = analogueInput.busbarVoltage();
    energyStats.busbarCurrent = analogueInput.busbarCurrent();
    energyStats.busbarPower = energyStats.busbarVoltage * energyStats.busbarCurrent;

    energyStats.totalEnergy = energyStats.averagePower * 100 / 1000;
}

void checkSystemStatus() {
    //TODO
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
ISR(PCINT2_vect) {
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


    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        if (Counter  % 10 == 0) {
            utc++;
            testOutputPin('C',6);
        };
        timeUTC = gmtime((time_t*)&utc); //Update time (hopefully)
        pictorDrawS(reinterpret_cast<const unsigned char *>(timeHandler.returnTime()),display.timePos,WHITE,RED, Mash,1);
        updateCounter = Counter;

        if (updateCounter % 10 == 0) {
            updateStats(0);
            updateCounter = 0;
        }

        //cycle through the screens somehwow
        if (timeUTC->tm_sec % displayDuration == 0 && timeUTC->tm_sec != lastScreenUpdateSecond) {
            screen = static_cast<Screen>((screenPage + 1) % SCREENCOUNT );
            lastScreenUpdateSecond = timeUTC->tm_sec;
        }

        if (emergencyScreen) {
            display.showEmergencyScreen();
            continue;
        }

        if (Counter % 10 == 0) {
            checkSystemStatus();
        }

        display.carouselScreen(screen);

        //Implement Labview Algorithm
        //Default: Charge battery and turn all loads off.
        digitalOutput.chargeBattery();
        loads.turnLoadOff(1);
        loads.turnLoadOff(2);
        loads.turnLoadOff(3);


    }
}
