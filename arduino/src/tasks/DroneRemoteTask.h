#ifndef __DRONE_REMOTE_TASK__
#define __DRONE_REMOTE_TASK__

#include "kernel/Task.h"
#include "model/Dashboard.h"
#include "model/Hangar.h"
#include "model/DroneRemoteUnit.h"   // <--- classe modello lato PC (tipo Dashboard)

/*
 * Task che gestisce la comunicazione con il DRU (Drone Remote Unit).
 * - legge i comandi dal PC (take-off, landing, reset...)
 * - aggiorna lo stato del drone nell'hangar
 * - notifica al DRU lo stato corrente (drone, hangar, distanza)
 */
class DroneRemoteTask : public Task {

public:
  DroneRemoteTask(Dashboard* pDashboard, Hangar* pHangar);
  void tick();

private:
  enum State {
    NORMAL,                 // situazione normale: nessun allarme
    WAITING_FOR_RESET_ALARM // siamo in allarme: aspettiamo reset dal DRU o dal bottone
  };

  void setState(State state);
  long elapsedTimeInState();
  bool checkAndSetJustEntered();

  State state;
  long stateTimestamp;
  bool justEntered;

  Dashboard* pDashboard;
  Hangar* pHangar;
};

#endif
