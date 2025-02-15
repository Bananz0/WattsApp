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
    mainsHandler->setMainsCapacity(mainsCapacityIn);
    mainsCapacity += mainsCapacityIn;
}

void Sources::requestBattery(uint8_t batteryCapacityOut) {
    batteryHandler->dischargeBattery();
    if(batteryCapacity >= 1) {
        batteryCapacity-=1;
    }
    isBatteryCharging = false;
}

void Sources::chargeBattery() {
    batteryHandler->chargeBattery();
    if(batteryCapacity < 24) {
        batteryCapacity+=1;
        isBatteryCharging = true;
    } else if(batteryCapacity >= 24) {
        isBatteryCharging = false;
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
    totalAvailableCapacity = totalRenewableCapacity + batteryCapacity + mainsCapacity;
}

void Sources::calculateTotalEnergyAndPower() {
    busbarPower = busbarCurrent * busbarVoltage;
    totalRenewableCapacity = pvCapacity + windTurbineCapacity;
}
