//
// Created by glenm on 2/10/2025.
//

#include "WiFiHandler.h"

WiFiHandler::WiFiHandler(HardwareSerial* serial, uint8_t enablePin) {
    this->espSerial = serial;
    this->enablePin = enablePin;
    this->connected = false;

    espSerial->begin(115200); //from arduino docs this initializes the hardware serial
    espSerial->println("WiFiHandler initialized");

    turnOnModule();

    isModuleReady();

    sendATCommand("AT+CWMODE=1", "OK");  //Station Mode
}


bool WiFiHandler::connectToWiFi(const char *ssid, const char *password) {
    String ssidStr = ssid,passwordStr = password, command = "AT+CWJAP_DEF=";
    ssidStr = "\"" + ssidStr + "\"";
    passwordStr = "\"" + passwordStr + "\"";
    sendATCommand(command+","+ssidStr+","+passwordStr, {"OK"});
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
    bool ready = false;
    String readyString;
    // while  (!ready) {
    //     delay(500);
    //     readyString = sendATCommand("AT", "OK");
    //     if (readyString == "OK") {
    //         ready = true;
    //     }
    // }
    return ready;
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
    for (uint8_t attempt = 0; attempt < 3; attempt++) {
        espSerial->println(command);
        _delay_ms(100);
        response = waitForResponse();
        if (response == expectedResponse) {
            break; // Stop if we get the expected response
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
