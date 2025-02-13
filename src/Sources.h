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

    float windTurbineCapacity{0},
            pvCapacity{0},
            totalRenewableCapacity{0},
            busbarVoltage{0},
            busbarCurrent{0},
            busbarPower{0},
            averagePower{0},
            totalEnergy{0},
            mainsCapacity{0};

    uint8_t batteryCapacity{0};
    float totalAvailableCapacity{0};
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
