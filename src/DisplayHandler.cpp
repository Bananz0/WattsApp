//
// Created by glenm on 2/4/2025.
//

#include "DisplayHandler.h"
#include "globalVariables.h"

#include <stdio.h>
#include <string.h>
#include "../assets/stripe_assets.h"

///Will implement carousel for switching between the different screens later TODO

DisplayHandler::DisplayHandler() : topLeft(), bottomRight(), bottomLeft(), topRight(), top(), center(), bottom(),
                                   primaryColour(WHITE), secondaryColour(BLUE),
                                   backgroundColour(BLACK),fontColour(DARK GRAY),
                                   accentColour(ORANGE), errorColour(BRIGHT RED),
                                   successColour(LIGHT_GREEN), lineColour(BROWN), shapeColour(GRAY)
                                    {
    topLeft = {0,0};
    bottomLeft = {0,240};
    topRight = {320,0};
    bottomRight = {320,240};


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
    sprintf(bootAuthors, "TEAM\n L");

    screenPage = BUSBAR_SCREEN;
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
        default:
            showErrorScreen();
        break;
    }
}


void DisplayHandler::drawBootSequence() {
    pictorDrawS(reinterpret_cast<const unsigned char *>(bootAuthors),teamPos,RED,BLACK, Mash,7);
    pictorDrawS(reinterpret_cast<const unsigned char *>(bootName),bootNamePos,GREEN,BLACK, Mash,4);
    pictorDrawS(reinterpret_cast<const unsigned char *>(bootVersion),bootVersionPos,BLUE,BLACK, Mash,4);

    _delay_ms(5000);
    clearScreen();
}

void DisplayHandler::showBusbarScreen() {
    sprintf(title, "BUSBAR\nSTATUS");
    sprintf(text1, "Current: \n%00.2f A", (double)energyStats.busbarCurrent);
    sprintf(text2, "Volatage: \n%00.2f V", (double)energyStats.busbarVoltage);
    sprintf(text3, "Power: \n%0000.2f VA", (double)energyStats.busbarPower);

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),currentPos, BLUE,backgroundColour,OryxB,3);
    pictorDrawS(reinterpret_cast<unsigned char *>(text2),voltPos, RED,backgroundColour,OryxB,3);
    pictorDrawS(reinterpret_cast<unsigned char *>(text3),powerPos, DARK_GREEN,backgroundColour,OryxB,3);
}

void DisplayHandler::showPVScreen() {
    sprintf(title, "SOLAR\nSTATUS");
    sprintf(text1, "Solar\nCapacity: \n%00.2f A", (double)energyStats.busbarCurrent);
    //sprintf(text2, "Volatage: \n%00.2fV", (double)energyStats.busbarVoltage);
    //sprintf(text3, "Power: \n%0000.2fVA", (double)energyStats.busbarPower);

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),solarCapacityPos, YELLOW,backgroundColour,OryxB,3);

    //pictorDrawS(reinterpret_cast<unsigned char *>(text2),voltPos, RED,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text3),powerPos, DARK_GREEN,backgroundColour,OryxB,3);
}

void DisplayHandler::showTurbineScreen() {
    sprintf(title, "TURBINE\nSTATUS");
    sprintf(text1, "Turbine\nCapacity: \n%00.2f A", (double)energyStats.busbarCurrent);
    //sprintf(text2, "Volatage: \n%00.2fV", (double)energyStats.busbarVoltage);
    //sprintf(text3, "Power: \n%0000.2fVA", (double)energyStats.busbarPower);

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),currentPos, YELLOW,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text2),voltPos, RED,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text3),powerPos, DARK_GREEN,backgroundColour,OryxB,3);
}

void DisplayHandler::showBatteryScreen() {
    sprintf(title, "BATTERY\nSTATUS");
    sprintf(text1, "Current: \n%00.2f AH", (double)energyStats.busbarCurrent);
    //sprintf(text2, "Volatage: \n%00.2fV", (double)energyStats.busbarVoltage);
    //sprintf(text3, "Power: \n%0000.2fVA", (double)energyStats.busbarPower);

    pictorDrawS(reinterpret_cast<unsigned char *>(title) ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS(reinterpret_cast<unsigned char *>(text1),currentPos, YELLOW,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text2),voltPos, RED,backgroundColour,OryxB,3);
    //pictorDrawS(reinterpret_cast<unsigned char *>(text3),powerPos, DARK_GREEN,backgroundColour,OryxB,3);
}

void DisplayHandler::showErrorScreen() {

}

void DisplayHandler::showLoadsScreen() {

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
    pictorDrawAll(BLACK);
}

void DisplayHandler::screenOn() {  //Waking from Sleep
    setBacklight(LIGHT);
}

void DisplayHandler::screenOff() {
    setBacklight(DIM); //Dim and turn off backlight
}

void DisplayHandler::drawText(char message[]) {
    //Will expand to handle any char string decimal or number
    point textPos = {50, 50};
    pictorDrawS((unsigned char*)message, textPos, fontColour, backgroundColour, Mash, 2);  // Draw the message with scale 1
}

void DisplayHandler::drawUIsimple() {


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

void DisplayHandler::drawArc() {
}

void DisplayHandler::drawSquircle() {
}

void DisplayHandler::drawBootLogo() {

}