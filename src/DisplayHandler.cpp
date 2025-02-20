//
// Created by glenm on 2/4/2025.
//

#include "DisplayHandler.h"
#include <HardwareSerial.h>
#include "Sources.h"
#include "Loads.h"
#include "globalVariables.h"
#include "../assets/stripe_assets.h"

///Will implement carousel for switching between the different screens later TODO

DisplayHandler::DisplayHandler(Loads *loads, Sources *sources) : primaryColour(WHITE),
                                                                 secondaryColour(BLUE),
                                                                 backgroundColour(BLACK),
                                                                 fontColour(DARK GRAY),
                                                                 accentColour(ORANGE),
                                                                 errorColour(BRIGHT RED),
                                                                 successColour(LIGHT_GREEN),
                                                        lineColour(BROWN), shapeColour(GRAY),
                                   topLeft({0,0}),bottomRight({320,240}),
                                   bottomLeft({0,240}), topRight({320,0}),
                                   top(), center(), bottom(),
                                   sources(sources), loads(loads)
                                    {

    // bottomRight.X = 320; bottomRight.Y = 240;
    // bottomLeft.X = 0; bottomLeft.Y = 240;
    // topLeft.X = 0; topLeft.Y = 0;
    // topRight.X = 320; topRight.Y = 0;

    top.X = 10; top.Y = 10;
    center.X = 10; center.Y = 85;
    bottom.X = 10; bottom.Y = 160;

    screenCenter.X = 160; screenCenter.Y = 120;

    //Common Settings
    titlePos.X = 10; titlePos.Y = 5;
    timePos = {205,230};
    //Busbar Settings
    voltPos.X = 10; voltPos.Y = 90;
    currentPos.X = 10; currentPos.Y = 140;
    powerPos.X = 10; powerPos.Y = 190;
    //PV Settings
    solarCapacityPos.X = 10; solarCapacityPos.Y = 150;
    //Turbine Settings
    turbineCapacityPos = {10,190};



    teamPos = {10, 10};
    bootNamePos = {10, 160};
    bootVersionPos = {10, 200};

    sprintf(bootName, "WattsApp");
    sprintf(bootVersion, "v1.0.0");
    sprintf(bootAuthors, "TEAM\nL");

    fanPos = {250 , 140} ;
    batteryCapacityPos = {250,140};

    load1Pos = {0 , 140};
    load2Pos = {105 , 140};
    load3Pos = {211 , 140};
}

void DisplayHandler::carouselScreen(Screen screen) { //May move away from this
    prevScreenPage = screenPage;
    if (prevScreenPage != screen) {
        clearScreen();
    }
    nextScreenPage = screen;
    screenPage = nextScreenPage;
    switch (screenPage) {
        case PV_SCREEN:
            showPVScreen();
        break;
        case LOADS_SCREEN:
            showLoadsScreen();
        break;
        case BUSBAR_SCREEN:
            showBusbarScreen();
        break;
        case BATTERY_SCREEN:
            showBatteryScreen();
        break;
        case TURBINE_SCREEN:
            showTurbineScreen();
        break;
        case SEC_REV_SCREEN:
            secondReviewScreen();
        break;
        case ERROR_SCREEN:
            showErrorScreen();
        break;
        case UART_SCREEN:
            showUARTScreen();
        break;
        case DAY_SCREEN:
            showDayCountScreen();
        break;
        default:
        break;
    }
}


void DisplayHandler::drawBootSequence() {
#ifdef NORMAL_MODE
    pictorDrawS(reinterpret_cast<const unsigned char *>(bootAuthors),teamPos,RED,BLACK, Mash,7);
    pictorDrawS(reinterpret_cast<const unsigned char *>(bootName),bootNamePos,GREEN,BLACK, Mash,4);
    pictorDrawS(reinterpret_cast<const unsigned char *>(bootVersion),bootVersionPos,BLUE,BLACK, Mash,4);

    _delay_ms(3500);
    clearScreen();
#endif
}

void DisplayHandler::showBusbarScreen() {
    sprintf(title, "BUSBAR\nSTATUS");
    sprintf(text1, "Current: \n%0.2f A", static_cast<double>(sources->busbarCurrent));
    sprintf(text2, "Volatage: \n%0.2f V", static_cast<double>(sources->busbarVoltage));
    sprintf(text3, "Power: \n%0.2f VA", static_cast<double>(sources->busbarPower));

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),currentPos, BLUE,backgroundColour,OryxB,3);
    pictorDrawS(reinterpret_cast<unsigned char *>(text2),voltPos, RED,backgroundColour,OryxB,3);
    pictorDrawS(reinterpret_cast<unsigned char *>(text3),powerPos, DARK_GREEN,backgroundColour,OryxB,3);
}

void DisplayHandler::showPVScreen() {
    sprintf(title, "SOLAR\nSTATUS");
    sprintf(text1, "Solar\nCapacity: \n%0.2f A", (double)sources->pvCapacity);
    //sprintf(text2, "Volatage: \n%00.2fV", (double)sources->busbarVoltage);
    //sprintf(text3, "Power: \n%0000.2fVA", (double)sources->busbarPower);

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),solarCapacityPos, YELLOW,backgroundColour,OryxB,3);

    //pictorDrawS(reinterpret_cast<unsigned char *>(text2),voltPos, RED,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text3),powerPos, DARK_GREEN,backgroundColour,OryxB,3);
}

void DisplayHandler::showTurbineScreen() {
    sprintf(title, "TURBINE\nSTATUS");
    sprintf(text1, "Turbine\nCapacity: \n%0.2f A", (double)sources->windTurbineCapacity);
    //sprintf(text2, "Volatage: \n%00.2fV", (double)sources->busbarVoltage);
    //sprintf(text3, "Power: \n%0000.2fVA", (double)sources->busbarPower);

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),currentPos, YELLOW,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text2),voltPos, RED,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text3),powerPos, DARK_GREEN,backgroundColour,OryxB,3);

    drawUIfan();
}

void DisplayHandler::showBatteryScreen() {
    sprintf(title, "BATTERY\nSTATUS");
    sprintf(text1, "Current: \n%0.2f AH", (double)sources->batteryCapacity);
    //sprintf(text2, "Volatage: \n%00.2fV", (double)sources->busbarVoltage);
    //sprintf(text3, "Power: \n%0000.2fVA", (double)sources->busbarPower);

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),currentPos, YELLOW,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text2),voltPos, RED,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text3),powerPos, DARK_GREEN,backgroundColour,OryxB,3);

    drawUIbattery();
}

void DisplayHandler::showErrorScreen() {
    strncpy(tempMessage, const_cast<char *>(emergencyMessage), sizeof(tempMessage)); //Ignore error - compiles
    //memcpy(const_cast<char *>(emergencyMessage), tempMessage, sizeof(tempMessage));;

    sprintf(title, "ERROR\nSTATUS");
    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, RED,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<const unsigned char *>(tempMessage),currentPos,  YELLOW,backgroundColour,OryxB,3);
    //TODO: Exclamation mark sprite
}

void DisplayHandler::showDayCountScreen() {
    strncpy(tempMessage, const_cast<char *>(emergencyMessage), sizeof(tempMessage)); //Ignore error - compiles

    sprintf(title, "DAY\nSTATUS");
    sprintf(text1, "Current: \n%0.2u Days", dayCount);

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),currentPos, YELLOW,backgroundColour,OryxB,3);
    //TODO: Exclamation mark sprite
}

void DisplayHandler::showLoadsScreen() {
    sprintf(title, "LOADS\nSTATUS");
    //TODO: implement load status

    sprintf(text1, "Load\n  1\n %s", loads->currentLoad1Call? "ON" : "OFF");
    sprintf(text2, "Load\n  2\n %s", loads->currentLoad2Call? "ON" : "OFF");
    sprintf(text3, "Load\n  3\n %s", loads->currentLoad3Call? "ON" : "OFF");

    pictorDrawS(reinterpret_cast<unsigned char *>(title),titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),load1Pos, loads->currentLoad1Call?  GREEN: RED,backgroundColour,OryxB,3);
    pictorDrawS(reinterpret_cast<unsigned char *>(text2),load2Pos, loads->currentLoad2Call?  GREEN: RED,backgroundColour,OryxB,3);
    pictorDrawS(reinterpret_cast<unsigned char *>(text3),load3Pos, loads->currentLoad3Call?  GREEN: RED,backgroundColour,OryxB,3);
}

void DisplayHandler::showUARTScreen() {
    sprintf(title, "UART\nDEBUG");
    sprintf(tempMessage, "UART Received:\n%s",const_cast<char *>(uartMessage));

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, ORANGE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(tempMessage),currentPos,  WHITE,backgroundColour,OryxB,2);
}

void DisplayHandler::secondReviewScreen() {
    sprintf(title, "TEAM\n    \n L  ");
    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLACK,WHITE,OryxB,9);
}

DisplayHandler::~DisplayHandler() = default;

void DisplayHandler::startDisplay(bool vsync) {
    pictorInit(vsync);
    if (vsync)  pictorFrame();
    pictorCanvasSet(topLeft, bottomRight);
}

void DisplayHandler::stopDisplay() { //Should be used when turning off the Il Matto/Reset as it clears the screen contents
    clearScreen();
    screenOff();

}

void DisplayHandler::setBacklight(const state State) {
    pictorBacklightState(State);
}

void DisplayHandler::toggleBacklight() {
    pictorBacklightState(-1);
}

void DisplayHandler::setOrientation(const orientation rotation) {
    pictorSetRotation(rotation);
}

void DisplayHandler::clearScreen() {
    //set screen to black
    pictorDrawAll(BLACK);
}

void DisplayHandler::screenOn() {  //Waking from Sleep
    setBacklight(LIGHT);
}

void DisplayHandler::screenOff() {
    setBacklight(DIM); //Dim and turn off backlight
}

void DisplayHandler::drawText(char text[]) {
    //Will expand to handle any char string decimal or number
    point textPos = {50, 50};
    pictorDrawS(reinterpret_cast<unsigned char *>(text), textPos, fontColour, backgroundColour, Mash, 2);  // Draw the message with scale 1
}

void DisplayHandler::drawUIbattery() {
    if (sources->isBatteryCharging) {
        pictorDrawSpriteType(&charging7, batteryCapacityPos,7,2);
    }
    if (!sources->isBatteryCharging) {
        switch (sources->batteryCapacity/6) { //Since we have 24Ah, scale it for to the four available cases.
            case 0:
                pictorDrawSpriteType(&batt207, batteryCapacityPos,7,2);
            break;
            case 1:
                pictorDrawSpriteType(&batt407, batteryCapacityPos,7,2);
            break;
            case 2:
                pictorDrawSpriteType(&batt607, batteryCapacityPos,7,2);
            break;
            case 3:
                pictorDrawSpriteType(&batt807, batteryCapacityPos,7,2);
            break;
            case 4:
                pictorDrawSpriteType(&batt1007, batteryCapacityPos,7,2);
            break;
        }
    }
}

void DisplayHandler::drawUIfan() {
    pictorDrawSpriteType(&fan7, fanPos,7,2);
}

void DisplayHandler::drawBitmap() {
}

void DisplayHandler::drawPicture() {
}


void DisplayHandler::drawBootLogo() {

}