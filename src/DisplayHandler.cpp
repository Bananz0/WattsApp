//
// Created by glenm on 2/4/2025.
//

#include "DisplayHandler.h"

DisplayHandler::DisplayHandler() : topLeft(), bottomRight(), bottomLeft(), topRight() {

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
// void DisplayHandler::setBacklight(state State) {
//     pictorBacklightState(State);
// }
void DisplayHandler::toggleBacklight() {
    pictorBacklightState(-1);
}
void DisplayHandler::setOrientation(orientation rotation) {
    pictorSetRotation(rotation);
}
void DisplayHandler::clearScreen() {
    //set screen to black
}
void DisplayHandler::drawText(char text[]) {

}
void DisplayHandler::drawUIsimple() {
    uint16_t lineColor = encode656Colour(0, 255, 0);  // Green color for the line
    point start = {10, 200};
    point end = {200, 200};
    pictorDrawLine(start, end, lineColor);  // Draw a green line
    pictorDrawBox(topLeft, bottomRight, lineColor);
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