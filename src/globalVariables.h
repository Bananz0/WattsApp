//
// Created by glenm on 2/2/2025.
//

#pragma once
#include <stdio.h>
#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

extern uint16_t Vref;                            //Reference Voltage, 3.3v
extern volatile uint16_t ADCVoltage;             //Global - extern - ADC voltage variable and idk if this is good
extern volatile bool ADCConversionFlag;


#endif //GLOBALVARIABLES_H
