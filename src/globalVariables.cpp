//
// Created by glenm on 2/2/2025.
//

#include "globalVariables.h"

float Vref = 3.3;                       //Reference Voltage, 3.3v
volatile float ADCVoltage;              //Global ADC voltage variable and idk if this is good
volatile float ADCConversionFlag = 0;   //Global ADC Conversion Complete Flag
volatile uint32_t Counter = 0;          //Global Counter variable