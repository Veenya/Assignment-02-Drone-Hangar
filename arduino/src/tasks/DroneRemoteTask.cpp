#include "DroneRemoteTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

// puoi decidere un periodo di invio stato, ad es. 500 ms
#define STATE_UPDATE_PERIOD 500

DroneRemoteTask::DroneRemoteTask(CommunicationCenter* pCommunicationCenter, Hangar* pHangar)
  : pCommunicationCenter(pCommunicationCenter),
    pHangar(pHangar) {
  setState(NORMAL);
}
  
void DroneRemoteTask::tick(){    
  // 1) aggiorna informazioni dal canale seriale / remoto
  if (pCommunicationCenter) {
    pCommunicationCenter->sync();   // leggere eventuali messaggi in arrivo dal DRU
  }

  // 2) invia periodicamente lo stato corrente al DRU
  static unsigned long lastStateUpdate = 0;
  unsigned long now = millis();

  if (pCommunicationCenter && (now - lastStateUpdate >= STATE_UPDATE_PERIOD)) {
    lastStateUpdate = now;
    pCommunicationCenter->notifyNewState();
  }

  // 3) gestione macchina a stati
  switch (state) {

  case NORMAL: {
    if (checkAndSetJustEntered()) {
      Logger.log(F("[DR] normal"));
    }

    // --- controlla se c'è una richiesta di TAKE-OFF dal DRU ---
    if (pCommunicationCenter && pCommunicationCenter->checkAndResetTakeOffRequest()) {
      Logger.log(F("[DR] take-off request from DRU"));
      // il drone parte dal REST dentro l'hangar
      pHangar->setDroneState(DroneState::TAKING_OFF);
      // altri task (es. DroneOperationTask) si occuperanno di aprire porta, ecc.
    }

    // --- controlla se c'è una richiesta di LANDING dal DRU ---
    if (pCommunicationCenter && pCommunicationCenter->checkAndResetLandingRequest()) {
      Logger.log(F("[DR] landing request from DRU"));
      pHangar->setDroneState(DroneState::LANDING);
      // anche qui: la logica di porta/sonar viene gestita da altri task
    }

    // --- se l'hangar va in allarme, passiamo allo stato di attesa reset ---
    if (pHangar->getHangarState() == HangarState::ALARM) {
      setState(WAITING_FOR_RESET_ALARM);
    }

    break;
  }

  case WAITING_FOR_RESET_ALARM: {
    if (checkAndSetJustEntered()) {
      Logger.log(F("[DR] alarm state, notifying DRU"));

      // se il drone è fuori, manda un messaggio di ALLARM al DRU
      if (!pHangar->isDroneInside() && pCommunicationCenter) {
        // TODO: adatta al tuo protocollo
        pCommunicationCenter->notifyAlarm();      // ad es. manda "ALLARM" sul canale seriale
      }
    }

    // qui puoi decidere come uscire dall'allarme:
    // - reset da PC (DRU)
    // - reset da bottone locale (UserPanel)
    bool resetFromRemote = pCommunicationCenter && pCommunicationCenter->checkAndResetAlarmResetRequest();
    bool hangarBackToNormal = (pHangar->getHangarState() == HangarState::NORMAL);

    if (resetFromRemote || hangarBackToNormal) {
      Logger.log(F("[DR] alarm reset, back to NORMAL"));
      setState(NORMAL);
    }

    break;
  }

  } // end switch
}

void DroneRemoteTask::setState(State s){
  state = s;
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
