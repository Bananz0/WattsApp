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

    float windTurbineCapacity,
            pvCapacity,
            totalRenewableCapacity,
            busbarVoltage,
            busbarCurrent,
            busbarPower,
            averagePower,
            totalEnergy,
            mainsCapacity;

    uint8_t batteryCapacity{24};
    float totalAvailableCapacity{0};
    float totalAvailableCapacityMain{0};
    float loadDeficit{0};
    bool isBatteryCharging{false}; // to monitor battery charging status for sprite

    Sources(AnalogueInput *sourceHandler, AnalogueOutput *mainsHandler,DigitalOutput *batteryHandler);
    ~Sources();

    void requestMains(float mainsCapacity);
    void requestBattery(uint8_t batteryCapacityOut);
    void chargeBattery();

    void readTurbineCapacity();
    void readPvCapacity();
    void readBusbarCurrent();
    void readBusbarVoltage();

    void calculateTotalAvailableCapacity();
    void calculateTotalEnergyAndPower();
};



#endif //SOURCES_H
