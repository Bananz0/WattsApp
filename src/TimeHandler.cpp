//
// Created by glenm on 2/5/2025.
//

#include "TimeHandler.h"


TimeHandler::TimeHandler() {
    timeUTC->tm_hour = 12;
    timeUTC->tm_min = 00;
    timeUTC->tm_sec = 00;
    timeUTC->tm_mday = 9;
    timeUTC->tm_mon = 02;
    timeUTC->tm_year = 2024;
}

TimeHandler::~TimeHandler() {
}

bool TimeHandler::syncTime(tm ntpTime) {
    timeUTC->tm_hour = ntpTime.tm_hour;
    timeUTC->tm_min = ntpTime.tm_min;
    timeUTC->tm_sec = ntpTime.tm_sec;
    timeUTC->tm_mday = ntpTime.tm_mday;
    timeUTC->tm_mon = ntpTime.tm_mon;
    timeUTC->tm_year = ntpTime.tm_year;
    return true;
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

