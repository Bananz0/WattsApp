//
// Created by glenm on 2/2/2025.
//
#pragma once
#ifndef ANALOGUEINPUT_H
#define ANALOGUEINPUT_H
#include "globalVariables.h"
#include "ADCHandler.h"

class AnalogueInput {
public:
    AnalogueInput();
    float turbineCurrentCapacity();

    float pvCurrentCapacity();

    float busbarCurrent();

    float busbarVoltage();

private:
    ADCHandler aDCHandler;
}; //First RJ45 Port A

#endif //ANALOGUEINPUT_H
