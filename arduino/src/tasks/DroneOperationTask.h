#ifndef __DRONE_OPERATION_TASK__
#define __DRONE_OPERATION_TASK__

#include "kernel/Task.h"
#include "model/Hangar.h"
#include "model/UserPanel.h"

class DroneOperationTask: public Task {

public:
  DroneOperationTask(Hangar* pHangar, UserPanel* pPanel);
  void tick();

private:
  // Stati della FSM per la porta dell’hangar
  enum State {
    READY,          // porta chiusa, drone dentro
    DOOR_OPENING,   // sto aprendo la porta
    SPILLING,       // "operazione in corso" (es. decollo/atterraggio)
    DOOR_CLOSING    // sto chiudendo la porta
  };

  void setState(State s);
  long elapsedTimeInState();
  bool checkAndSetJustEntered();

  State state;
  long stateTimestamp;
  bool justEntered;

  Hangar* pHangar;
  UserPanel* pPanel;
};

#endif
