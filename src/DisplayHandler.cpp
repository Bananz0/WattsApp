//
// Created by glenm on 2/4/2025.
//

#include "DisplayHandler.h"
#include "globalVariables.h"

#include <stdio.h>

///Will implement carousel for switching between the different screens later TODO

DisplayHandler::DisplayHandler() : topLeft(), bottomRight(), bottomLeft(), topRight(), top(), center(), bottom(),
                                   primaryColour(WHITE), secondaryColour(BLUE),
                                   backgroundColour(BLACK),fontColour(0x1A1A1A),
                                   accentColour(ORANGE), errorColour(0xD32F2F),
                                   successColour(0xCFD8DC), lineColour(0x263238), shapeColour(0x263238)
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
}

void DisplayHandler::drawArc() {
}

void DisplayHandler::drawSquircle() {
}

void DisplayHandler::showBusbarScreen() {
    char text[40];
    char text2[40];
    char text3[40];

    sprintf(text, "BUSBAR \nCurrent: \n%.2fA", energyStats.busbarCurrent);
    sprintf(text2, "BUSBAR \nVolatage: \n%.2fV", energyStats.busbarVoltage);
    sprintf(text3, "BUSBAR \nPower: \n%.2fVA", energyStats.busbarPower);

    pictorDrawS((unsigned char*)text,top, BLUE,backgroundColour,OryxB,3);
    pictorDrawS((unsigned char*)text2,center, RED,backgroundColour,OryxB,3);
    pictorDrawS((unsigned char*)text3,bottom, DARK_GREEN,backgroundColour,OryxB,3);
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
    point start = {10, 200};
    point end = {200, 200};
    pictorDrawLine(start, end, lineColour);  // Draw a green line
    pictorDrawBox(topLeft, bottomRight, lineColour);
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