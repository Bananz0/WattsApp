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

    int8_t batteryCapacity{-1};
    float totalAvailableCapacity{0};
    float totalAvailableCapacityMain{0};
    float loadDeficit{0};
    bool isBatteryCharging{false}; // to monitor battery charging status for sprite

    Sources(AnalogueInput *sourceHandler, AnalogueOutput *mainsHandler,DigitalOutput *batteryHandler);
    ~Sources();

    void requestMains(float mainsCapacity);
    void requestBattery(bool requestDischarge);
    void chargeBattery();

    void readTurbineCapacity();
    void readPvCapacity();
    void readBusbarCurrent();
    void readBusbarVoltage();

    void calculateTotalAvailableCapacity();
    void calculateTotalEnergyAndPower();

    int8_t dailyMainsChange = 0; //gets the mains change (+1 or -1)
    int8_t dailyBatteryChange = 0; //gets the battery change (+1 or -1)
};



#endif //SOURCES_H
