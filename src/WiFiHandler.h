//
// Created by glenm on 2/10/2025.
//

#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <stdint.h>
#include "HardwareSerial.h"

class WiFiHandler {
public:
    WiFiHandler(); //Initialize the esp8266 with the baud and everyghing else.
    ~WiFiHandler();
    bool connectToWiFi(const char* ssid, const char* password); //Connects to other universe
    void disconnectFromWiFi();
    bool isConnected();
    bool sendToFireBase(const char* message);

    void turnOnModule(); //Set the EN pin to 3.3v
    void turnOffModule(); //Set the EN pin to 0.0025v
    void resetModule(); //Have you tried turning it off and on again

private:
    uint8_t enablePin;
    bool connected;
    void sendATCommand(const char* command);
    void waitForResponse();
    HardwareSerial* espSerial;

};



#endif //WIFIHANDLER_H
