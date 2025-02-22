//
// Created by glenm on 2/2/2025.
//

#include "globalVariables.h"

#define TARGET_TIME_MS 100
#define PRESCALER 64

volatile float Vref = 3.3;                       //Reference Voltage, 3.3v
volatile float ADCVoltage;              //Global ADC voltage variable and idk if this is good
volatile bool ADCConversionFlag = false;   //Global ADC Conversion Complete Flag
volatile bool LoadChangeFlag = false;      //Global Load Change Flag
volatile uint64_t ADCraw;

// volatile EnergyStats energyStats;
// volatile LoadStats loadStats;
// Moved to Loads and Sources classes

volatile uint32_t Counter = 0;
volatile uint32_t utc = 0;

volatile tm* timeUTC;

volatile Screen screenPage, nextScreenPage, prevScreenPage;

volatile bool emergencyScreen = false;
volatile char emergencyMessage[] = "Emergency\nNot\nSpecified";
volatile char uartMessage[] = "";

volatile uint8_t dayCount = 0, remainingDays = 24;