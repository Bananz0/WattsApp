#include "globalVariables.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "debug.h"

#include "portHandler.h"

#include "AnalogueInput.h"
#include "AnalogueOutput.h"
#include "DigitalInput.h"
#include "DigitalOutput.h"

#include "DisplayHandler.h"

#include "../lib/pictor.h"
#include "../fonts/OryxB.h"
#include "../fonts/Mash.h"
#include "sprite_ghost.h"
#include "sprite_check.h"

#define TARGET_TIME_MS 100
#define PRESCALER 64
// #define F_CPU 12000000 This is already defined in the CMake


point topLeft = {0, 0};
point bottomRight = {319, 239};



DisplayHandler::DisplayHandler() {

}

DisplayHandler::~DisplayHandler() {
}

void DisplayHandler::drawRectangle() {
}

void DisplayHandler::drawCircle() {
}

void DisplayHandler::drawTriangle() {
}

void DisplayHandler::drawEllipse() {
}

void DisplayHandler::drawBitmap() {
}

void DisplayHandler::drawPicture() {
}

void DisplayHandler::drawBootLogo() {
}

void DisplayHandler::startDisplay(bool vsync) {
   pictorInit(vsync);
    if (vsync)  pictorFrame();
    pictorCanvasSet(topLeft, bottomRight);
}
void DisplayHandler::stopDisplay() {

}
void DisplayHandler::setBacklight(state State) {
    pictorBacklightState(State);
}
void DisplayHandler::toggleBacklight() {
    pictorBacklightState(-1);
}
void DisplayHandler::setOrientation(orientation rotation) {
    pictorSetRotation(rotation);
}
void DisplayHandler::clearScreen() {
    //set screen to black
}
void DisplayHandler::drawText() {

}
void DisplayHandler::drawUIsimple() {
    uint16_t lineColor = encode656Colour(0, 255, 0);  // Green color for the line
    point start = {10, 200};
    point end = {200, 200};
    pictorDrawLine(start, end, lineColor);  // Draw a green line
    pictorDrawBox(topLeft, bottomRight, lineColor);
}





volatile uint32_t Counter=0;//Only used for time interrupt

// Timer initialization function
void initializeTimer1() {
    TCCR1B |= (1 << WGM12);
    OCR1A = (F_CPU / (PRESCALER * 1000)) * TARGET_TIME_MS - 1;
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << CS11) | (1 << CS10);
}

AnalogueInput analogueInput;                        //Starts the ADC up in the AI (analog input) constructor    PORTA1
//AnalogueOutput analogueOutput;                      //Starts the PWM up in the AO (analog output) constructor   PORTD7
DigitalInput digitalInput;                          //Start the Digital ISR                                     PORTC0-2
DigitalOutput digitalOutput;                        //very basic                                                PORTC3-7

//ADC ISR
ISR(ADC_vect){
    ADCVoltage = (ADC * Vref) / 0x3FF;
    ADCConversionFlag = true;
}
//Counter ISR
ISR(TIMER1_COMPA_vect) {
    Counter++;
    //Time Interrupt
    if (Counter % 10 == 0) {
        //Measure available wind turbine capacity and PV capacity then calculate total renewable power capacity
        float windTurbineCapacity = analogueInput.turbineCurrentCapacity();
        float pvCapacity = analogueInput.pvCurrentCapacity();
        float totalRenewablePower = windTurbineCapacity + pvCapacity;

        //Calculate average power and total energy consumption based on bus voltage and bus current (analogue output)
        float busbarVoltage = analogueInput.busbarVoltage();
        float busbarCurrent = analogueInput.busbarCurrent();
        float averagePower = busbarVoltage * busbarCurrent;
        float totalEnergy = averagePower * TARGET_TIME_MS / 1000;
    }
}
//Pin Change ISR
ISR(PCINT2_vect) {
    // Read Load Calls 1, 2 and 3
    digitalInput.checkLoadCallChanges();
}

int main() {
    finalizePorts();
    testLight(1);                              //Boot Light
    sei();                                               //Enable Global interrupts

    //Initialize the display - will refractor this to DisplayHandler
    pictorInit(1);
    pictorSetRotation(0);

    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        //signOfLife();                                   //Blink LED every .5 sec to show sign of life
        uint16_t current = analogueInput.busbarCurrent();




    }
}
