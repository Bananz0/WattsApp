//
// Created by glenm on 2/9/2025.
//

#include "Loads.h"

Loads::Loads(DigitalOutput* loadSwitcher, DigitalInput* loadCaller) : output(loadSwitcher), input(loadCaller) {
     lastLoad1Call = false;
     lastLoad2Call = false;
     lastLoad3Call = false;
}

Loads::~Loads()= default;

void Loads::checkLoadCallChanges() {
    currentLoad1Call = input->readLoad1Call();
    currentLoad2Call = input->readLoad2Call();
    currentLoad3Call = input->readLoad3Call();
    emergencyScreen = currentLoad1Call || currentLoad2Call || currentLoad3Call;

    //load 1
    if (currentLoad1Call && !lastLoad1Call) {
        output->loadSwitch1(DigitalOutput::ON);  //Load 1 on
    } else if (!currentLoad1Call && lastLoad1Call) {
        output->loadSwitch1(DigitalOutput::OFF); //OFF
    }

    //load 2
    if (currentLoad2Call && !lastLoad2Call) {
        output->loadSwitch2(DigitalOutput::ON);  //ON
    } else if (!currentLoad2Call && lastLoad2Call) {
        output->loadSwitch2(DigitalOutput::OFF); //OFF
    }

    //load 3
    if (currentLoad3Call && !lastLoad3Call) {
        output->loadSwitch3(DigitalOutput::ON);  //ON
    } else if (!currentLoad3Call && lastLoad3Call) {
        output->loadSwitch3(DigitalOutput::OFF); //OFF
    }

    lastLoad1Call = currentLoad1Call;
    lastLoad2Call = currentLoad2Call;
    lastLoad3Call = currentLoad3Call;
}

void Loads::turnLoadOn(uint8_t load) {
    switch (load) {
        case 1:
            output->loadSwitch1(DigitalOutput::ON);
        break;
        case 2:
            output->loadSwitch2(DigitalOutput::ON);
        break;
        case 3:
            output->loadSwitch3(DigitalOutput::ON);
        break;
        default:
            break;
    }
}

void Loads::turnLoadOff(uint8_t load) {
    switch (load) {
        case 1:
            output->loadSwitch1(DigitalOutput::OFF);
        break;
        case 2:
            output->loadSwitch2(DigitalOutput::OFF);
        break;
        case 3:
            output->loadSwitch3(DigitalOutput::OFF);
        break;
        default:
            break;
    }
}
