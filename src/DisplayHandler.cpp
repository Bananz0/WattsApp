//
// Created by glenm on 2/4/2025.
//

#include "DisplayHandler.h"
#include "globalVariables.h"

#include <stdio.h>
//#include "../assets/fan.h"

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

}

void DisplayHandler::drawArc() {
}

void DisplayHandler::drawSquircle() {
}

void DisplayHandler::showBusbarScreen() {
    char text[40];
    char text2[40];
    char text3[40];

    sprintf(text, "BUSBAR \nCurrent: \n%.2fA", (double)energyStats.busbarCurrent);
    sprintf(text2, "BUSBAR \nVolatage: \n%.2fV", (double)energyStats.busbarVoltage);
    sprintf(text3, "BUSBAR \nPower: \n%.2fVA", (double)energyStats.busbarPower);

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
    unsigned char bootName[] = "WattsApp";
    unsigned char bootVersion[] = "v1.0.0";
    unsigned char bootAuthors[] = "TEAM L";

    pictorDrawBox(topLeft,bottomRight,BLACK);

    point teamPos = {10, 10};
    point bootNamePos = {10, 80};
    point bootVersionPos = {100, 200};

    pictorDrawS(bootAuthors,teamPos,WHITE,RED, Mash,6);
    pictorDrawS(bootName,bootNamePos,WHITE,RED, Mash,4);
    pictorDrawS(bootVersion,bootVersionPos,WHITE,RED, Mash,4);

    _delay_ms(5000);
    clearScreen();
}
