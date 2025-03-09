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

    bool lastLoad1Call, lastLoad2Call, lastLoad3Call;
    bool currentLoad1Call{}, currentLoad2Call{}, currentLoad3Call{};
    bool loadOverride1, loadOverride2, loadOverride3;
    bool loadOverride[3];
    bool currentLoad[3];
    bool currentLoadStatus[3];
    bool currentLoad1, currentLoad2, currentLoad3;
    int totalLoadCapacity{0};

    private:
    DigitalOutput* output;
    DigitalInput* input;
 };



#endif //LOADS_H
