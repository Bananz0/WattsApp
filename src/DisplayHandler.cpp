//
// Created by glenm on 2/4/2025.
//

#include "DisplayHandler.h"

DisplayHandler::DisplayHandler() : topLeft(), bottomRight(), bottomLeft(), topRight(),
                                   primaryColour(), secondaryColour(0x505A64),
                                   backgroundColour(0xFFFFFF),fontColour(0x1A1A1A),
                                   accentColour(0x006F7A), errorColour(0xD32F2F),
                                   successColour(0xCFD8DC), lineColour(0x263238), shapeColour(0x263238) {
    point topLeft = {0,0};
    point bottomLeft = {0,240};
    point topRight = {320,0};
    point bottomRight = {320,240};
}

DisplayHandler::~DisplayHandler() {
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
    pictorDrawAll(BLACK);
}
void DisplayHandler::drawText(char message[]) {
    point textPos = {50, 50};
    pictorDrawS((unsigned char*)message, textPos, fontColour, backgroundColour, Mash, 3);  // Draw the message with scale 1
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