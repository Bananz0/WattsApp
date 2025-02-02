//
// Created by glenm on 2/2/2025.
//
#pragma once
#ifndef DIGITALOUTPUT_H
#define DIGITALOUTPUT_H

#include <avr/io.h>


class DigitalOutput {
public:
    DigitalOutput();
    void chargeBattery();
    void dischargeBattery();
    void loadSwitch1();
    void loadSwitch2();
    void loadSwitch3();
private:
    bool load1Status, load2Status, load3Status;
}; //Third RJ45 Port C (2 to 7)



#endif //DIGITALOUTPUT_H
