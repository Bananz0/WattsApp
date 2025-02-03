//
// Created by glenm on 2/2/2025.
//

#include "AnalogueInput.h"

AnalogueInput::AnalogueInput() {
    ADCHandler::initializeADC();
}
uint16_t AnalogueInput::turbineCurrentCapacity() { //Read and return turbine capacity current
    //Read Pin A2 using ADC
    ADCHandler::getVoltage(2);
    return ADCVoltage; //Amps 1:1 mapping
}

uint16_t AnalogueInput::pvCurrentCapacity() { //Read and return the PV Capacity current
    //Read Pin A3 using ADC
    ADCHandler::getVoltage(3);
    return ADCVoltage; //Amps 1:1 mapping
}

uint16_t AnalogueInput::busbarCurrent() {
    //Read Pin A1 using ADC
    ADCHandler::getVoltage(1);
    return ADCVoltage; //Amps 1:1 mapping
}

uint16_t AnalogueInput::busbarVoltage() {
    //Read Pin A0 using ADC (scaled down from 10v)
    ADCHandler::getVoltage(0);
    return 100 * ADCVoltage; //Volts
}