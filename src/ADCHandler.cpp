//
// Created by glenm on 2/2/2025.
//

#include "ADCHandler.h"

ADCHandler::ADCHandler() {

}


void ADCHandler::initializeADC() {
    //Remember turn it off when sleeping as is recommended by the ADC
    ADCSRA |= _BV(ADEN)   /*ADC Enable*/
           | _BV(ADPS2)   /*This and PS1 are the pre scaler Bits*/
           | _BV(ADPS1)   /*This combination divides by 64 and more at p259*/
           | _BV(ACIE)    /*Enables the Interrupt for ADC Complete*/
           | ~_BV(ADATE); /*Auto Trigger Disable*/

    ADMUX &= ~(_BV(REFS0) | _BV(REFS1));  //Set the AREF to VCC, 5v and interal Vref turned off
}
void ADCHandler::setADCChannel(const uint8_t channel) {
    ADMUX = ((ADMUX & 0xE0)|channel); //Set the lower three bits to n
}
void ADCHandler::readADC() {
    ADCSRA |= _BV(ADSC); //Start ADC Conversion by setting the ad start convert to high
    //while (ADCSRA & _BV(ADSC)) {} //This checks and waits for the ADC conversion bit but is blocking. (Disabled because it actually blocks the CPU)
}
void ADCHandler::getVoltage(const uint8_t channel) {

    setADCChannel(channel);
    readADC();
    while (!ADCConversionFlag){}
    ADCConversionFlag = 0;
}