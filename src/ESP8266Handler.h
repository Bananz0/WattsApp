//
// Created by glenm on 3/8/2025.
//

#ifndef ESP8266HANDLER_H
#define ESP8266HANDLER_H

#include <Arduino.h>
#include <WiFiEsp/WiFiEsp.h>
#include "Loads.h"
#include "Sources.h"
#include "WiFiHandler.h"

#include <HardwareSerial.h>
#include "globalVariables.h"
#include "globalVariables.h"
#include <checksum/checksum.h>

class ESP8266Handler {
  public:
    ESP8266Handler(Loads *loads, Sources *sources);
    ~ESP8266Handler();

    void enableESP();
    void sendDataToWifi();
    void processSerialCommand();

    private:
      Loads *loads;
      Sources *sources;
};

#endif //ESP8266HANDLER_H
