//
// Created by glenm on 2/9/2025.
//

#include "Loads.h"

Loads::Loads(DigitalOutput* loadSwitcher, DigitalInput* loadCaller) : output(loadSwitcher), input(loadCaller) {
     lastLoad1Call = false;
     lastLoad2Call = false;
     lastLoad3Call = false;

    currentLoad[0] = 1.2f;
    currentLoad[1] = 2.0f;
    currentLoad[2] = 0.8f;

    loadOverride1 = false;
    loadOverride2 = false;
    loadOverride3 = false;

    loadOverride[0] = false;
    loadOverride[1] = false;
    loadOverride[2] = false;
}

Loads::~Loads()= default;

void Loads::checkLoadCallChanges() {


    currentLoad1Call = input->readLoad1Call();
    currentLoad2Call = input->readLoad2Call();
    currentLoad3Call = input->readLoad3Call();
    //emergencyScreen = currentLoad1Call || currentLoad2Call || currentLoad3Call;

    //load 1
    if (currentLoad1Call && !lastLoad1Call) {
        output->loadSwitch1(DigitalOutput::ON);  //Load 1 on
        currentLoadStatus[0] = true;
    } else if (!currentLoad1Call && lastLoad1Call) {
        output->loadSwitch1(DigitalOutput::OFF); //OFF
        currentLoadStatus[0] = false;
    }

    //load 2
    if (currentLoad2Call && !lastLoad2Call) {
        output->loadSwitch2(DigitalOutput::ON);  //ON
        currentLoadStatus[1] = true;
    } else if (!currentLoad2Call && lastLoad2Call) {
        output->loadSwitch2(DigitalOutput::OFF); //OFF
        currentLoadStatus[1] = false;
    }

    //load 3
    if (currentLoad3Call && !lastLoad3Call) {
        output->loadSwitch3(DigitalOutput::ON);  //ON
        currentLoadStatus[2] = true;
    } else if (!currentLoad3Call && lastLoad3Call) {
        output->loadSwitch3(DigitalOutput::OFF); //OFF
        currentLoadStatus[2] = false;
    }

    lastLoad1Call = currentLoad1Call;
    lastLoad2Call = currentLoad2Call;
    lastLoad3Call = currentLoad3Call;
}

void Loads::turnLoadOn(uint8_t load) {
    switch (load) {
        case 1:
            output->loadSwitch1(DigitalOutput::ON);
            currentLoadStatus[0] = true;
        break;
        case 2:
            output->loadSwitch2(DigitalOutput::ON);
            currentLoadStatus[1] = true;
        break;
        case 3:
            output->loadSwitch3(DigitalOutput::ON);
            currentLoadStatus[2] = true;
        break;
        default:
            break;
    }
}

void Loads::turnLoadOff(uint8_t load) {
    switch (load) {
        case 1:
            output->loadSwitch1(DigitalOutput::OFF);
            currentLoadStatus[0] = false;
        break;
        case 2:
            output->loadSwitch2(DigitalOutput::OFF);
            currentLoadStatus[1] = false;
        break;
        case 3:
            output->loadSwitch3(DigitalOutput::OFF);
            currentLoadStatus[2] = false;
        break;
        default:
            break;
    }
}

void Loads::calculateLoadCapacity() {
    // if (currentLoad1Call) {
    //     currentTotalLoad += currentLoad1;
    // }
    // if (currentLoad2Call) {
    //     currentTotalLoad += currentLoad2;
    // }
    // if (currentLoad3Call) {
    //     currentTotalLoad += currentLoad3;
    // }
    for (int i = 0; i < 2; i++) {
        totalLoadCapacity += currentLoad[i]*currentLoadStatus[i];
    }
}
