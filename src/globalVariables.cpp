//
// Created by glenm on 2/2/2025.
//

#include "globalVariables.h"

#define TARGET_TIME_MS 100
#define PRESCALER 64

volatile float Vref = 3.3;                       //Reference Voltage, 3.3v
volatile float ADCVoltage;              //Global ADC voltage variable and idk if this is good
volatile bool ADCConversionFlag = false;   //Global ADC Conversion Complete Flag

volatile EnergyStats energyStats;
volatile LoadStats loadStats;

volatile uint32_t Counter = 0;