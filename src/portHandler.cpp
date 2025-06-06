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
    //initializePorts('A', INPUT); // Set Port A to Analog Output (from SM to TB ( Mains Capacity, Analog Ground Ref for Mains Capacity))
    // initializePorts('B', OUTPUT); // Set Port B to Analog Input (from TB to SM ( Power Generation Stats ) )
    // initializePorts('C', OUTPUT); // Set Port C to Digital Output ( from SM to TB ( Charge batt, discharge batt, eetc...))
    //initializePorts('D', INPUT); // Set Port D to Digital Input (from TB to SM ( Call for Load 1,2,3, ground ref)

    //Port A pin defs
    //Will need to use ADC
    //Redundant but set 4 pins as explicit inputs
    DDRA &= ~(_BV(0) /*Busbar Voltage*/
            | _BV(1) /*Busbar Current*/
            | _BV(2) /*Wind Turbine Capacity*/
            | _BV(3) /*PV Capacity*/
            | _BV(5) /*Call for Load 1*/
            | _BV(6) /*Call for Load 2*/
            | _BV(7) /*Call for Load 3*/);


    DDRA |=  _BV(4) /*Potential ESP8266 Enable Pin*/;
    PORTA &= ~(1 << PORTA4);



    //PORT B AND C ARE USED FOR THE DISPLAY

    //Port B pin defs
    //Analog Output
    // DDRB |= _BV(0); //Mains Capacity (0-10v)

    //Port C pin defs
    //Digital Outputs
    // DDRC |= (_BV(2) /*Charge Battery*/
    //      | _BV(3) /*Discharge Battery*/
    //      | _BV(4) /*Load 1 Switch*/
    //      | _BV(5) /*Load 2 Switch*/
    //      | _BV(6) /*Load 3 Switch*/); //this took way tooo long bruh

    //PORT B AND C ARE USED FOR THE DISPLAY

    //Port D pin defs
    //Digital Outputs
    DDRD |= (_BV(2) /*Charge Battery*/
         | _BV(3) /*Discharge Battery*/
         | _BV(4) /*Load 1 Switch*/
         | _BV(5) /*Load 2 Switch*/
         | _BV(6) /*Load 3 Switch*/); //this took way tooo long bruh
    PORTD &= ~(1 << PORTD2);
    PORTD &= ~(1 << PORTD3);
    PORTD &= ~(1 << PORTD4);
    PORTD &= ~(1 << PORTD5);
    PORTD &= ~(1 << PORTD6);

}