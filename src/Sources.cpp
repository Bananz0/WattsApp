//
// Created by glenm on 2/9/2025.
//

#include "Sources.h"

Sources::Sources(AnalogueInput *sourceHandler, AnalogueOutput *mainsHandler, DigitalOutput *batteryHandler) {
    this->sourceHandler = sourceHandler;
    this->mainsHandler = mainsHandler;
    this->batteryHandler = batteryHandler;
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
    }
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
    totalAvailableCapacity = totalRenewableCapacity + batteryCapacity + mainsCapacity;
}

void Sources::calculateTotalEnergyandPower() {
    busbarPower = busbarCurrent * busbarVoltage;
    totalRenewableCapacity = pvCapacity + windTurbineCapacity;
}
