#include "globalVariables.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "debug.h"

#include "portHandler.h"
#include "PWMHandler.h"
#include "ADCHandler.h"


#define TARGET_TIME_MS 100
#define PRESCALER 64
// #define F_CPU 12000000 This is already defined in the CMake

//ISR ADC
ISR(ADC_vect){
    ADCVoltage = (float)(ADC * Vref) / 0x3FF;
    ADCConversionFlag = 1;
}
//Counter ADC
ISR(TIMER1_COMPA_vect) {
    Counter++;
    //Time Interrupt
}

// Timer initialization function
void initializeTimer1() {
    TCCR1B |= (1 << WGM12);
    OCR1A = (F_CPU / (PRESCALER * 1000)) * TARGET_TIME_MS - 1;
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << CS11) | (1 << CS10);
}

class AnalogueInput {
public:
    AnalogueInput() {
        aDCHandler.initializeADC();
    }
    float turbineCurrentCapacity() { //Read and return turbine capacity current
        //Read Pin A2 using ADC
        aDCHandler.getVoltage(2);
        return ADCVoltage; //Amps 1:1 mapping
    }

    float pvCurrentCapacity() { //Read and return the PV Capacity current
        //Read Pin A3 using ADC
        aDCHandler.getVoltage(3);
        return ADCVoltage; //Amps 1:1 mapping
    }

    float busbarCurrent() {
        //Read Pin A1 using ADC
        aDCHandler.getVoltage(1);
        return ADCVoltage; //Amps 1:1 mapping
    }

    float busbarVoltage() {
        //Read Pin A0 using ADC (scaled down from 10v)
        aDCHandler.getVoltage(0);
        return 100.0f * ADCVoltage; //Volts
    }

private:
    ADCHandler aDCHandler;
}; //First RJ45 Port A

class AnalogueOutput {
    public:
    AnalogueOutput() {
        pWMHandler.initializePWM();
    }
    void setMainsCapacity(uint16_t mainsCapacity) { //Set Mains Capacity using PWM from 0 to 10v
        pWMHandler.setOutputVoltage(mainsCapacity);
    }
private:
    PWMHandler pWMHandler;

};//Second RJ45 Port D

class DigitalInput {
    public:
    DigitalInput() {
    }
    bool readLoad1() { //Pin C0

    }
    bool readLoad2() { //Pin C1

    }
    bool readLoad3() { //Pin C2

    }

};  //Third RJ45 Port C (0 to 2)

class DigitalOutput {
public:
    DigitalOutput() : load1Status(false), load2Status(false), load3Status(false) {

    }
    void chargeBattery() { //Pin C3

    }
    void dischargeBattery() { //Pin C4

    }
    void loadSwitch1() { //Pin C5

    }
    void loadSwitch2() { //Pin C6

    }
    void loadSwitch3() { //Pin C7

    }
private:
    bool load1Status, load2Status, load3Status;


}; //Third RJ45 Port C (2 to 7)

int main() {
    sei();                                              //Enable Global interrupts
    AnalogueInput analogueInput;                        //Starts the ADC up in the AI (analog input) constructor
    AnalogueOutput analogueOutput;                      //Starts the PWM up in the AO (analog output) constructor

    finalizePorts();

    testLight(1);

    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        //signOfLife();                                 //Blink LED every .5 sec to show sign of life

        testOutputPin('B', 0);
        uint16_t current = analogueInput.busbarCurrent();
        analogueOutput.setMainsCapacity(current/2.5f + .5f);

    }
}
