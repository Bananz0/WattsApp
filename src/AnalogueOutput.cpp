//
// Created by glenm on 2/2/2025.
//

#include "AnalogueOutput.h"

AnalogueOutput::AnalogueOutput() {
    PWMHandler::initializePWM();
}
void AnalogueOutput::setMainsCapacity(float mainsCapacity) { //Set Mains Capacity using PWM from 0 to 10v
    PWMHandler::setOutputVoltage(mainsCapacity);
}