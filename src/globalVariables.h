//
// Created by glenm on 2/2/2025.
//

#pragma once
#include <stdio.h>
#include <time.h>
#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

extern volatile float Vref;                            //Reference Voltage, 3.3v
extern volatile float ADCVoltage;             //Global - extern - ADC voltage variable and idk if this is good
extern volatile bool ADCConversionFlag;

// extern volatile float windTurbineCapacity;
// extern volatile float pvCapacity;
// extern volatile float totalRenewablePower;
// extern volatile float busbarVoltage;
// extern volatile float busbarCurrent;
// extern volatile float averagePower;
// extern volatile float totalEnergy;

struct EnergyStats {
    volatile float windTurbineCapacity;
    volatile float pvCapacity;
    volatile float totalRenewablePower;
    volatile float busbarVoltage;
    volatile float busbarCurrent;
    volatile float busbarPower;
    volatile float averagePower;
    volatile float totalEnergy;
    volatile float batteryCapacity;
};
struct LoadStats {

};

extern volatile EnergyStats energyStats;
extern volatile LoadStats loadStats;

extern volatile uint32_t Counter;
extern volatile uint32_t utc;

extern volatile tm* timeUTC;

#endif //GLOBALVARIABLES_H
