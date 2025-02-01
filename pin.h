// Created by Jackson.Liu on 25-1-31.
#ifndef PIN_H
#define PIN_H

#include <avr/io.h>
#include <avr/iom644p.h>

class Pin {
private:
    volatile uint8_t *port;   // PORTx register
    volatile uint8_t *ddr;    // DDRx data direction register
    volatile uint8_t *pinReg; // PINx read status register
    uint8_t pinMask;          // Target pin bit mask
    uint8_t adcChannel;       // ADC channel number (0-7)

public:
    // Constructor
    Pin(
            volatile uint8_t &ddr,
            volatile uint8_t &port,
            volatile uint8_t &pinReg,
            uint8_t pinNum,
            uint8_t adcCh = 0xFF
    );

    // Set as input or output
    void setOutput();
    void setInput();
    void setInputPullup(); // Enable internal pull-up: avoid floating

    // Set pin state
    void setHigh();
    void setLow();
    void toggle(); // Toggle pin state

    // Read pin state
    bool isHigh();
    bool isLow();

    // Read ADC value
    uint16_t readADC(float vcc = 5.0); // Read ADC value
    void writeADC(float voltage, float maxvcc = 5.0); // Write ADC value
};

#endif //PIN_H