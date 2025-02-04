//
// Created by glenm on 2/2/2025.
//

#include "PWMHandler.h"

PWMHandler::PWMHandler() {}

void PWMHandler::initializePWM() { //Initialize the PWM based on pg155 of the ilMatto datasheet
    //Using PD5
    //initializeTimer1();
    //Using PD7
    initializeTimer2();
}

void PWMHandler::setOutputVoltage(float voltage) { //PD6 and PD7 for Amplitude Modulation
    DDRD |= _BV(7);
    const float scalingFactor = static_cast<float>(voltage) * 1000 / Vref;
    //OCR2A = ((voltage * 1000)/ Vref ) * 0xFF;
    OCR2A = (float) 0xFF * scalingFactor;
}

void PWMHandler::initializeTimer1() {
    TCCR2A |=  _BV(COM2A1)              /* Clear OC2A on Compare Match*/
   |   _BV(WGM21)                       /* Fast PWM mode */
   |   _BV(WGM20);	                    /* 0xFF, BOTTOM */
    TCCR2A &= ~_BV(COM2A0);             /* Set OC2A at bottom*/
    TCCR2B = _BV(CS20);		        	// No Prescaling
    ASSR = 0;			            	// I/O clock
    TIMSK2 = 0;			            	// No interrupts
    TCNT2 = 0x00;	        			// Counter 1
    OCR2A = 0xFF;			         	// Compare Value
}
void PWMHandler::initializeTimer2() {
    TCCR2A |=  _BV(COM2A1)              /* Clear OC2A on Compare Match*/
       |   _BV(WGM21)                   /* Fast PWM mode */
       |   _BV(WGM20);	                /* 0xFF, BOTTOM */
    TCCR2A &= ~_BV(COM2A0);             /* Set OC2A at bottom*/
    TCCR2B = _BV(CS20);		        	// No Prescaling
    ASSR = 0;			            	// I/O clock
    TIMSK2 = 0;			            	// No interrupts
    TCNT2 = 0x00;	        			// Counter 1
    OCR2A = 0xFF;			         	// Compare Value
}
