// Created by Jackson.Liu on 25-1-31.
#include "pin.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/iom644p.h>

// Constructor
Pin::Pin(volatile uint8_t &ddr, volatile uint8_t &port, volatile uint8_t &pinReg, uint8_t pinNum, uint8_t adcCh) {
    this->ddr = &ddr;
    this->port = &port;
    this->pinReg = &pinReg; // Read status register
    this->pinMask = (1 << pinNum); // 1 shifted left by pinNum bits
    this->adcChannel = adcCh; // ADC channel number (0-7)
}

// Set as output mode
void Pin::setOutput() {
    *ddr |= pinMask; // Set as output
}

// Set as input mode
void Pin::setInput() {
    *ddr &= ~pinMask;  // Set as input
    *port &= ~pinMask; // Disable pull-up
}

// Set as input mode (enable internal pull-up)
void Pin::setInputPullup() {
    *ddr &= ~pinMask;  // Set as input
    *port |= pinMask;  // Enable pull-up resistor
}

// Set pin to HIGH
void Pin::setHigh() {
    *port |= pinMask;
}

// Set pin to LOW
void Pin::setLow() {
    *port &= ~pinMask;
}

// Toggle pin state
void Pin::toggle() {
    *port ^= pinMask;
}

// Check if pin is HIGH
bool Pin::isHigh() {
    return (*pinReg & pinMask) != 0;
}

// Check if pin is LOW
bool Pin::isLow() {
    return (*pinReg & pinMask) == 0;
}

uint16_t Pin::readADC(float vcc) {
    ADMUX = (1 << REFS0) | adcChannel; // Select AVCC as reference voltage, select ADC channel
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, start conversion, prescaler 64

    while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete

    uint16_t adcValue = ADC; // Read ADC value
    return adcValue; // Convert to voltage
}

void Pin::writeADC(float voltage, float maxVcc) {
    if (voltage < 0) voltage = 0;
    if (voltage > maxVcc) voltage = maxVcc;

    auto dutyCycle = (uint8_t )((voltage / maxVcc) * 255); // Calculate PWM duty cycle

    // Configure PWM
    TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); // Fast PWM mode
    TCCR0B = (1 << CS01);  // Prescaler 8
    OCR0A = dutyCycle; // Set PWM duty cycle
}