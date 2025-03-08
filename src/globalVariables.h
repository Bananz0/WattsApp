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
extern volatile bool ADCConversionFlag, LoadChangeFlag;
extern volatile uint8_t dayCount , remainingDays;
extern volatile uint64_t ADCraw;

// extern volatile float windTurbineCapacity;
// extern volatile float pvCapacity;
// extern volatile float totalRenewablePower;
// extern volatile float busbarVoltage;
// extern volatile float busbarCurrent;
// extern volatile float averagePower;
// extern volatile float totalEnergy;

// struct EnergyStats {
//     volatile float windTurbineCapacity;
//     volatile float pvCapacity;
//     volatile float totalRenewablePower;
//     volatile float busbarVoltage;
//     volatile float busbarCurrent;
//     volatile float busbarPower;
//     volatile float averagePower;
//     volatile float totalEnergy;
// };
// struct LoadStats {
//     volatile float loadCapacity;
// };
//
// extern volatile EnergyStats energyStats;
// extern volatile LoadStats loadStats;

//Moved to Loads and Sources classes


extern volatile uint32_t Counter;
extern volatile uint32_t utc;

extern volatile tm* timeUTC;

typedef enum  {BUSBAR_SCREEN ,PV_SCREEN , TURBINE_SCREEN , BATTERY_SCREEN
              ,LOADS_SCREEN, DAY_SCREEN, ERROR_SCREEN,SEC_REV_SCREEN,UART_SCREEN,SCREEN_COUNT} Screen ;

extern volatile Screen screenPage, nextScreenPage,prevScreenPage;

extern volatile bool emergencyScreen;
extern volatile char emergencyMessage[100];
extern volatile char uartMessage[40];

extern volatile bool batteryDecrease, batteryIncrease;


extern  time_t startNonBlockingWait();

extern bool isWaitComplete(time_t startTime, time_t durationSeconds);

#endif //GLOBALVARIABLES_H
