//
// Created by glenm on 2/5/2025.
//

#ifndef POWERHANDLER_H
#define POWERHANDLER_H



class PowerHandler {
public:
  PowerHandler();
  ~PowerHandler();
  void enterSleep();
  void exitSleep();
  void manageBacklight(bool backlight);
  void manageWiFiPower(bool enable);


};



#endif //POWERHANDLER_H
