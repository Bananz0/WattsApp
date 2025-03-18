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

    int8_t batteryCapacity{0};
    int8_t hourMainsChange = {0};
    int8_t hourBatteryChange = {0};
    float totalAvailableCapacity{0};
    float totalAvailableCapacityMain{0};
    float loadDeficit{0};
    bool isBatteryCharging{false}; // to monitor battery charging status for sprite

    Sources(AnalogueInput *sourceHandler, AnalogueOutput *mainsHandler, DigitalOutput *batteryHandler);

    ~Sources();

    void requestMains(float mainsCapacityIn);

    void requestBattery(bool requestDischarge);

    void chargeBattery(bool requestCharge);

    void readTurbineCapacity();

    void readPvCapacity();

    void readBusbarCurrent();

    void readBusbarVoltage();

    void calculateTotalAvailableCapacity();

    void calculateTotalEnergyAndPower();
};


#endif //SOURCES_H
