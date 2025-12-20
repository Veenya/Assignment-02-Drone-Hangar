#include "DroneRemoteTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

// puoi decidere un periodo di invio stato, ad es. 500 ms
#define STATE_UPDATE_PERIOD 500

DroneRemoteTask::DroneRemoteTask(CommunicationCenter* pCommunicationCenter, Hangar* pHangar)
  : pCommunicationCenter(pCommunicationCenter),
    pHangar(pHangar) {
  setState(DroneState::NORMAL);
}
  
void DroneRemoteTask::tick(){    
  // 1) aggiorna informazioni dal canale seriale / remoto
  if (pCommunicationCenter) {
    pCommunicationCenter->sync();   // leggere eventuali messaggi in arrivo dal DRU
  }

  // 2) invia periodicamente lo stato corrente al DRU
  lastStateUpdate = 0;
  now = millis();

  if (pCommunicationCenter && (now - lastStateUpdate >= STATE_UPDATE_PERIOD)) {
    lastStateUpdate = now;
    pCommunicationCenter->notifyNewState();
  }
}

void DroneRemoteTask::setState(DroneState state){
  this->state = state;
  stateTimestamp = millis();
  justEntered = true;
}

long DroneRemoteTask::elapsedTimeInState(){
  return millis() - stateTimestamp;
}

bool DroneRemoteTask::checkAndSetJustEntered(){
  bool bak = justEntered;
  if (justEntered){
    justEntered = false;
  }
  return bak;
}
