//
// Created by glenm on 3/8/2025.
//

#include "ESP8266Handler.h"

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
    char dataString[100];
    snprintf(dataString, sizeof(dataString),
             "%.2f,%.2f,%.2f,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.2f,%.2f,%.f\n", //Format teh sring so it is parseable by the ESP
             //Renewable Stats
             sources->windTurbineCapacity, //Float
             sources->pvCapacity,//Float
             sources->totalRenewableCapacity,//Float
             //Battery Stores
             sources->batteryCapacity,//Float
             //Final Source
             sources->mainsCapacity,//Float
             //Busbar stuff
             sources->busbarVoltage,//Float
             sources->busbarCurrent,//Float
             sources->averagePower,//Float
             sources->totalEnergy,//Float
             //Load Statuses
             //Current Load Statuses
             loads->currentLoadStatus[0],
             loads->currentLoadStatus[1],
             loads->currentLoadStatus[2],
             //Current Requests
             loads->currentLoad1Call,
             loads->currentLoad2Call,
             loads->currentLoad3Call,
             //Load Overicdes for when we can't supply enough power -definitiojn of over engineering.
             loads->loadOverride1,
             loads->loadOverride2,
             loads->loadOverride3,
             //TotalCapacityStuff
             sources->loadDeficit,
             loads->totalLoadCapacity,
             //Day Statuses - Twice cause fuck it we ball (parity)
             dayCount
             //timeUTC->tm_mday
             );

    //Calculate chdcksum shere
    uint8_t checksum = 0;
    for (int i = 0; dataString[i] != '\0'; i++) {
        checksum ^= dataString[i];
    }
    Serial.print(dataString);
    Serial.print("*");
    Serial.println(checksum, HEX);



    //Debug output to serial monitor
    //Serial.print("Sent to Wifi: ");
    //Serial.println(dataString);
}