//
// Created by glenm on 2/5/2025.
//

#ifndef TIMEHANDLER_H
#define TIMEHANDLER_H

#include "time.h"
#include "globalVariables.h"

class TimeHandler {
  public:
    TimeHandler();
    ~TimeHandler();
    bool syncTime();
    bool getTime();
    void updateTime();
    char* returnTime();
    uint32_t returnUNIXTime();

private:
    char timeString[20];
};



#endif //TIMEHANDLER_H
