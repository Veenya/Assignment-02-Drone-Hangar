#ifndef __TEMPERATURE_TASK__
#define __TEMPERATURE_TASK__

#include "kernel/Task.h"
#include "model/Dashboard.h"
#include "model/Hangar.h"
#include "model/HangarDoor.h"
#include "model/HWPlatform.h"
#include "model/States.h"
#include "model/UserPanel.h"

class TemperatureTask: public Task {

public:
  TemperatureTask(Hangar* pHangar); 
  void tick();

private:  
  void setState(int state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  enum { NORMAL, PRE_ALARM, ALARM} state;
  long stateTimestamp;
  bool justEntered;

  Hangar* pHangar;
};

#endif