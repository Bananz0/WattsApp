//
// Created by glenm on 2/2/2025.
//

#include "DigitalOutput.h"

DigitalOutput::DigitalOutput() : load1Status(false), load2Status(false), load3Status(false) {
    //Explicitly set Pin C3 - C7 as outputs
    DDRC |= _BV(3)
          | _BV(4)
          | _BV(5)
          | _BV(6)
          | _BV(7);
}
void DigitalOutput::chargeBattery() { //Pin C3
    PORTC |= (1 << PORTC3); //Set PC3 to High
}
void DigitalOutput::dischargeBattery() { //Pin C4
    PORTC &= ~(1 << PORTC4); //Set PC4 to Low
}
void DigitalOutput::loadSwitch1(status Status) { //Pin C5
    switch (Status) {
        case ON:
            PORTC |= (1 << PORTC5); //Set PC3 to High
        break;
        case OFF:
            PORTC &= ~(1 << PORTC5); //Set PC3 to LOW
        break;
        default:
            PORTC ^= (1 << PORTC5); //Toggle switch 1
    }
}
void DigitalOutput::loadSwitch2(status Status) { //Pin C6
    switch (Status) {
        case ON:
            PORTC |= (1 << PORTC6); //Set PC3 to High
        break;
        case OFF:
            PORTC &= ~(1 << PORTC6); //Set PC3 to LOW
        break;
        default:
            PORTC ^= (1 << PORTC6); //Toggle switch 1
    }
}
void DigitalOutput::loadSwitch3(status Status) { //Pin C7
    switch (Status) {
        case ON:
            PORTC |= (1 << PORTC7); //Set PC3 to High
        break;
        case OFF:
            PORTC &= ~(1 << PORTC7); //Set PC3 to LOW
        break;
        default:
            PORTC ^= (1 << PORTC7); //Toggle switch 1
    }
}