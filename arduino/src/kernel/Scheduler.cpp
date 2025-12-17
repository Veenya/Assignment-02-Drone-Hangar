#include "Scheduler.h"

void Scheduler::init(int basePeriod){
  this->basePeriod = basePeriod;
  nTasks = 0;
  lastRun = millis();    // primo riferimento temporale
}

bool Scheduler::addTask(Task* task){
  if (nTasks < MAX_TASKS){
    taskList[nTasks] = task;
    nTasks++;
    return true;
  } else {
    return false; 
  }
}
  
void Scheduler::schedule(){   
  unsigned long now = millis();

  // se non è ancora passato il periodo base, non facciamo nulla
  if (now - lastRun < (unsigned long)basePeriod){
    return;
  }

  // aggiorna il riferimento
  lastRun = now;

  for (int i = 0; i < nTasks; i++){
    if (taskList[i]->isActive()){
      if (taskList[i]->isPeriodic()){
        if (taskList[i]->updateAndCheckTime(basePeriod)){
          taskList[i]->tick();
        }
      } else {
        taskList[i]->tick();
        if (taskList[i]->isCompleted()){
          taskList[i]->setActive(false);
        }
      }
    }
  }
}
