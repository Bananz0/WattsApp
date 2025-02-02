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
void DigitalOutput::loadSwitch1() { //Pin C5
    PORTC ^= (1 << PORTC5); //Toggle switch 1
}
void DigitalOutput::loadSwitch2() { //Pin C6
    PORTC ^= (1 << PORTC6); //Toggle switch 2
}
void DigitalOutput::loadSwitch3() { //Pin C7
    PORTC ^= (1 << PORTC7); //Toggle switch 3
}