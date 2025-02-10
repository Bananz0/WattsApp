//
// Created by glenm on 2/2/2025.
//

#include "DigitalInput.h"

DigitalInput::DigitalInput() {
    //Explicitly set PIN C0, C1 and C2 as inputs.
    DDRC &= ~(_BV(0) | _BV(1) | _BV(2));
    //Enable Pull Up resistors
    PORTC |= (_BV(0) | _BV(1) | _BV(2));
    //Enable Pin Change Registers
    PCICR |= _BV(PCIE2); //Pin change regiser for PortC
    //Enable specific pin change mask for the port c
    PCMSK2 |= _BV(PCINT16)|_BV(PCINT17)| _BV(PCINT18);

    //Set the load switches PB1/PB2/PB3 as output
    DDRB |= (1 << PB3) | (1 << PB4) | (1 << PB5);
    //Turn off the load switches
    PORTB &= ~((1 << PB3) | (1 << PB4) | (1 << PB5));
}
bool DigitalInput::readLoad1Call() { //Pin C0 Call for Load 1 (1 - on 0 - off)
    return (PINC & (1 << PINC0));
}
bool DigitalInput::readLoad2Call() { //Pin C1 Call for Load 2 (1 - on 0 - off)
    return (PINC & (1 << PINC1));
}
bool DigitalInput::readLoad3Call() { //Pin C2 Call for Load 3 (1 - on 0 - off)
    return (PINC & (1 << PINC2));
}