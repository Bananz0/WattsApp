//
// Created by glenm on 2/5/2025.
//

#include "TimeHandler.h"


TimeHandler::TimeHandler() {
}

TimeHandler::~TimeHandler() {
}

bool TimeHandler::syncTime() {
    timeUTC->tm_hour = NULL;
    timeUTC->tm_min = NULL;
    timeUTC->tm_sec = NULL;
    timeUTC->tm_mday = NULL;
    timeUTC->tm_mon = NULL;
    timeUTC->tm_year = NULL;
}

void TimeHandler::updateTime() {

}

char * TimeHandler::returnTime() {
    snprintf(timeString, sizeof(timeString), "Time: %02d:%02d:%02d",timeUTC->tm_hour,timeUTC->tm_min, timeUTC->tm_sec);
    return timeString;
}

uint32_t TimeHandler::returnUNIXTime() {
    //return time();
    return 0;
}

