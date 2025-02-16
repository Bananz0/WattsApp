//
// Created by glenm on 2/2/2025.
//

#include "DigitalOutput.h"

DigitalOutput::DigitalOutput() : load1Status(false), load2Status(false), load3Status(false) {
    //Explicitly set Pin D2 - D6 as outputs
    DDRD |= _BV(2)
          | _BV(3)
          | _BV(4)
          | _BV(5)
          | _BV(6);
}
void DigitalOutput::chargeBattery() { //Pin C3
    PORTD |= (1 << PORTD2); //Set PC3 to High
}
void DigitalOutput::dischargeBattery() { //Pin C4
    PORTD |= (1 << PORTD3); //Set PC4 to Low
}
void DigitalOutput::loadSwitch1(status Status) { //Pin C5
    switch (Status) {
        case ON:
            PORTD |= (1 << PORTD4); //Set PC3 to High
        break;
        case OFF:
            PORTD &= ~(1 << PORTD4); //Set PC3 to LOW
        break;
        default:
            PORTD ^= (1 << PORTD4); //Toggle switch 1
    }
}
void DigitalOutput::loadSwitch2(status Status) { //Pin C6
    switch (Status) {
        case ON:
            PORTD |= (1 << PORTD5); //Set PC3 to High
        break;
        case OFF:
            PORTD &= ~(1 << PORTD5); //Set PC3 to LOW
        break;
        default:
            PORTD ^= (1 << PORTD5); //Toggle switch 1
    }
}
void DigitalOutput::loadSwitch3(status Status) { //Pin C7
    switch (Status) {
        case ON:
            PORTD |= (1 << PORTD6); //Set PC3 to High
        break;
        case OFF:
            PORTD &= ~(1 << PORTD6); //Set PC3 to LOW
        break;
        default:
            PORTD ^= (1 << PORTD6); //Toggle switch 1
    }
}