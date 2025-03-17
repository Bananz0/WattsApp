//
// Created by glenm on 2/9/2025.
//

#include "Loads.h"

Loads::Loads(DigitalOutput* loadSwitcher, DigitalInput* loadCaller) : output(loadSwitcher), input(loadCaller) {
     lastLoadCall[0] = false;
     lastLoadCall[1] = false;
     lastLoadCall[2] = false;

    currentLoadCapacity[0] = 1.2f;
    currentLoadCapacity[1] = 2.0f;
    currentLoadCapacity[2] = 0.8f;

    loadOverride[0] = false;
    loadOverride[1] = false;
    loadOverride[2] = false;

    currentLoadCall[0] = false;
    currentLoadCall[1] = false;
    currentLoadCall[2] = false;

    currentLoadStatus[0] = false;
    currentLoadStatus[1] = false;
    currentLoadStatus[2] = false;
}

Loads::~Loads()= default;

void Loads::checkLoadCallChanges() {
    // currentLoadCall[0] = input->readLoad1Call();
    // currentLoadCall[1] = input->readLoad2Call();
    // currentLoadCall[2] = input->readLoad3Call();
    for (int loadCount = 0; loadCount < 3; loadCount++) {
        currentLoadCall[loadCount] = DigitalInput::readLoadCall(loadCount);
    }
    //emergencyScreen = currentLoadCall[0] || currentLoadCall[1] || currentLoadCall[2];

    //Commented out currentLoadStatus updates from the loadcall
    //load 1
    //Jackson's code which prevents hysterisis actually. NIce implementation
    if (currentLoadCall[0] && !lastLoadCall[0]) {
        DigitalOutput::loadSwitch1(DigitalOutput::ON);  //Load 1 on
       // currentLoadStatus[0] = true;
    } else if (!currentLoadCall[0] && lastLoadCall[0]) {
        DigitalOutput::loadSwitch1(DigitalOutput::OFF); //OFF
        //currentLoadStatus[0] = false;
    }

    //load 2
    if (currentLoadCall[1] && !lastLoadCall[1]) {
        DigitalOutput::loadSwitch2(DigitalOutput::ON);  //ON
        //currentLoadStatus[1] = true;
    } else if (!currentLoadCall[1] && lastLoadCall[1]) {
        DigitalOutput::loadSwitch2(DigitalOutput::OFF); //OFF
        //currentLoadStatus[1] = false;
    }

    //load 3
    if (currentLoadCall[2] && !lastLoadCall[2]) {
        DigitalOutput::loadSwitch3(DigitalOutput::ON);  //ON
        //currentLoadStatus[2] = true;
    } else if (!currentLoadCall[2] && lastLoadCall[2]) {
        DigitalOutput::loadSwitch3(DigitalOutput::OFF); //OFF
        //currentLoadStatus[2] = false;
    }

    lastLoadCall[0] = currentLoadCall[0];
    lastLoadCall[1] = currentLoadCall[1];
    lastLoadCall[2] = currentLoadCall[2];
}

void Loads::turnLoadOn(uint8_t load) {
    switch (load) {
        case 1:
            DigitalOutput::loadSwitch1(DigitalOutput::ON);
            currentLoadStatus[0] = true;
        break;
        case 2:
            DigitalOutput::loadSwitch2(DigitalOutput::ON);
            currentLoadStatus[1] = true;
        break;
        case 3:
            DigitalOutput::loadSwitch3(DigitalOutput::ON);
            currentLoadStatus[2] = true;
        break;
        default:
            break;
    }
}

void Loads::turnLoadOff(uint8_t load) {
    switch (load) {
        case 1:
            DigitalOutput::loadSwitch1(DigitalOutput::OFF);
            currentLoadStatus[0] = false;
        break;
        case 2:
            DigitalOutput::loadSwitch2(DigitalOutput::OFF);
            currentLoadStatus[1] = false;
        break;
        case 3:
            DigitalOutput::loadSwitch3(DigitalOutput::OFF);
            currentLoadStatus[2] = false;
        break;
        default:
            break;
    }
}

void Loads::calculateLoadCapacity() {
    // if (currentLoadCall[0]) {
    //     currentTotalLoad += currentLoad1;
    // }
    // if (currentLoadCall[1]) {
    //     currentTotalLoad += currentLoad2;
    // }
    // if (currentLoadCall[2]) {
    //     currentTotalLoad += currentLoad3;
    // }
    totalLoadCapacity = 0;
    for (int i = 0; i < 3; i++) {
        totalLoadCapacity += currentLoadCapacity[i]*currentLoadStatus[i];
    }
}
