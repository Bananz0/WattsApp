//
// Created by glenm on 3/8/2025.
//

#include "ESP8266Handler.h"

#include <Arduino.h>
#include <WiFiEsp/WiFiEsp.h>

#include "WiFiHandler.h"

ESP8266Handler::ESP8266Handler(Loads *loads, Sources *sources): loads(loads), sources(sources) {
    enableESP(); //Set the EN pin to High (dont remember what it was but will recheck the pin map;

    HardwareSerial *wifiSerial = &Serial;
    wifiSerial->begin(115200);
    wifiSerial->println("ATMEGA_WIFI_SETUP_COMPLETE");




}

ESP8266Handler::~ESP8266Handler() = default;

void ESP8266Handler::enableESP() {
    //Set PA4 to High
    //Unnecessary but will use the WifiHandler code i wrote a whie back
    WiFiHandler::turnOnModule();
}

void ESP8266Handler::sendDataToWifi() {
    char dataString[200];
    snprintf(dataString, sizeof(dataString),
             "%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%.2f,%.2f,%d,%d,%d\n", //Format teh sring so it is parseable by the ESP
             sources->windTurbineCapacity, sources->pvCapacity,
             sources->busbarVoltage, sources->busbarCurrent,
             sources->totalRenewableCapacity, sources->batteryCapacity,
             sources->mainsCapacity, dayCount, loads->totalLoadCapacity, sources->loadDeficit,
             remainingDays, utc,
             loads->currentLoad1);

    Serial.print(dataString); // Send over serial

    //Debug output to serial monitor
    //Serial.print("Sent to Wifi: ");
    //Serial.println(dataString);
}