#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "debug.h"



float Vref = 5;      //Reference Voltage, not sure if 3.3 or 5
volatile float ADCVoltage;   //Global ADC voltage variable and idk if this is good
volatile uint8_t ADCConversionFlag = 0;

void LED(uint8_t lightStatus) { //Turn LED ON/OFF/Toggle
    if (lightStatus == 1) {
        PORTB |= _BV(7);
    } else if (lightStatus == 0) {
        PORTB &= ~_BV(7);
    } else if (lightStatus == 2) {
        PORTB ^= _BV(7);
    }
}

//ISR ADC
ISR(ADC_vect){
    ADCVoltage = (float)(ADC * Vref) / 0x3FF;
    ADCConversionFlag = 1;
}

class ADCHandler {
public:
    ADCHandler() {}
    void initializeADC() {
        //Remember turn it off when sleeping as is recommended by the ADC
        ADCSRA |= _BV(ADEN)   /*ADC Enable*/
               | _BV(ADPS2)   /*This and PS1 are the pre scaler Bits*/
               | _BV(ADPS1)   /*This combination divides by 64 and more at p259*/
               | _BV(ACIE)    /*Enables the Interrupt for ADC Complete*/
               | ~_BV(ADATE); /*Auto Trigger Disable*/

        ADMUX &= ~(_BV(REFS0) | _BV(REFS1));  //Set the AREF to VCC, 5v and interal Vref turned off
    }
    void setADCChannel(const uint8_t channel) {
        ADMUX = ((ADMUX & 0xE0)|channel); //Set the lower three bits to n
    }
    void readADC() {
        ADCSRA |= _BV(ADSC); //Start ADC Conversion by setting the ad start convert to high
        //while (ADCSRA & _BV(ADSC)) {} //This checks and waits for the ADC conversion bit but is blocking. (Disabled because it actually blocks the CPU)
    }
    void getVoltage(const uint8_t channel) {
        setADCChannel(channel);
        readADC();
        while (!ADCConversionFlag){}
        ADCConversionFlag = 0;
    }
};

class PWMHandler {
    public:
    PWMHandler() {}
    void initializePWM() {
        TCCR2A = _BV(COM2A0) | _BV(COM2A1);	// Set OC1A on Compare Match
        TCCR2A |= _BV(WGM21) | _BV(WGM20);	// PWM, Fast, 0xFF, BOTTOM
        TCCR2B = _BV(CS20);		        	// clk/1 prescaler
        ASSR = 0;			            	// I/O clock
        TIMSK2 = 0;			            	// No interrupts
        TCNT2 = 0x00;	        			// Counter 1
        OCR2A = 0xFF;			         	// Compare register
        DDRD |= _BV(7);			        	// Output
    }
    void setOutputVoltage(uint8_t voltage) { //PD6 and PD7 for Amplitude Modulation
        DDRD |= _BV(7);
        if (voltage > Vref) voltage = Vref;
        //OCR2A = (voltage / Vref) * 255;
        OCR2A = (100 - voltage) * 0xFF / 100;		// Duty cycle
    }
};

class AnalogueInput {
public:
    AnalogueInput() {
        aDCHandler.initializeADC();        
    }
    float turbineCurrentCapacity() { //Read and return turbine capacity current
        //Read Pin A2 using ADC
        aDCHandler.getVoltage(2);
        return ADCVoltage; //Amps 1:1 mapping
    }

    float pvCurrentCapacity() { //Read and return the PV Capacity current
        //Read Pin A3 using ADC
        aDCHandler.getVoltage(3);
        return ADCVoltage; //Amps 1:1 mapping
    }

    float busbarCurrent() {
        //Read Pin A1 using ADC
        aDCHandler.getVoltage(1);
        return ADCVoltage; //Amps 1:1 mapping
    }

    float busbarVoltage() {
        //Read Pin A0 using ADC (scaled down from 10v)
        aDCHandler.getVoltage(0);
        return 100.0f * ADCVoltage; //Volts
    }

private:
    ADCHandler aDCHandler;
}; //First RJ45 Port A

class AnalogueOutput {
    public:
    AnalogueOutput() {
        pWMHandler.initializePWM();
    }
    void setMainsCapacity(uint16_t mainsCapacity) { //Set Mains Capacity using PWM from 0 to 10v
        pWMHandler.setOutputVoltage(mainsCapacity);
    }
private:
    PWMHandler pWMHandler;

};//Second RJ45 Port B

class DigitalInput {

};  //Third RJ45 Port D

class DigitalOutput {

}; //Fourth RJ45 Port C

void signOfLife() {
    PORTB ^= (1 << PB7);
    _delay_ms(500);
}

// void testUART (int n)
// {
//     printf("Hello World! %d \n" , n);
//     ugetchar0();
// }

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
    PORTB ^= _BV(pin);
    _delay_ms(100);
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

void finalizePorts() {
    /*portType Input = 0x00 , Output = 0xFF*/
    initializePorts('A', INPUT); // Set Port A to Analog Output (from SM to TB ( Mains Capacity, Analog Ground Ref for Mains Capacity))
    initializePorts('B', OUTPUT); // Set Port B to Analog Input (from TB to SM ( Power Generation Stats ) )
    initializePorts('C', OUTPUT); // Set Port C to Digital Output ( from SM to TB ( Charge batt, discharge batt, eetc...))
    initializePorts('D', INPUT); // Set Port D to Digital Input (from TB to SM ( Call for Load 1,2,3, ground ref)

    //Port A pin defs
    //Will need to use ADC
    //Redundant but set 4 pins as explicit inputs
    DDRA &= ~(_BV(0) /*Busbar Voltage*/
           | _BV(1) /*Busbar Current*/
           | _BV(2) /*Wind Turbine Capacity*/
           | _BV(3) /*PV Capacity*/);

    //Port B pin defs
    //Analog Output
    DDRB |= _BV(0); //Mains Capacity (0-10v)


    //Port C pin defs
    //Digital Outputs
    DDRC |= (_BV(0) /*Charge Battery*/
         | _BV(1) /*Discharge Battery*/
         | _BV(2) /*Load 1 Switch*/
         | _BV(3) /*Load 2 Switch*/
         | _BV(4) /*Load 3 Switch*/); //this took way tooo long bruh

    //Port D pin defs
    //Digital Inputs
    PORTD = 0xFF;
    DDRD &= ~(_BV(0) /*Call for Load 1*/
            | _BV(1) /*Call for Load 2*/
            | _BV(2) /*Call for Load 3*/);
}


int main() {
    sei();                                              //Enable Global interrupts
    AnalogueInput analogueInput;                        //Starts the ADC up in the AI (analog input) constructor
    AnalogueOutput analogueOutput;                      //Starts the PWM up in the AO (analog output) constructor

    finalizePorts();

    init_debug_uart0();

    testLight(1);

    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        //signOfLife();                                 //Blink LED every .5 sec to show sign of life
        testOutputPin('B', 0);
        float current = analogueInput.pvCurrentCapacity();
        analogueOutput.setMainsCapacity(5);


        if (current < 1) {
            LED(1);
            _delay_ms(500);
            LED(0);
            _delay_ms(500);
            LED(1);
            _delay_ms(500);
            LED(0);

        } else if (current > 1 && current < 2) {
            LED(0);
        } else if (current > 2 && current < 3) {
            LED(1);
            _delay_ms(500);
            LED(0);
            _delay_ms(500);
            LED(0);
            _delay_ms(500);
            LED(1);

        } else if (current > 3 && current < 4) {
            LED(0);
        } else if (current > 4 && current < 5) {
            LED(0);
        }

        //Print to UART
        printf("%4d : %6.5fV \n",ADC,  current);


    }
}
