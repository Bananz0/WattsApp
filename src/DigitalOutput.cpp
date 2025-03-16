//
// Created by glenm on 2/2/2025.
//

#include "DigitalOutput.h"

DigitalOutput::DigitalOutput() : load1Status(false), load2Status(false), load3Status(false) {
    initPorts();

}
void DigitalOutput::initPorts() {
    //Explicitly set Pin D2 - D6 as outputs
    DDRD |= _BV(2)
          | _BV(3)
          | _BV(4)
          | _BV(5)
          | _BV(6);
    PORTD &= ~(1 << PORTD2);
    PORTD &= ~(1 << PORTD3);
    PORTD &= ~(1 << PORTD4);
    PORTD &= ~(1 << PORTD5);
    PORTD &= ~(1 << PORTD6);
}
void DigitalOutput::chargeBattery(bool endis) { //Pin C3 //enable disable -endis
    endis? PORTD |= (1 << PORTD2) : PORTD &= ~(1 << PORTD2);
    // PORTD |= (1 << PORTD2); //Set PC3 to High incicating to charge the battery
    // PORTD &= ~(1 << PORTD3); //Indicate to stop discharging battery.

}
void DigitalOutput::dischargeBattery(bool endis ) { //Pin C4 //enable disable
    endis? PORTD |= (1 << PORTD3) : PORTD &= ~(1 << PORTD3);
    // PORTD |= (1 << PORTD3); //Set PC4 to High
    // PORTD &= ~(1 << PORTD2); //Indicate to stop charging battery.
}
void DigitalOutput::loadSwitch1(const status Status) { //Pin C5
    switch (Status) {
        case ON:
            PORTD |= (1 << PORTD4); //Set PC3 to High
        break;
        case OFF:
            PORTD &= ~(1 << PORTD4); //Set PC3 to LOW
        break;
        default:
            PORTD ^= (1 << PORTD4); //Toggle switch 1
        break;
    }
}
void DigitalOutput::loadSwitch2(const status Status) { //Pin C6
    switch (Status) {
        case ON:
            PORTD |= (1 << PORTD5); //Set PC3 to High
        break;
        case OFF:
            PORTD &= ~(1 << PORTD5); //Set PC3 to LOW
        break;
        default:
            PORTD ^= (1 << PORTD5); //Toggle switch 1
        break;
    }
}
void DigitalOutput::loadSwitch3(const status Status) { //Pin C7
    switch (Status) {
        case ON:
            PORTD |= (1 << PORTD6); //Set PC3 to High
        break;
        case OFF:
            PORTD &= ~(1 << PORTD6); //Set PC3 to LOW
        break;
        default:
            PORTD ^= (1 << PORTD6); //Toggle switch 1
        break;
    }
}