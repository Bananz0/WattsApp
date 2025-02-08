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
    // topLeft = {0,0};
    // bottomLeft = {0,240};
    // topRight = {320,0};
    // bottomRight = {320,240};


    bottomRight.X = 320; bottomRight.Y = 240;
    bottomLeft.X = 0; bottomLeft.Y = 240;
    topLeft.X = 0; topLeft.Y = 0;
    topRight.X = 320; topRight.Y = 0;

    top.X = 10; top.Y = 10;
    center.X = 10; center.Y = 85;
    bottom.X = 10; bottom.Y = 160;

    screenCenter.X = 160; screenCenter.Y = 120;

    titlePos.X = 10; titlePos.Y = 5;
    voltPos.X = 10; voltPos.Y = 90;
    currentPos.X = 10; currentPos.Y = 140;
    powerPos.X = 10; powerPos.Y = 190;

    teamPos = {10, 10};
    bootNamePos = {10, 160};
    bootVersionPos = {100, 200};

    sprintf(bootName, "WattsApp");
    sprintf(bootVersion, "v1.0.0");
    sprintf(bootAuthors, "TEAM\nL");

}

void DisplayHandler::drawArc() {
}

void DisplayHandler::drawSquircle() {
}

void DisplayHandler::showBusbarScreen() {

    sprintf(title, "BUSBAR\nSTATUS");
    sprintf(currentStatus, "Current: \n%00.2fA", (double)energyStats.busbarCurrent);
    sprintf(voltStatus, "Volatage: \n%00.2fV", (double)energyStats.busbarVoltage);
    sprintf(powerStatus, "Power: \n%0000.2fVA", (double)energyStats.busbarPower);

    pictorDrawS((unsigned char*)title ,titlePos, BLUE,backgroundColour,OryxB,5);
    pictorDrawS((unsigned char*)currentStatus,currentPos, BLUE,backgroundColour,OryxB,3);
    pictorDrawS((unsigned char*)voltStatus,voltPos, RED,backgroundColour,OryxB,3);
    pictorDrawS((unsigned char*)powerStatus,powerPos, DARK_GREEN,backgroundColour,OryxB,3);
}


void DisplayHandler::showPVScreen() {

}

void DisplayHandler::showTurbineScreen() {
}

void DisplayHandler::showBatteryScreen() {
}

DisplayHandler::~DisplayHandler() {
}

void DisplayHandler::startDisplay(bool vsync) {
    pictorInit(vsync);
    if (vsync)  pictorFrame();
    pictorCanvasSet(topLeft, bottomRight);
}
void DisplayHandler::stopDisplay() { //Should be used when turning off the Il Matto / Reset as it clears the screen contents
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

void DisplayHandler::drawBootLogo() {
    pictorDrawBox(topLeft,topRight,WHITE);
}

void DisplayHandler::drawBootSequence() {

    pictorDrawS(bootAuthors,teamPos,RED,BLACK, Mash,7);
    pictorDrawS(bootName,bootNamePos,GREEN,BLACK, Mash,4);
    pictorDrawS(bootVersion,bootVersionPos,BLUE,BLACK, Mash,4);

    _delay_ms(5000);
    clearScreen();
}
