#include "time.h" // NOLINT(*-deprecated-headers)
#include <TimeHandler.h>
#include "usbdrv/usbdrv.h"
#include <WString.h>

#include "globalVariables.h"

#include <avr/io.h>
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

uint16_t displayDuration = 2;
uint16_t updateCounter = 0;
Screen screen{};
uint8_t lastScreenUpdateSecond = -1;
uint16_t lastCounter = 0;
String response;

uint32_t lastUtcUpdateMillis = 0;
uint32_t utcUpdateInterval = 1000;//1sec
uint32_t lastScreenUpdateMillis = 0;
uint32_t screenUpdateInterval = 5000;//2secs
uint32_t lastStatsUpdateMillis = 0;
uint32_t statsUpdateInterval = 100;//100ms
uint32_t lastEspUpdateUpdateMillis = 0;
uint32_t espUpdateInterval = 2500;//seconds1.5
uint32_t lastHourCheckUpdateMillis = 0;
uint32_t hourCheckUpdateInterval = 2000;//2 sec
uint32_t simulatedSecondCounter = 0;
uint32_t lastSimulatedSecondUpdateMillis = 0;
uint32_t simulatedSecondUpdateInterval = 1000; //Update every 1 second
uint8_t simulatedSeconds = 0;
uint8_t simulatedHourUpdateThresholdSeconds = 55; //when hto start checkign for the new hour or "simulated day"

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

}

void drawTime() {
    const uint32_t currentMillis = millis();
    if (currentMillis - lastUtcUpdateMillis >= utcUpdateInterval) {
        lastUtcUpdateMillis = currentMillis;
        utc++;
    }
    timeUTC = gmtime(const_cast<time_t *>(&utc)); //Update time (hopefully)
    pictorDrawS(reinterpret_cast<const unsigned char *>(timeHandler.returnTime()),display.timePos,WHITE,RED, Mash,1);
    updateCounter = currentMillis;
}

void screenCarrousel() {
    uint32_t currentMillis = millis();
#ifdef SECOND_REVIEW_MODE
    screen = SEC_REV_SCREEN;
#elif defined(UARTDEBUG)
    screen = UART_SCREEN;
#elif defined(NORMAL_MODE)
    //cycle through the screensF
    if (currentMillis - lastScreenUpdateMillis >= screenUpdateInterval) {
        lastScreenUpdateMillis = currentMillis;
        if (emergencyScreen) {
            //Display screen for 5 seconds
            screen = ERROR_SCREEN;
            emergencyScreen = false;
            lastScreenUpdateMillis = currentMillis - screenUpdateInterval + 5000;
        } else {
            //Subtracted to skip the alternate screens
            screen = static_cast<Screen>((screenPage + 1) % (SCREEN_COUNT-3) );
        }
    }
#endif
}

void updateMainStats() {
    uint32_t currentMillis = millis();
    if (LoadChangeFlag) {
        loads.checkLoadCallChanges();
        LoadChangeFlag = false;
    }
    if (currentMillis - lastStatsUpdateMillis >= statsUpdateInterval) {
        lastStatsUpdateMillis = currentMillis;
        updateStats();
    }
}

void checkForHourChange() {
    //Copy all the stats to a temporary store. Array or struct could work
    const float sourcesStats[4] = {sources.busbarVoltage, sources.busbarCurrent, sources.pvCapacity, sources.windTurbineCapacity};
    bool loadStats[3] = {loads.currentLoadCall[0], loads.currentLoadCall[1], loads.currentLoadCall[2]};

    static float sourceStatsBuffer[4] = {0};
    static bool loadsStatsBuffer[3] = {false};

    float percentStatsDifference[4];
    bool loadStatsDifferent = false;

    uint8_t percentStatsDifferenceCount = 0; //ran out of names bruh

    //Calcultae the difference between the two (for sources)
    for (int i = 0; i < 4; i++) {
        if ((sourceStatsBuffer[i]!=0)&&(sourcesStats[i]!=0)) {
            percentStatsDifference[i] = (sourcesStats[i] - sourceStatsBuffer[i])/ sourceStatsBuffer[i] * 100;
            abs(percentStatsDifference[i]) > 2 ? percentStatsDifferenceCount++ : 0;
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

    hourHasChanged = (percentStatsDifferenceCount > 2) || loadStatsDifferent;
    hourHasChanged = true;
}

void  controlAlgrithm() {
    uint32_t currentMillis = millis();
    float availableCapacity = sources.totalRenewableCapacity;

    //Simulated time changes with teh correction if undershot and cap it to 60 cause the avr is slower
    if (currentMillis - lastSimulatedSecondUpdateMillis >= simulatedSecondUpdateInterval) {
        lastSimulatedSecondUpdateMillis = currentMillis;
        simulatedSeconds++;

        if (simulatedSeconds >= 60) {
            simulatedSeconds = 0;
            hourHasChanged = true;
        } else if (simulatedSeconds >= simulatedHourUpdateThresholdSeconds) {
            checkForHourChange();
        }

        if (hourHasChanged) {
            // Reset simulated seconds if a change is detected
            simulatedSeconds = 0;
        }
    }
    //Implement LabView Algorithm

    //Still need to figure out how to accurately detect the changes in days without using a blocking _delay(6000) which would work but would have an effect in the other operations in the smart meter
    //This is not accurate as there is no RTC on the board but could use NTP or add the clock later
    //If the statuses change drastically >10% add 1 to the simulated day. and subtract 1 from the remaining dayCount.

    if (hourHasChanged) {
        if (sources.hourMainsChange != 0) {
            sources.mainsCapacity += sources.hourMainsChange;
            sources.hourMainsChange= 0;  // Reset
        }

        if (sources.hourBatteryChange != 0) {
            sources.batteryCapacity += sources.hourBatteryChange;

            //keep within range
            if (sources.batteryCapacity < 0) {
                sources.batteryCapacity = 0;
            }
            if (sources.batteryCapacity > 24) {
                sources.batteryCapacity = 24;
            }
        }
        sources.hourBatteryChange = 0;

        if (sources.mainsCapacity < 0) {
            sources.mainsCapacity = 0;
        }
        if (sources.mainsCapacity > 2) {
            sources.mainsCapacity = 2;
        }

        analogueOutput.setMainsCapacity(sources.mainsCapacity);

        hourCount += 1;
        //firstDay = false;
        remainingHours-=1;
        hourHasChanged = false;
        hourChangeStartTime = millis();
    }


    //Last resort.
    //Turn off loads if the battery capacity, renewables and main can't match the output of the loads required
    //Will need to consider priority into all of these. -- Old remnant i will prob not delete this
    sources.loadDeficit =  sources.totalAvailableCapacity - loads.totalLoadCapacity;

    //Moving away from Hysterisis, this is getting too complicated to debug and follow but it works and doesnt charge the battery - also aold remnant
    float neededCapacity = 0;
    //Finds teh needed capacity from the loads to request the main if necessary
    for (int loadCoumt = 0; loadCoumt < 3; loadCoumt++) {
        if(!loads.loadOverride[loadCoumt] && loads.currentLoadCall[loadCoumt]) {
            if(loadCoumt != 0 || (loadCoumt == 0 && (hourCount >= 8 && hourCount <= 22))) {
                neededCapacity += loads.currentLoadCapacity[loadCoumt];
            }
        }
    }

    //Check iff the battery can be used and if ot call the mains.
    float mainsRequest = 0;

    if (neededCapacity > availableCapacity) {
        //If we have battery discharge request mains from available energy
        if (sources.batteryCapacity > 0) {
            sources.requestBattery(true); //Request battery charge caps battery change with capacity
            neededCapacity -=1; //transfer the capacity from teh needed to the available
            availableCapacity +=1;
        }

        mainsRequest = neededCapacity - availableCapacity;

        if (mainsRequest > 0 ) { //clamp the mains
            if (mainsRequest > 2) {
                mainsRequest = 2;
            }
            sources.requestMains(mainsRequest);
            availableCapacity += mainsRequest;  // Add mains power to the total
        }
    } else {
        sources.requestMains(0);
        sources.requestBattery(false); //If not in use disable battery
    }

    //Load calculations
    for (int loadCoumt = 0; loadCoumt < 3; loadCoumt++) {
        bool shouldTurnOn = false;

        // Decide if it should be running
        if(!loads.loadOverride[loadCoumt] && loads.currentLoadCall[loadCoumt]) {
            //Additional check for Load 0's time restriction:
            if(loadCoumt != 0 || (loadCoumt == 0 && (hourCount >= 8 && hourCount <= 22))) {
                // Check if there is sufficient power before turning on the load:
                if(availableCapacity >= loads.currentLoadCapacity[loadCoumt]) {
                    shouldTurnOn = true;
                    availableCapacity -= loads.currentLoadCapacity[loadCoumt];
                } else {
                    shouldTurnOn = false;  // Not enough capacity to turn on, overide it
                    loads.loadOverride[loadCoumt] = true;
                }
            } else {
                shouldTurnOn = false; // Outside of load0's time window.
            }
        } else {
            shouldTurnOn = false; // There is an overide or no load call. Leave it off
        }

        // switch the load on or off.
        if (shouldTurnOn) {
            loads.turnLoadOn(loadCoumt + 1);
        }
        else {
            loads.turnLoadOff(loadCoumt + 1);
            // reset the load override
            loads.loadOverride[loadCoumt] = false;
        }
    }

    //Charge battery if there is available surplus capacity and since we can only charge at 1A per hour
    //And check that only the renewable is used by adding the capacity to the cap buffer
    //Charge battery if there is available surplus capacity and we do not request for mains in load
    float availableRenewableEnergy = sources.totalRenewableCapacity - neededCapacity;
    if (availableRenewableEnergy >= 1) {
        sources.chargeBattery(true); //Charges it by 1A
    } else {
        sources.chargeBattery(false);
    }

    //This should check for the available load capacity and the renewables capacity. If not sufficient, it should call the battery.
    //ultimately if not available it should call the mains as a last option. -- This might be wrong as I should call teh mains if tehre is not enough available capacity and were trying to turn on a load prob
    // if (sources.loadDeficit<0) {
    //     if (sources.batteryCapacity > 0) {
    //         sources.requestBattery(true); // We can only discharge 1A per day
    //     }  else if (sources.batteryCapacity <= 0 ){
    //         sources.requestBattery(false);
    //         if (abs(sources.loadDeficit)>2) {
    //             sources.loadDeficit=2;
    //         }
    //         sources.requestMains(-1 * sources.loadDeficit); //Limit to available capacity
    //         availableCapacity += sources.mainsCapacity;
    //     }
    //     // break; // If the battery can sustain it break the if condition. Do not trust this cause im not sure if i should use break or continue. Will search later. - switched to elif
    // }
}

void updateInfluxDB() {
    uint32_t currentMillis = millis();
    if (currentMillis - lastEspUpdateUpdateMillis >= espUpdateInterval) {
        lastEspUpdateUpdateMillis = currentMillis;
        esp8266Handler.sendDataToWifi();
    }
}
//ADC ISR
ISR(ADC_vect){
    ADCraw = ADC;
    //ADCVoltage = (ADC * Vref) / 0x3FF; //Moved to main
    ADCConversionFlag = true;
}
//Counter ISR - Moved to timer0 using milis()
ISR(TIMER1_COMPA_vect) {
    // PORTC ^= (1 << PC7); //for testing - interrupt doesn't work with functions in it for some reason
    // Counter++;
    //utc++;
}
//Pin Change ISR
ISR(PCINT0_vect) {
    // Read Load Calls 1, 2 and 3
    // loads.checkLoadCallChanges(); //Moved to main
    LoadChangeFlag = true;
}

int main() {
    sei();   //Enable Global interrupts
    // debugSerial.println("Starting up...");
    finalizePorts();
    // debugSerial.println("Initialized ports");
    testLight(1);                              //Boot Light
    // debugSerial.println("Tested light PB6 and enabled global interrupts");
    //usbInit();
    display.startDisplay(false);
    // debugSerial.println("Turned on display");
    display.clearScreen();
    // debugSerial.println("Cleared screen");
    display.setBacklight(DisplayHandler::LIGHT);
    display.setOrientation(DisplayHandler::LANDSCAPE);
    // debugSerial.println("Initialized display and set to Landscape");
    //wifiHandler.connectToWiFi("\"Glen's XPS\"", "\"eesp8266\"");
    lastUtcUpdateMillis = millis();
    lastScreenUpdateMillis = millis();
    lastStatsUpdateMillis = millis();
    lastHourCheckUpdateMillis = millis();
    lastEspUpdateUpdateMillis = millis();
    hourChangeStartTime = millis();
    //Boot and Initialization
    // debugSerial.println("Drawing boot sequence");
    display.drawBootSequence();
    // debugSerial.println("Boot Complete...starting program");

    //Start of the LabView Algorighm
    //Moved LV algorightm to a seperate funcigton
    // Maximum battery capacity = 24Ah
    // 1 Hour Simulation time = 1 Min Runtime
    hourCount = 0, remainingHours = 24;
    // Default: Charge battery and turn all loads off, turn off batt req and batt chrg
    sources.requestMains(0);
    sources.chargeBattery(false);
    sources.requestBattery(false);
    loads.turnLoadOff(1);
    loads.turnLoadOff(2);
    loads.turnLoadOff(3);
    loads.checkLoadCallChanges();
    updateMainStats();

    // ReSharper disable once CppDFAEndlessLoop - CLion complains about forever while loop
    while (true) {
        drawTime();
        screenCarrousel();  //screen - variable to give to display.carrouselScreen() below
        display.carouselScreen(screen); //Screen - 1. screen (normal carrousel), others - BUSBAR_SCREEN, UART_SCREEN and so o
        updateMainStats();
        controlAlgrithm();
        //esp8266Handler.processSerialCommand();
        updateInfluxDB();
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