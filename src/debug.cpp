//
// Created by glenm on 2/2/2025.
//
#include "debug.h"

void signOfLife() {
    PORTB ^= (1 << PB7);
    _delay_ms(500);
}

void LED(uint8_t lightStatus) { //Turn LED ON/OFF/Toggle
    if (lightStatus == 1) {
        PORTB |= _BV(7);
    } else if (lightStatus == 0) {
        PORTB &= ~_BV(7);
    } else if (lightStatus == 2) {
        PORTB ^= _BV(7);
    }
}

//Function to test light for a specific time delay (seconds)
void testLight(const uint8_t time_delay) {
    DDRB |= (1 << PB7); // Set PB7 as output
    PORTB &= ~(1 << PB7); // Start with PB7 low

    for (int i = 0; i < (time_delay * 1000) / 50; i++){
        PORTB ^= (1 << PB7); // Toggle PB7
        _delay_ms(50);       // Delay 50ms
    }
}

void testOutputPin(char portName, const uint8_t pin) {
    //Test Pin B0
    PORTC ^= _BV(pin);
}
