#include "time.h" // NOLINT(*-deprecated-headers)
#include <TimeHandler.h>

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

#include "Loads.h"
#include "Sources.h"

#include "WiFiHandler.h"

#define CARROUSEL_E //Use CARROUSEL_E to enable and CARROUSEL_D to disable
//#define DEBUG
#define NORMAL_MODE
//#define SECOND_REVIEW_MODE //Turn off all auxillary functions - Everything low power and only display team L TODO: Fully implement this.


//Moved all timer functions to PWMHandler for central management
AnalogueInput analogueInput;                        //Starts the ADC up in the AI (analog input) constructor    PORTA1
AnalogueOutput analogueOutput;                      //Starts the PWM up in the AO (analog output) constructor   PORTD7
DigitalInput digitalInput;                          //Start the Digital ISR                                     PORTC0-2
DigitalOutput digitalOutput;                        //very basic                                                PORTC3-7

Loads loads(&digitalOutput,&digitalInput);
Sources sources(&analogueInput, &analogueOutput,&digitalOutput);

DisplayHandler display(&loads,&sources);
TimeHandler timeHandler;

HardwareSerial wifiSerial = Serial;
HardwareSerial debugSerial = Serial;


float netCapacity = 0;
uint16_t displayDuration = 4;
uint16_t updateCounter = 0;
Screen screen{};
uint8_t lastScreenUpdateSecond = -1;
uint16_t lastCounter = 0;
String response;

void updateStats() {
    //Time Interrupt - Moved the div/10 to main
    //Measure available wind turbine capacity and PV capacity then calculate total renewable power capacity
    sources.readTurbineCapacity();
    sources.readPvCapacity();

    //Calculate average power and total energy consumption based on bus voltage and bus current (analogue output)
    sources.readBusbarCurrent();
    sources.readBusbarVoltage();

    sources.calculateTotalEnergyAndPower();

    sources.totalEnergy = sources.averagePower * 100 / 1000;

    loads.calculateLoadCapacity();

    netCapacity = sources.totalAvailableCapacity -  loads.currentTotalLoad;
}

void drawTime() {
    if ((Counter % 10 == 0) && (Counter != lastCounter)) {
        utc++;
        lastCounter = Counter;
    };
    timeUTC = gmtime((time_t*)&utc); //Update time (hopefully)
    pictorDrawS(reinterpret_cast<const unsigned char *>(timeHandler.returnTime()),display.timePos,WHITE,RED, Mash,1);
    updateCounter = Counter;
}

void screenCarrousel() {
#ifdef SECOND_REVIEW_MODE
    screen = SEC_REV_SCREEN;
#elif !defined(SECOND_REVIEW_MODE)
    //cycle through the screens somehwow
    if (timeUTC->tm_sec % displayDuration == 0 && timeUTC->tm_sec != lastScreenUpdateSecond) {
        if (emergencyScreen) {
            //Display screen for 5 seconds
            screen = ERROR_SCREEN;
            emergencyScreen = false;
        } else if (!emergencyScreen) {
            screen = static_cast<Screen>((screenPage + 1) % ((int)SCREENCOUNT-3) ); //Subtraced 1 to never cycle to the error screebb
            lastScreenUpdateSecond = timeUTC->tm_sec;
        }
    }
#endif
}

void updateMainStats() {
    if (updateCounter % 10 == 0) {
        updateStats();
        updateCounter = 0;
    }
}

void echoSerial(){
    if(debugSerial.available()) {
        String uartMessageBuff = debugSerial.readStringUntil('\r');
        strncpy((char*)uartMessage, uartMessageBuff.c_str(), sizeof(uartMessage) - 1);
        uartMessage[sizeof(uartMessage) - 1] = '\0';
        // sprintf((char*)emergencyMessage,"%s",response.c_str());
        // memcpy(const_cast<char *>(emergencyMessage), response.c_str(), sizeof(response.length()));;
        debugSerial.println("Received: " + uartMessageBuff);
    }
}

//ADC ISR
ISR(ADC_vect){
    ADCVoltage = (ADC * Vref) / 0x3FF;
    ADCConversionFlag = true;
}
//Counter ISR
ISR(TIMER1_COMPA_vect) {
   // PORTC ^= (1 << PC7); //for testing - interrupt doesn't work with functions in it for some reason
    Counter++;
    //utc++;
}
//Pin Change ISR
ISR(PCINT0_vect) {
    // Read Load Calls 1, 2 and 3
    loads.checkLoadCallChanges();
}

int main() {
    // debugSerial.println("Starting up...");
    finalizePorts();
    // debugSerial.println("Initialized ports");
    testLight(1);                              //Boot Light
    sei();                                               //Enable Global interrupts
    // debugSerial.println("Tested light PB6 and enabled global interrupts");

    display.startDisplay(false);
    // debugSerial.println("Turned on display");
    display.clearScreen();
    // debugSerial.println("Cleared screen");
    display.setBacklight(DisplayHandler::LIGHT);
    display.setOrientation(DisplayHandler::LANDSCAPE);
    WiFiHandler wifiHandler(&Serial, 36);
    // debugSerial.println("Initialized display and set to Landscape");

    wifiHandler.connectToWiFi("\"Glen's XPS\"", "\"eesp8266\"");

    //Boot and Initialization
    // debugSerial.println("Drawing boot sequence");
    display.drawBootSequence();
    // debugSerial.println("Boot Complete...starting program");

    //Start of the LabView Algorighm

    // Maximum battery capacity = 24Ah
    // 1 Hour Simulation time = 1 Min Runtime
    // Default: Charge battery and turn all loads off.
    sources.requestMains(0);
    sources.chargeBattery();
    loads.turnLoadOff(1);
    loads.turnLoadOff(2);
    loads.turnLoadOff(3);
    loads.checkLoadCallChanges();

    //program automatically listens for calls for loads and updates.
    //could be a good idea to use that as a way to update the stats.
    //Could be also be a good idea to enable pin change interrupts for every one of the pins as the busbar could
    //change while the turbine capacity maintaining its original value.
    //since 1 min is 1 hour and there is the potential of having the NTP, we could time the changes from the start
    //to get the values from the TB instantly. not sure if the speed at which the change is detected nor the
    //resolution of the scope

    //CLion complains about forever while loop
    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        drawTime();
        screenCarrousel();
        updateMainStats();
        echoSerial();

        display.carouselScreen(screen);

        //Implement LabView Algorithm


    }
}


// strncpy((char*)emergencyMessage, response.c_str(), 39);
// emergencyMessage[response.length()] = '\0'; // Null-terminate

// if (sources.busbarVoltage > 2.5) {
//     sources.requestMains(11);
// }
//Used for testing the error screen which hopefully will never have to happen

// display.carouselScreen(screen);


// if(wifiSerial.available()) {
//     String response = wifiSerial.readStringUntil('\r');
//     debugSerial.println(response);  // Print to debug serial
// }

// if(wifiSerial.available()) {
//     char c = wifiSerial.read();
//     wifiSerial.write(c);  // Echo single character
// }


// debugString = wifiSerial.readStringUntil('\n');
//
// debugSerial.println(debugString);

// sprintf(debugString, "Busbar Current: %.2f", sources.busbarCurrent);
//
// if (bCurrentBuffer != sources.busbarCurrent) {
//     bCurrentBuffer = sources.busbarCurrent;
//     debugSerial.println(debugString);
// }.

