#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "Arduino.h"
#include "Task.h"

#define MAX_TASKS 50

class Scheduler {
  
  int basePeriod;
  int nTasks;
  Task* taskList[MAX_TASKS];  

  unsigned long lastRun;   // ultimo tick eseguito (in ms, da millis())

public:
  void init(int basePeriod);  
  virtual bool addTask(Task* task);  
  virtual void schedule();
};

#endif
