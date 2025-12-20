#ifndef __DOOR_TASK__
#define __DOOR_TASK__

#include "kernel/Task.h"
#include "model/States.h"
#include "model/CommunicationCenter.h"
#include "model/Hangar.h"
#include "model/UserPanel.h"

/*
 * Task che gestisce la porta dell'hangar.
 * - legge lo stato delle richieste/notifiche
 * - apre e chiude la porta
 */
class DoorTask : public Task {

public:
    DoorTask(CommunicationCenter* pCommunicationCenter, Hangar* pHangar, UserPanel* pPanel);
    void tick();

private:
    void setDoorState(DoorState state);
    long elapsedTimeInState();

    DoorState doorState;
    DroneState droneState;
    long stateTimestamp;

    CommunicationCenter* pCommunicationCenter;
    Hangar* pHangar;
    UserPanel* pUserPanel;
};

#endif
