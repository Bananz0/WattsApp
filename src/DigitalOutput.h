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
    static void initPorts();
    static void chargeBattery(bool endis);
    static void dischargeBattery(bool endis);
    static void loadSwitch1(status Status);
    static void loadSwitch2(status Status);
    static void loadSwitch3(status Status);
private:
    bool load1Status, load2Status, load3Status;
}; //Third RJ45 Port C (2 - 7)



#endif //DIGITALOUTPUT_H
