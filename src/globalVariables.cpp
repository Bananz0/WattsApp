//
// Created by glenm on 2/2/2025.
//

#include "globalVariables.h"

uint8_t mVref = 3300;                        //Reference Voltage, 3.3v
volatile uint16_t ADCVoltage;                //Global ADC voltage variable and idk if this is good
volatile bool ADCConversionFlag = false;     //Global ADC Conversion Complete Flag
volatile uint32_t Counter = 0;               //Global Counter variable