//
// Created by glenm on 2/4/2025.
//

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H
#include <globalVariables.h>
#include <Loads.h>
#include <Sources.h>
#include <stdint.h>
#include <pictor/pictor.h>
#include "Mash.h"
#include "OryxB.h"

class DisplayHandler {
public:
    typedef enum {PORTRAIT = 0, LANDSCAPE = 1, PORTRAIT_INV = 2, LANDSCAPE_INV = 3} orientation;
    typedef enum {LIGHT = 1, DIM = 0}state;

    point timePos{};
    //Sprite Pos
    point fanPos{};
    point load1Pos{}, load2Pos{}, load3Pos{}, call1Pos{}, call2Pos{}, call3Pos{};
    point busbarSPose{};
    point pvSpos{}; //PV Sprite Pos

    uint16_t primaryColour, secondaryColour, backgroundColour, fontColour,
             accentColour, errorColour,successColour, lineColour, shapeColour;

    DisplayHandler(Loads *loads, Sources *sources);
    ~DisplayHandler();
    void startDisplay(bool vsync);
    void stopDisplay();
    void setBacklight(state State);
    void toggleBacklight();
    void setOrientation(orientation rotation);
    void clearScreen();
    void screenOn();
    void screenOff();

    //Text elements
    void drawText(char text[]);
    void drawBitmap();
    void drawPicture();

    //UI Elements (hopefully)
    void drawUIbattery();
    void drawUIfan();
    void drawBootLogo();
    void drawBootSequence();

    //Pages maybe?
    void showBusbarScreen();
    void showPVScreen();
    void showTurbineScreen();
    void showBatteryScreen();
    void showLoadsScreen();

    void showUARTScreen();

    void secondReviewScreen();

    void showErrorScreen();

    void showDayCountScreen();

    void carouselScreen(Screen screen);

private:
    point topLeft, bottomRight, bottomLeft, topRight, top ,center ,bottom, screenCenter{}
    ,currentPos{}, voltPos{}, powerPos{}, titlePos{}, teamPos{} ,bootNamePos{} ,bootVersionPos{},
    solarCapacityPos{}, batteryCapacityPos{},turbineCapacityPos{};

    char bootName[10]{},bootVersion[10]{}, bootAuthors[10]{};
    char title[40]{}, text2[40]{}, text1[40]{}, batteryStatus[40]{}, text3[40]{}, tempMessage[40]{}, text4[40]{}, text5[40]{}, text6[40]{}, text7[40]{};

    Sources *sources;
    Loads *loads;
};




#endif //DISPLAYHANDLER_H
