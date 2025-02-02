//
// Created by glenm on 2/2/2025.
//

#include "AnalogueOutput.h"

AnalogueOutput::AnalogueOutput() {
    pWMHandler.initializePWM();
}
void AnalogueOutput::setMainsCapacity(uint16_t mainsCapacity) { //Set Mains Capacity using PWM from 0 to 10v
    pWMHandler.setOutputVoltage(mainsCapacity);
}