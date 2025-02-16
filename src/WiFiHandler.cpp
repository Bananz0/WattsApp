//
// Created by glenm on 2/10/2025.
//

#include "WiFiHandler.h"

#include <Arduino.h>
#include <time.h>

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
    const String ssidStr = ssid;
    const String passwordStr = password;
    const String command = "AT+CWJAP_DEF=";
    // ssidStr = "\"" + ssidStr + "\"";
    // passwordStr = "\"" + passwordStr + "\""; // did this in the command's arguments.
    sendATCommand(command+","+ssidStr+","+passwordStr, {"OK"});
    return true;
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
    // bool ready = false;
    // String readyString;
    // while  (!ready) {
    //     delay(500);
    //     readyString = sendATCommand("AT", "OK");
    //     if (readyString == "OK") {
    //         ready = true;
    //     }
    // }
    bool ready = sendATCommand("AT", "OK");
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

// String WiFiHandler::sendATCommand(const String &command, const char *expectedResponse) {
//     String response;
//
//         espSerial->println(command);
//         response = waitForResponse();
//         //     if (response.indexOf(expectedResponse) != -1) {
//         //         return true;  // Found expected response
//         //     }
//         // }
//         // return false;  // Failed after 3 attempts
//
//         return response;
// }

String WiFiHandler::sendATCommand(const String &command, const char *expectedResponse) {
    String response;
    for (uint8_t attempt = 0; attempt < 3; attempt++) {
        // Clear any leftover data
        while (espSerial->available()) {
            espSerial->read();
        }
        espSerial->println(command);
        response = waitForResponse();
        if (response.length() > 0) {
            break;  // Got a response, no need to retry
        }
        _delay_ms(100);  // Wait before retry
    }
    return response;
}

// String WiFiHandler::waitForResponse() {
//     uint16_t timeout = 500;
//     String response;
//     espSerial->setTimeout(timeout);  // Set timeout test
//     unsigned long startTime = millis();
//     while (!espSerial->available() && (millis() - startTime < timeout)) { //wait for timeout
//     }
//     if (espSerial->available()) {
//         response = espSerial->readStringUntil('\r');
//         //response = espSerial->readString(); // THIS DOESNT WORK AND I AM NOT SMART ENOUGH TO FIGYURE IT OUT
//     }
//     return response;
// }

String WiFiHandler::waitForResponse() {
    uint16_t timeout = 500;
    String response;
    char c;
    unsigned long startTime = millis();

    while ((millis() - startTime) < timeout) {
        if (espSerial->available()) {
            c = espSerial->read();
            response += c;

            // Check for common AT command endings
            if (response.endsWith("\r\n") || response.endsWith("OK\r\n") || response.endsWith("OK")) {
                break;
            }
        }
        _delay_us(100);  // Small delay to prevent tight polling
    }
    return response;
}

bool WiFiHandler::checkResponse(const String &received, const char *expectedResponse) {
    if (received.length() == 0 || expectedResponse == nullptr) {
        return false;
    }
    return (received.indexOf(expectedResponse) != -1);
}

