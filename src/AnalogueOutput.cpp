//
// Created by glenm on 2/2/2025.
//

#define SQRT2 1.414f
#include "AnalogueOutput.h"
#include <string.h>

AnalogueOutput::AnalogueOutput(): scaledMainsCapacity(0) {
    pWMHandler.initializePWM();
}

void AnalogueOutput::setMainsCapacity(float mainsCapacity) { //Set Mains Capacity using PWM from 0 to 10v
    // We need to clamp the output from 0-10v and scale it down to 3.3v
    //Voltage Clamp
    if (mainsCapacity < 0.0f) {
        mainsCapacity = 0.0f;
    } else {
        if (mainsCapacity > 2.0f) {
            mainsCapacity = 2.0f;
            emergencyScreen = true;
            strcpy(const_cast<char *>(emergencyMessage), "Mains\nCapacity\n> 2");
            // memcpy(const_cast<char *>(emergencyMessage), "Mains\nCapacity\n> 10", sizeof("Mains\nCapacity\n> 10"));
        }
    }

    //Voltage Scale - 0-2A scaled to 0-2vrms
    scaledMainsCapacity = mainsCapacity * SQRT2;

    pWMHandler.setOutputVoltage(scaledMainsCapacity);
}
