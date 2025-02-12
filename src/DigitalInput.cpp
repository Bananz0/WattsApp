//
// Created by glenm on 2/2/2025.
//

#include "DigitalInput.h"

DigitalInput::DigitalInput() {
    //Explicitly set PIN A5, A6 and A7 as inputs.
    DDRC &= ~(_BV(5) | _BV(6) | _BV(7));
    //Enable Pull Up resistors
    PORTC |= (_BV(5) | _BV(6) | _BV(7));
    //Enable Pin Change Registers
    PCICR |= _BV(PCIE0); //Pin change regiser for Port A
    //Enable specific pin change mask for the Pins A5-A7
    PCMSK0 |= _BV(PCINT5)|_BV(PCINT6)| _BV(PCINT7);
}
bool DigitalInput::readLoad1Call() { //Pin A5 Call for Load 1 (1 - on 0 - off)
    return (PINA & (1 << PINA5));
}
bool DigitalInput::readLoad2Call() { //Pin A6 Call for Load 2 (1 - on 0 - off)
    return (PINA & (1 << PINA6));
}
bool DigitalInput::readLoad3Call() { //Pin A7 Call for Load 3 (1 - on 0 - off)
    return (PINA & (1 << PINA7));
}