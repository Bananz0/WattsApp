//
// Created by glenm on 2/2/2025.
//

#include "AnalogueInput.h"

AnalogueInput::AnalogueInput() {
    aDCHandler.initializeADC();
}
float AnalogueInput::turbineCurrentCapacity() { //Read and return turbine capacity current
    //Read Pin A2 using ADC
    aDCHandler.getVoltage(3);
    return ADCVoltage; //Amps 1:1 mapping
}

float AnalogueInput::pvCurrentCapacity() { //Read and return the PV Capacity current
    //Read Pin A3 using ADC
    aDCHandler.getVoltage(2);
    return ADCVoltage; //Amps 1:1 mapping
}

float AnalogueInput::busbarCurrent() {
    //Read Pin A1 using ADC
    aDCHandler.getVoltage(0);
    return ADCVoltage; //Amps 1:1 mapping
}

float AnalogueInput::busbarVoltage() {
    //Read Pin A0 using ADC (scaled down from 10v)
    aDCHandler.getVoltage(1);
    return 100 * ADCVoltage; //Volts
}