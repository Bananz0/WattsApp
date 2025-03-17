//
// Created by glenm on 2/9/2025.
//

#ifndef LOADS_H
#define LOADS_H

#include "DigitalOutput.h"
#include "DigitalInput.h"
#include <avr/io.h>

//Manage all the loads

class Loads {
  public:
    Loads(DigitalOutput* loadSwitcher, DigitalInput* loadCaller);
    ~Loads();
    void checkLoadCallChanges();
    void turnLoadOn(uint8_t load);
    void turnLoadOff(uint8_t load);

    void calculateLoadCapacity();

    bool loadOverride[3]{false};
    bool lastLoadCall[3]{false};

    bool currentLoadCall[3]{false};
    bool currentLoadStatus[3]{false};

    float currentLoadCapacity[3]{0.0f};
    float totalLoadCapacity{0};

    private:
    DigitalOutput* output;
    DigitalInput* input;
 };



#endif //LOADS_H
