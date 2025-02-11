//
// Created by glenm on 2/2/2025.
//
#pragma once
#ifndef DIGITALOUTPUT_H
#define DIGITALOUTPUT_H

#include <avr/io.h>


class DigitalOutput {
public:
    typedef enum {OFF, ON}status;
    DigitalOutput();
    void chargeBattery();
    void dischargeBattery();
    void loadSwitch1(status Status);
    void loadSwitch2(status Status);
    void loadSwitch3(status Status);
private:
    bool load1Status, load2Status, load3Status;
}; //Third RJ45 Port C (2 - 7)



#endif //DIGITALOUTPUT_H
