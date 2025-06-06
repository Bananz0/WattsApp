//
// Created by glenm on 2/2/2025.
//

#define TARGET_TIME_MS 100
#define PRESCALER 64
#include "PWMHandler.h"

PWMHandler::PWMHandler() {}

void PWMHandler::initializePWM() { //Initialize the PWM based on pg155 of the ilMatto datasheet
    //System Timer
    initializeTimer0();
    //Using PD5
    initializeTimer1();
    //Using PD7
    initializeTimer2();

}

void PWMHandler::initializeTimer0() {
    TCCR0A |= (1 << CS01) | (1 << WGM00);
    TCCR0B |= (1 << WGM01) | (1 << CS00);
    TIMSK0 |= (1 << TOIE0);
}

void PWMHandler::setOutputVoltage(float voltage) { //PD6 and PD7 for Amplitude Modulation
    DDRD |= _BV(7);
    const float scalingFactor = static_cast<float>(voltage) / Vref;
    //OCR2A = ((voltage * 1000)/ Vref ) * 0xFF;
    OCR2A = (float) 0xFF * scalingFactor;
}

void PWMHandler::initializeTimer1() {
    TCCR1B |= (1 << WGM12);
    OCR1A = static_cast<uint16_t>((static_cast<uint32_t>(F_CPU) / (static_cast<uint32_t>(PRESCALER) * 1000UL)) * TARGET_TIME_MS - 1);
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << CS11) | (1 << CS10);
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
