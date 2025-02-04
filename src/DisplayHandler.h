//
// Created by glenm on 2/4/2025.
//

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H
#include <stdint.h>


class DisplayHandler {
public:
    typedef enum {PORTRAIT = 0, LANDSCAPE = 1, PORTRAIT_INV = 2, LANDSCAPE_INV = 3} orientation;
    typedef enum {ON = 1, OFF = 0} state;

    DisplayHandler();
    ~DisplayHandler();
    static void startDisplay(bool vsync);
    static void stopDisplay();
    static void setBacklight(state State);
    static void setOrientation(orientation rotation);
    static void clearScreen();

private:


};




#endif //DISPLAYHANDLER_H
