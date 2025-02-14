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

    sendATCommand("AT+CWMODE=1", {});  //Station Mode
}


bool WiFiHandler::connectToWiFi(const char *ssid, const char *password) {
    String ssidStr = ssid,passwordStr = password, command = "AT+CWJAP_DEF=";
    sendATCommand(command+","+ssid+","+password, {"OK"});
}

void WiFiHandler::disconnectFromWiFi() {
    sendATCommand("AT+CWQAP", {});
    if (waitForResponse() == "OK") {
        connected = false;
    }
}

bool WiFiHandler::isConnected() const {
    return connected;
}

bool WiFiHandler::isModuleReady() {
    sendATCommand("AT", {});
    if (waitForResponse() == "OK") {
        return true;
    }
    return false;
}

const char * WiFiHandler::getIPAddress() {
    const char* ipAddress = nullptr;
    //get ip from serial AT command
    sendATCommand("AT+ CIFSR", {}); //TODO: not fully implemented
    if (waitForResponse() == "OK") {

    }

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

void WiFiHandler::restartModule() {
    sendATCommand("AT+RST", {});
}

void WiFiHandler::resetModule() {
    turnOffModule();
    _delay_ms(100);
    turnOnModule();
}

void WiFiHandler::sleepMode(uint8_t mode) {
    //Mode 1: Disable sleep
    //Mode 2: Light Sleep
    //Mode 3: Deep Sleep mode
    String command = "AT+SLEEP=" +  mode;
    // switch (mode) {
    //     case 1:
    //         sendATCommand("AT+SLEEP");
    //     break;
    //     case 2:
    //         sendATCommand("AT+RST");
    //     break;
    //     case 3:
    //         sendATCommand("AT+SLEEP");
    //     break;
    // }
}

const char *WiFiHandler::sendATCommand(const String &command, const char *expectedResponse){
    String response{}; //Should store the response from Serial
    if(Serial1.available()) {
        espSerial->println(command);
        //Retries and timeout maybe
        response = waitForResponse();
        if (response.c_str() != expectedResponse) {
            for (uint8_t i = 0; i < 20; i++) { //Three retries
                espSerial->println(command);
            }
        }
    }
    return response.c_str();
}

String WiFiHandler::waitForResponse() {
    String response;
    if(Serial1.available()) {
        response = Serial1.readStringUntil('\r');
    }
    return response;
}
