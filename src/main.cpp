#include <stdio.h>
#include <avr/io.h>


int main() {
    DDRB = 0xFF;
    PORTB = 0x00;
    while (1) {
        PORTB ^= (1 << PB5);
    }
    return 0;
}
