//
// Created by glenm on 2/5/2025.
//

#ifndef TIMEHANDLER_H
#define TIMEHANDLER_H



class TimeHandler {
  public:
    TimeHandler();
    ~TimeHandler();
    static bool syncTime();
    static bool getTime();
    static void updateTime();



};



#endif //TIMEHANDLER_H
