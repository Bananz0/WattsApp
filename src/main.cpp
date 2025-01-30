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

//Define portType as enum
typedef enum {
    INPUT = 0x00,
    OUTPUT = 0xFF
} PortType;

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

int main() {
    /*portType Input = 0x00 , Output = 0xFF*/
    initializePorts('A', OUTPUT); // Set Port A to Analog Output (from SM to TB ( Mains Capacity, Analog Ground Ref for Mains Capacity))
    initializePorts('B', INPUT); // Set Port B to Analog Input (from TB to SM ( Power Generation Stats ) )
    initializePorts('C', OUTPUT); // Set Port C to Digital Output ( from SM to TB ( Charge batt, discharge batt, etc...))
    initializePorts('D', INPUT); // Set Port D to Digital Input (from TB to SM ( Call for Load 1,2,3, ground ref)


    return 0;
}
