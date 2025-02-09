//
// Created by glenm on 2/2/2025.
//

#include "DigitalInput.h"

bool lastLoad1Call = false;
bool lastLoad2Call = false;
bool lastLoad3Call = false;

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
void DigitalInput::checkLoadCallChanges() {
    bool currentLoad1Call = readLoad1Call();
    bool currentLoad2Call = readLoad2Call();
    bool currentLoad3Call = readLoad3Call();
    emergencyScreen = currentLoad1Call || currentLoad2Call || currentLoad3Call;

    //load 1
    if (currentLoad1Call && !lastLoad1Call) {
        PORTB |= (1 << PB3);  //Load 1 on
    } else if (!currentLoad1Call && lastLoad1Call) {
        PORTB &= ~(1 << PB3); //OFF
    }

    //load 2
    if (currentLoad2Call && !lastLoad2Call) {
        PORTB |= (1 << PB4);  //ON
    } else if (!currentLoad2Call && lastLoad2Call) {
        PORTB &= ~(1 << PB4); //OFF
    }

    //load 3
    if (currentLoad3Call && !lastLoad3Call) {
        PORTB |= (1 << PB5);  //ON
    } else if (!currentLoad3Call && lastLoad3Call) {
        PORTB &= ~(1 << PB5); //OFF
    }

    lastLoad1Call = currentLoad1Call;
    lastLoad2Call = currentLoad2Call;
    lastLoad3Call = currentLoad3Call;
}