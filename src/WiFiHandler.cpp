//
// Created by glenm on 2/10/2025.
//

#include "WiFiHandler.h"

WiFiHandler::WiFiHandler(HardwareSerial* serial, uint8_t enablePin) {
    this->espSerial = serial;
    this->enablePin = enablePin;
    this->connected = false;

    espSerial->begin(115200); //from arduino docs this initializes the hardware serial

    turnOnModule();
}


bool WiFiHandler::connectToWiFi(const char *ssid, const char *password) {\
    bool result = false;

    return result;
}

void WiFiHandler::disconnectFromWiFi() {

}

bool WiFiHandler::isConnected() {
    bool result = false;

    return result;
}

bool WiFiHandler::isModuleReady() {
    bool result = false;

    return result;
}

const char * WiFiHandler::getIPAddress() {
    const char* ipAddress = nullptr;
    //get ip from serial AT command

    ip.fromString(ipAddress);
    return ipAddress;
}

bool WiFiHandler::sendHTTP(const char *url, const char *payload) {
    bool result = false;
    return result;
}

bool WiFiHandler::sendToFirebase(const char *message) {
    bool result = false;
    return result;
}

bool WiFiHandler::sendToInfluxDB(const char *message) {
    bool result = false;
    return result;
}

void WiFiHandler::turnOnModule() {
    PORTA |= (1 << PORTA4); //PA4 to high
}

void WiFiHandler::turnOffModule() {
    PORTA &= ~(1 << PORTA4); //PA4 to low
}

void WiFiHandler::resetModule() {
    turnOffModule();
    _delay_ms(100);
    turnOnModule();
}

void WiFiHandler::sendATCommand(const char *command) {
}

void WiFiHandler::waitForResponse() {
}
