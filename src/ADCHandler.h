//
// Created by glenm on 2/2/2025.
//

#pragma once

#ifndef ADCHANDLER_H
#define ADCHANDLER_H

#include <stdio.h>
#include <avr/io.h>
#include "globalVariables.h"

class ADCHandler {
public:
    ADCHandler();
    static void initializeADC();
    static void setADCChannel(const uint8_t channel);
    static void readADC();
    static void getVoltage(const uint8_t channel);
private:

};

#endif //ADCHANDLER_H
