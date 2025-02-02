//
// Created by glenm on 2/2/2025.
//

#pragma once

#ifndef ADCHANDLER_H
#define ADCHANDLER_H

#include <stdio.h>
#include <avr/io.h>

class ADCHandler {
public:
    ADCHandler() {}
    void initializeADC() {}
    void setADCChannel(const uint8_t channel) {}
    void readADC() {}
    void getVoltage(const uint8_t channel) {}
    uint8_t ADCConversionFlag;
private:

};

#endif //ADCHANDLER_H
