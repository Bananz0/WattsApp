//
// Created by glenm on 2/2/2025.
//
#pragma once
#ifndef DIGITALINPUT_H
#define DIGITALINPUT_H
#include "globalVariables.h"
#include <avr/io.h>

class DigitalInput {
public:
    DigitalInput();
    static bool readLoadCall(uint8_t load);
private:
    static bool readLoad1Call();
    static bool readLoad2Call();
    static bool readLoad3Call();
};  //Third RJ45 Port C (0 to 2)=


#endif //DIGITALINPUT_H
