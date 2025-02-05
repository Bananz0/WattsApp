//
// Created by glenm on 2/5/2025.
//

#ifndef POWERHANDLER_H
#define POWERHANDLER_H



class PowerHandler {
public:
  PowerHandler();
  ~PowerHandler();
  static void enterSleep();
  static void exitSleep();
  static void manageBacklight(bool backlight);
  static void manageWiFiPower(bool enable);


};



#endif //POWERHANDLER_H
