#include <avr/io.h>
#include <util/delay.h>



//Function to test light for a specific time delay (seconds)
void testLight(const uint8_t time_delay) {
    uint8_t i = time_delay/50;
    DDRB |= (1 << PB7); // Set PB7 as output
    PORTB &= ~(1 << PB7); // Start with PB7 low

    while (i<20) {
        PORTB ^= (1 << PB7); // Toggle PB7
        _delay_ms(50);       // Delay 50ms
        i+=1;
    }
}
void initializePorts(char portName, int8_t portType ) { //This is to initialize the ports to a known output/input state
    /*portType Input = 0x00 , Output = 0xFF*/
    switch (portName) {
        case 'A': DDRA = portType; break;
        case 'B': DDRB = portType; break;
        case 'C': DDRC = portType; break;
        case 'D': DDRD = portType; break;
        default: break; // Invalid port name
    }
}

int main() {
    /*portType Input = 0x00 , Output = 0xFF*/
    initializePorts('A', 0x00); //Set Port A to Analog Input ( Mains Capacity )
    initializePorts('B', 0x00); // Set Port B to Digital
    initializePorts('C', 0x00);
    initializePorts('D', 0x00);


    return 0;
}
