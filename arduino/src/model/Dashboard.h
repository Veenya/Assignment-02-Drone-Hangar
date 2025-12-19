#ifndef __DASHBOARD__
#define __DASHBOARD__

#include "config.h"
#include "HWPlatform.h"
#include "model/Hangar.h"

class Dashboard {

public:
  Dashboard(Hangar* pHangar);

  void init();
  
  void notifyNewState();
  
  bool checkAndResetOpenDoorRequest();

  void sync();

private:
  Hangar* pHangar;
  bool openDoorNotification;
};

#endif
