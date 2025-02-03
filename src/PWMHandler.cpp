//
// Created by glenm on 2/2/2025.
//

#include "PWMHandler.h"

PWMHandler::PWMHandler() {}

void PWMHandler::initializePWM() { //Initialize the PWM based on pg155 of the ilMatto datasheet
    TCCR2A |=  _BV(COM2A1)    /* Clear OC2A on Compare Match*/
           |   _BV(WGM21)     /* Fast PWM mode */
           |   _BV(WGM20);	  /* 0xFF, BOTTOM */
    TCCR2A &= ~_BV(COM2A0);   /* Set OC2A at bottom*/
    TCCR2B = _BV(CS20);		        	// No Prescaling
    ASSR = 0;			            	// I/O clock
    TIMSK2 = 0;			            	// No interrupts
    TCNT2 = 0x00;	        			// Counter 1
    OCR2A = 0xFF;			         	// Compare Value
}

void PWMHandler::setOutputVoltage(uint8_t voltage) { //PD6 and PD7 for Amplitude Modulation
    DDRD |= _BV(7);
    OCR2A = ((voltage * 1000)/ Vref ) * 0xFF;
}