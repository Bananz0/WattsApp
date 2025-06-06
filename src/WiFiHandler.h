//
// Created by glenm on 2/10/2025.
//

#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <IPAddress.h>
#include <stdint.h>
#include "HardwareSerial.h"
#include <util/delay.h>
#include "avr/io.h"

class WiFiHandler {
public:
    WiFiHandler(HardwareSerial *serial, uint8_t enablePin);
    bool connectToWiFi(const char *ssid, const char *password);
    void disconnectFromWiFi();

    bool isConnected() const;

    void echoSerial();

    bool isModuleReady();

    const char* getIPAddress();

    bool sendHTTP(const char* url, const char* payload);

    bool sendToFirebase(const char* message);
    bool sendToInfluxDB(const char* message);

static    void turnOnModule();  // EN pin to 3.3v
static    void turnOffModule(); // EN low
    void restartModule(); // soft restart the wifi module
static    void resetModule();   //reset the module but wont implement because not enough pins

    void sleepMode(uint8_t mode);

private:
    uint8_t enablePin{8};
    bool connected{false};
    HardwareSerial* espSerial = espSerial;
    IPAddress ip;

    String sendATCommand(const String &command, const char *expectedResponse);
    String waitForResponse();

    static bool checkResponse(const String &received, const char *expectedResponse);

    void printWifiData();

    void printCurrentNet();
};

#endif //WIFIHANDLER_H
