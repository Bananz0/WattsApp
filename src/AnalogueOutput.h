//
// Created by glenm on 2/2/2025.
//
#pragma once
#ifndef ANALOGUEOUTPUT_H
#define ANALOGUEOUTPUT_H
#include "PWMHandler.h"

class AnalogueOutput {
public:
    AnalogueOutput();
    static void setMainsCapacity(float mainsCapacity);
private:
    PWMHandler pWMHandler;

};//Second RJ45 Port D

#endif //ANALOGUEOUTPUT_H
