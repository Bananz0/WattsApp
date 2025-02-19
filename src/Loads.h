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

    float currentLoad1, currentLoad2, currentLoad3;
    float totalLoadCapacity{};

    private:
    DigitalOutput* output;
    DigitalInput* input;
 };



#endif //LOADS_H
