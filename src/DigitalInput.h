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
    bool readLoad1Call();
    bool readLoad2Call();
    bool readLoad3Call();
    void checkLoadCallChanges();
};  //Third RJ45 Port C (0 to 2)=


#endif //DIGITALINPUT_H
