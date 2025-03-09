#include "time.h" // NOLINT(*-deprecated-headers)
#include <TimeHandler.h>
#include <WString.h>

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

#include "ESP8266Handler.h"

#define CARROUSEL_E //Use CARROUSEL_E to enable and CARROUSEL_D to disable
//#define DEBUG

#define NORMAL_MODE
//#define SECOND_REVIEW_MODE //Turn off all auxillary functions - Everything low power and only display team L TODO: Fully implement this.
//#define UARTDEBUG //Use this to enable UART debug screen


//Moved all timer functions to PWMHandler for central management (changed ports cause of display later on)
AnalogueInput analogueInput;                        //Starts the ADC up in the AI (analog input) constructor    PORTA1
AnalogueOutput analogueOutput;                      //Starts the PWM up in the AO (analog output) constructor   PORTD7
DigitalInput digitalInput;                          //Start the Digital ISR                                     PORTC0-2
DigitalOutput digitalOutput;                        //very basic                                                PORTC3-7

Loads loads(&digitalOutput,&digitalInput);
Sources sources(&analogueInput, &analogueOutput,&digitalOutput);

DisplayHandler display(&loads,&sources);
TimeHandler timeHandler;

HardwareSerial *wifiSerial = &Serial;
ESP8266Handler esp8266Handler(&loads, &sources);

float netCapacity = 0;
uint16_t displayDuration = 2;
uint16_t updateCounter = 0;
Screen screen{};
uint8_t lastScreenUpdateSecond = -1;
uint16_t lastCounter = 0;
String response;
bool dayHasChanged = false;

void updateStats() {
    //Time Interrupt - Moved the div/10 to main
    //Measure available wind turbine capacity and PV capacity then calculate total renewable power capacity
    sources.readTurbineCapacity();
    sources.readPvCapacity();

    //Calculate average power and total energy consumption based on bus voltage and bus current (analogue output)
    sources.readBusbarCurrent();
    sources.readBusbarVoltage();

    sources.calculateTotalEnergyAndPower();
    sources.calculateTotalAvailableCapacity();

    sources.totalEnergy = sources.averagePower * 100 / 1000;

    loads.calculateLoadCapacity();

    netCapacity = sources.totalAvailableCapacity -  loads.totalLoadCapacity;
}

void drawTime() {
    if ((Counter % 5 == 0) && (Counter != lastCounter)) {
        utc++;
        lastCounter = Counter;
    }
    timeUTC = gmtime((time_t*)&utc); //Update time (hopefully)
    pictorDrawS(reinterpret_cast<const unsigned char *>(timeHandler.returnTime()),display.timePos,WHITE,RED, Mash,1);
    updateCounter = Counter;
}

void screenCarrousel() {
#ifdef SECOND_REVIEW_MODE
    screen = SEC_REV_SCREEN;
#elif defined(UARTDEBUG)
    screen = UART_SCREEN;
#elif defined(NORMAL_MODE)
    //cycle through the screens
    if (timeUTC->tm_sec % displayDuration == 0 && timeUTC->tm_sec != lastScreenUpdateSecond) {
        if (emergencyScreen) {
            //Display screen for 5 seconds
            screen = ERROR_SCREEN;
            emergencyScreen = false;
        } else {
            //Subtracted to skip the alternate screens
            screen = static_cast<Screen>((screenPage + 1) % (SCREEN_COUNT-3) );
        }
        lastScreenUpdateSecond = timeUTC->tm_sec;
    }
#endif
}

void updateMainStats() {
    if (LoadChangeFlag) {
        loads.checkLoadCallChanges();
    }
    if (updateCounter % 5 == 0) {
        updateStats();
        updateCounter = 0;
    }
}

void checkForDayChange() {
    //Copy all the stats to a temporary store. Array or struct could work
    const float sourcesStats[4] = {sources.busbarVoltage, sources.busbarCurrent, sources.pvCapacity, sources.windTurbineCapacity};
    bool loadStats[3] = {loads.currentLoad1Call, loads.currentLoad2Call, loads.currentLoad3Call};

    static float sourceStatsBuffer[4] = {0};
    static bool loadsStatsBuffer[3] = {false};

    float percentStatsDifference[4];
    bool loadStatsDifferent = false;

    uint8_t percentStatsDifferenceCount = 0; //ran out of names bruh

    //Calcultae the difference between the two (for sources)
    for (int i = 0; i < 4; i++) {
        if ((sourceStatsBuffer[i]!=0)&&(sourcesStats[i]!=0)) {
            percentStatsDifference[i] = (sourcesStats[i] - sourceStatsBuffer[i])/ sourceStatsBuffer[i] * 100;
            abs(percentStatsDifference[i]) > 10 ? percentStatsDifferenceCount++ : 0;
        }
    }

    //Calculate the difference between the loads
    for (int i = 0; i < 3; i++) {
        if (loadsStatsBuffer[i] != loadStats[i]) {
            loadStatsDifferent = true;
        }
        loadsStatsBuffer[i] = loadStats[i];
    }

    //dopy new array buffer from main array
    for (int i = 0; i < 4; i++) {
        sourceStatsBuffer[i] = sourcesStats[i];
    }

    dayHasChanged = (percentStatsDifferenceCount > 2) || loadStatsDifferent;

}

void  controlAlgrithm() {
        //program automatically listens for calls for loads and updates.
        //could be a good idea to use that as a way to update the stats.
        //Could be also be a good idea to enable pin change interrupts for every one of the pins as the busbar could
        //change while the turbine capacity maintaining its original value.
        //since 1 min is 1 hour and there is the potential of having the NTP, we could time the changes from the start
        //to get the values from the TB instantly. not sure if the speed at which the change is detected nor the
        //resolution of the scope

        //updateMainStats(); //Should update every change of pins or every 10 seconds
        if (timeUTC->tm_sec % 5 == 0) {
            checkForDayChange();
        }

        //Implement LabView Algorithm

        //Still need to figure out how to accurately detect the changes in days without using a blocking _delay(6000) which would work but would have an effect in the other operations in the smart meter
        //This is not accurate as there is no RTC on the board but could use NTP or add the clock later
        //If the statuses change drastically >10% add 1 to the simulated day. and subtract 1 from the remaining dayCount.

        if (dayHasChanged) {
            if (sources.dailyMainsChange != 0) {
                sources.mainsCapacity += sources.dailyMainsChange;
                sources.dailyMainsChange= 0;  // Reset
            }

            if (sources.dailyBatteryChange != 0) {
                sources.batteryCapacity += sources.dailyBatteryChange;

                //keep within range
                if (sources.batteryCapacity < 0) {
                    sources.batteryCapacity = 0;
                }
                if (sources.batteryCapacity > 24) {
                    sources.batteryCapacity = 24;
                }

                sources.dailyBatteryChange = 0; //Reset
            }

            if (sources.mainsCapacity < 0) {
                sources.mainsCapacity = 0;
            }
            if (sources.mainsCapacity > 2) {
                sources.mainsCapacity = 2;
            }

            analogueOutput.setMainsCapacity(sources.mainsCapacity);

            dayCount += 1;
            remainingDays-=1;
            dayHasChanged = false;

            esp8266Handler.sendDataToWifi();
        }


        //Charge battery if there is available surplus capacity and since we can only charge at 1A per hour
        if ((sources.totalRenewableCapacity > loads.totalLoadCapacity)&&(sources.totalRenewableCapacity > (1 + loads.totalLoadCapacity))) {
            sources.chargeBattery(); //Charges it by 1A
        }

        //Deplete battery capacity if the capacity can last days close to the end of the "Simulation"
        if ((sources.batteryCapacity > remainingDays)) {
            sources.requestMains(0);
            sources.requestBattery(1);
        }

        //This should check for the available load capacity and the renewables capacity. If not sufficient, it should call the battery.
        //Ultimately if not available it should call the mains as a last option.

        if (netCapacity<0 && netCapacity!=0) { //redundant but idc
            if (sources.batteryCapacity > 1 && sources.totalRenewableCapacity < loads.totalLoadCapacity) {
                sources.requestBattery(1); // We can only discharge 1A per day
            }  else if (sources.batteryCapacity < 1 && sources.totalRenewableCapacity < loads.totalLoadCapacity) {
                sources.requestMains(-1 * netCapacity);
            }
            // break; // If the battery can sustain it break the if condition. Do not trust this cause im not sure if i should use break or continue. Will search later. - switched to elif
        }

        //Last resort.
        //Turn off loads if the battery capacity, renewables and main can't match the output of the loads required
        //Will need to consider priority into all of these.
        sources.loadDeficit =  sources.totalAvailableCapacity - loads.totalLoadCapacity;
        if (sources.totalAvailableCapacity < loads.totalLoadCapacity) {
            //will implement load priority
            for (uint8_t loadCount = 0; loadCount < 3; loadCount++) {
                if (loads.currentLoadStatus[loadCount] && loads.currentLoad[loadCount] > sources.totalAvailableCapacity ) {
                    loads.loadOverride[loadCount] = true;
                    loads.turnLoadOff(loadCount);
                }
            }
        } else if (sources.totalAvailableCapacity > loads.totalLoadCapacity) {
            for (bool & loadCount : loads.loadOverride) {
                loadCount = false;
            }
        }
}

//ADC ISR
ISR(ADC_vect){
    ADCraw = ADC;
    //ADCVoltage = (ADC * Vref) / 0x3FF; //Moved to main
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
    // loads.checkLoadCallChanges(); //Moved to main
    LoadChangeFlag = true;
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
    // debugSerial.println("Initialized display and set to Landscape");

                                                                            //wifiHandler.connectToWiFi("\"Glen's XPS\"", "\"eesp8266\"");

    //Boot and Initialization
    // debugSerial.println("Drawing boot sequence");
    display.drawBootSequence();
    // debugSerial.println("Boot Complete...starting program");

    //Start of the LabView Algorighm

    // Maximum battery capacity = 24Ah
    // 1 Hour Simulation time = 1 Min Runtime
    dayCount = -1, remainingDays = 24;
    // Default: Charge battery and turn all loads off.
    sources.requestMains(0);
    sources.chargeBattery();
    loads.turnLoadOff(1);
    loads.turnLoadOff(2);
    loads.turnLoadOff(3);
    loads.checkLoadCallChanges();

//   wifiSerial->begin(115200);


    // ReSharper disable once CppDFAEndlessLoop - CLion complains about forever while loop
    while (true) {
        drawTime();
        screenCarrousel();  //screen - variable to give to display.carrouselScreen() below
        display.carouselScreen(screen); //Screen - 1. screen (normal carrousel), others - BUSBAR_SCREEN, UART_SCREEN and so o
        updateMainStats();
        controlAlgrithm();

        //requestMains(20);  //this is a hard request to test if the clamp works. Should trigger an error screen and TODO: Document this feature
        //wifiHandler.echoSerial();
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