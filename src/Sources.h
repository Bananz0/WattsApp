//
// Created by glenm on 2/9/2025.
//

#ifndef SOURCES_H
#define SOURCES_H

#include "AnalogueInput.h"
#include "AnalogueOutput.h"
#include "DigitalOutput.h"

//Turbine, PV, Mains, Battery

class Sources {
public:
    AnalogueInput *sourceHandler;
    AnalogueOutput *mainsHandler;
    DigitalOutput *batteryHandler;

    float windTurbineCapacity{},
            pvCapacity{},
            totalRenewablePower{},
            busbarVoltage{},
            busbarCurrent{},
            busbarPower{},
            averagePower{},
            totalEnergy{};
    uint8_t batteryCapacity{};

    Sources(AnalogueInput *sourceHandler, AnalogueOutput *mainsHandler,DigitalOutput *batteryHandler);
    ~Sources();

    void requestMains(float mainsCapacity);
    void requestBattery(uint8_t batteryCapacity);
    void chargeBattery();

    void readTurbineCapacity();
    void readPvCapacity();
    void readBusbarCurrent();
    void readBusbarVoltage();

    void calculateTotalEnergyandPower();








};



#endif //SOURCES_H
