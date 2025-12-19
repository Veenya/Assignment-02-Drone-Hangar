#ifndef __DASHBOARD__
#define __DASHBOARD__

#include "config.h"
#include "HWPlatform.h"
#include "model/Hangar.h"

class Dashboard {

public:
  Dashboard(Hangar* pHangar); //? Hangar serve?

  void init();
  
  void notifyNewState();  // TODO RIVEDERE
  
  bool checkAndResetDischargeRequest(); // TODO RIVEDERE
  bool checkAndResetMaintenanceDone();  // TODO RIVEDERE

  void sync();

private:
  Hangar* pHangar;
  bool dischargeCmdRequested;   // TODO RIVEDERE
  bool maintenanceDoneNotified; // TODO RIVEDERE
};

#endif
