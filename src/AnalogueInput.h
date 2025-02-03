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
    static uint16_t turbineCurrentCapacity();

    static uint16_t pvCurrentCapacity();

    static uint16_t busbarCurrent();

    static uint16_t busbarVoltage();

private:
    ADCHandler aDCHandler;
}; //First RJ45 Port A

#endif //ANALOGUEINPUT_H
