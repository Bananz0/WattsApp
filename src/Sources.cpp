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
    if (mainsCapacityIn > 0 && dailyMainsChange == 0) {
        dailyMainsChange = 1; //Request increase
    } else if (mainsCapacityIn < 0 && dailyMainsChange == 0) {
        dailyMainsChange = -1; //Request decrease
    }
}

void Sources::requestBattery(bool requestDischarge) {
    // Only allow a discharge request if no other battery operation is pending
    if (requestDischarge && dailyBatteryChange == 0 && batteryCapacity > 0) {
        dailyBatteryChange = -1; // Request discharge
    }
    batteryHandler->dischargeBattery();
}

void Sources::chargeBattery() {
    // Only allow a charge request if no other battery operation is pending
    if (dailyBatteryChange == 0 && batteryCapacity < 24) {
        dailyBatteryChange = 1; // Request charge
    }
    batteryHandler->chargeBattery();

    isBatteryCharging = true;
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
