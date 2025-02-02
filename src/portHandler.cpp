//
// Created by glenm on 2/2/2025.
//

#include "portHandler.h"
#include <avr/io.h>

void initializePorts(const char portName, const PortType portType ) { //This is to initialize the ports to a known output/input state
    /*portType Input = 0x00 , Output = 0xFF*/
    switch (portName) {
        case 'A': DDRA = portType; break;
        case 'B': DDRB = portType; break;
        case 'C': DDRC = portType; break;
        case 'D': DDRD = portType; break;
        default: break; // Invalid port name
    }
}

void finalizePorts() {
    /*portType Input = 0x00 , Output = 0xFF*/
    initializePorts('A', INPUT); // Set Port A to Analog Output (from SM to TB ( Mains Capacity, Analog Ground Ref for Mains Capacity))
    initializePorts('B', OUTPUT); // Set Port B to Analog Input (from TB to SM ( Power Generation Stats ) )
    initializePorts('C', OUTPUT); // Set Port C to Digital Output ( from SM to TB ( Charge batt, discharge batt, eetc...))
    initializePorts('D', INPUT); // Set Port D to Digital Input (from TB to SM ( Call for Load 1,2,3, ground ref)

    //Port A pin defs
    //Will need to use ADC
    //Redundant but set 4 pins as explicit inputs
    DDRA &= ~(_BV(0) /*Busbar Voltage*/
           | _BV(1) /*Busbar Current*/
           | _BV(2) /*Wind Turbine Capacity*/
           | _BV(3) /*PV Capacity*/);

    //Port B pin defs
    //Analog Output
    DDRB |= _BV(0); //Mains Capacity (0-10v)


    //Port C pin defs
    //Digital Outputs
    DDRC |= (_BV(0) /*Charge Battery*/
         | _BV(1) /*Discharge Battery*/
         | _BV(2) /*Load 1 Switch*/
         | _BV(3) /*Load 2 Switch*/
         | _BV(4) /*Load 3 Switch*/); //this took way tooo long bruh

    //Port D pin defs
    //Digital Inputs
    PORTD = 0xFF;
    DDRD &= ~(_BV(0) /*Call for Load 1*/
            | _BV(1) /*Call for Load 2*/
            | _BV(2) /*Call for Load 3*/);
}