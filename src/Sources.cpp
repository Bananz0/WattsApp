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

void Sources::requestMains(float mainsCapacity) {
    mainsHandler->setMainsCapacity(mainsCapacity);
}

void Sources::requestBattery(uint8_t batteryCapacity) {
    //TODO: Handbook on how to handle time in this thing
    batteryHandler->chargeBattery();
}

void Sources::chargeBattery() {
    batteryHandler->chargeBattery();
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

void Sources::calculateTotalEnergyandPower() {
    busbarPower = busbarCurrent * busbarVoltage;
    totalRenewablePower = pvCapacity + windTurbineCapacity;
}
