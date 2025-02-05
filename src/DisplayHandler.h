//
// Created by glenm on 2/4/2025.
//

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H
#include <stdint.h>


class DisplayHandler {
public:
    typedef enum {PORTRAIT = 0, LANDSCAPE = 1, PORTRAIT_INV = 2, LANDSCAPE_INV = 3} orientation;

    DisplayHandler();
    ~DisplayHandler();
    void startDisplay(bool vsync);
    void stopDisplay();
 //   void setBacklight(state State);
    void toggleBacklight();
    void setOrientation(orientation rotation);
    void clearScreen();

    //Drawing Elements
    void drawRectangle();
    void drawCircle();
    void drawTriangle();
    void drawEllipse();


    //Text elements
    void drawText(char text[]);
    void drawBitmap();
    void drawPicture();

    //UI Elements (hopefully)
    void drawUIsimple();
    void drawBootLogo();


private:
    point topLeft, bottomRight, bottomLeft, topRight;
};




#endif //DISPLAYHANDLER_H
