//
// Created by glenm on 2/2/2025.
//

#pragma once

#include <stdio.h>
#include <avr/io.h>
#include "globalVariables.h"

#ifndef PWMHANDLER_H
#define PWMHANDLER_H

class PWMHandler {
public:
    PWMHandler();
    static void initializePWM();
    static void setOutputVoltage(uint8_t voltage);
};

#endif //PWMHANDLER_H
