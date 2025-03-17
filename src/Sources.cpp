//
// Created by glenm on 2/9/2025.
//

#include "Sources.h"

Sources::Sources(AnalogueInput *sourceHandler, AnalogueOutput *mainsHandler, DigitalOutput *batteryHandler) {
    this->sourceHandler = sourceHandler;
    this->mainsHandler = mainsHandler;
    this->batteryHandler = batteryHandler;
    windTurbineCapacity=0;
    pvCapacity=0;
    totalRenewableCapacity=0;
    busbarVoltage=0;
    busbarCurrent=0;
    busbarPower=0;
    averagePower=0;
    totalEnergy=0;
    mainsCapacity=0;
}

Sources::~Sources() = default;

void Sources::requestMains(float mainsCapacityIn) {
    // Clamp the request between 0 and 2
    if (mainsCapacityIn < 0) mainsCapacityIn = 0;
    else if (mainsCapacityIn > 2) mainsCapacityIn = 2;

    // Apply the request immediately to mainsCapacity
    mainsCapacity = mainsCapacityIn;

    // Update the analogue output immediately
    mainsHandler->setMainsCapacity(mainsCapacity);
}
//
// void requestMains(float request) {
//     // Clamp the request between 0 and 2
//     if (request < 0) request = 0;
//     else if (request > 2) request = 2;
//
//     // Apply the request immediately to mainsCapacity
//     mainsCapacity = request;
//
//     // Track hourly change (optional, if needed for logging)
//     hourMainsChange += request;
//
//     // Update the analogue output immediately
//     analogueOutput.setMainsCapacity(mainsCapacity);
// }

void Sources::requestBattery(bool requestDischarge) {
    if (batteryCapacity == 0) {
        requestDischarge = false;
        DigitalOutput::dischargeBattery(false);
    }
    // Only allow a discharge request if no other battery operation is pending
    if (requestDischarge && hourBatteryChange == 0 && batteryCapacity > 0) {
        hourBatteryChange = -1; // Request discharge
    } else if (!requestDischarge) {
        DigitalOutput::dischargeBattery(false);
    }
    if (requestDischarge) {
        DigitalOutput::dischargeBattery(true);
        isBatteryCharging = false;
    }
}

void Sources::chargeBattery(bool requestCharge) {
    // Only allow a charge request if no other battery operation is pending
    if (requestCharge) {
        DigitalOutput::chargeBattery(true);
        if (hourBatteryChange == 0 && batteryCapacity < 24) {
            hourBatteryChange = 1; // Request charge
            isBatteryCharging = true;
        }
    } else {
        DigitalOutput::chargeBattery(false);
    }
}

void Sources::readTurbineCapacity() {
    windTurbineCapacity = sourceHandler->turbineCurrentCapacity();
}

void Sources::readPvCapacity() {
    pvCapacity = sourceHandler->pvCurrentCapacity();
}

void Sources::readBusbarCurrent() {
    busbarCurrent = sourceHandler->busbarCurrent();
}

void Sources::readBusbarVoltage() {
    busbarVoltage = sourceHandler->busbarVoltage();
}

void Sources::calculateTotalAvailableCapacity() {
    totalRenewableCapacity = pvCapacity + windTurbineCapacity;
    totalAvailableCapacity = totalRenewableCapacity + batteryCapacity;
    totalAvailableCapacityMain = totalAvailableCapacity + mainsCapacity;
}

void Sources::calculateTotalEnergyAndPower() {
    busbarPower = busbarCurrent * busbarVoltage;
}
