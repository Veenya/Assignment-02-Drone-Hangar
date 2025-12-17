#include "DroneOperationTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include <avr/sleep.h>

#define DOOR_TIME           1000      // ms per aprire/chiudere la porta
#define SPILLING_MAX_TIME   20000     // ms di "operazione" (decollo/landing)

DroneOperationTask::DroneOperationTask(Hangar* pHangar, UserPanel* pPanel)
  : pHangar(pHangar),
    pPanel(pPanel) {
  setState(READY);
}
  
void DroneOperationTask::tick(){    
  // aggiorno hangar (sensori ecc.)
  pHangar->sync();

  // aggiorno stato del pannello (bottone reset)
  if (pPanel) {
    pPanel->sync();
  }

  switch (state){    
  case READY: {
    if (checkAndSetJustEntered()){
      Logger.log(F("[DO] READY (door closed)"));
      // qui potresti aggiornare LCD tramite HWPlatform, se vuoi
    }

    // Per ora usiamo il bottone RESET come "comando" di apertura
    bool openCommand = (pPanel && pPanel->isResetPressedEdge());
    if (openCommand) {
      setState(DOOR_OPENING);
    }
    break;
  }

  case DOOR_OPENING: {        
    if (checkAndSetJustEntered()){
      Logger.log(F("[DO] opening door"));
      pHangar->openDoor();
    }

    // dopo DOOR_TIME ms consideriamo la porta aperta
    if (elapsedTimeInState() > DOOR_TIME){
      setState(SPILLING);
    }
    break;       
  }

  case SPILLING: {
    if (checkAndSetJustEntered()){
      Logger.log(F("[DO] operation in progress"));
      // qui potresti mostrare "TAKE OFF" o "LANDING" su LCD,
      // a seconda del comando che hai ricevuto dal DRU
    }

    // dopo un po' di tempo chiudiamo la porta
    if (elapsedTimeInState() > SPILLING_MAX_TIME){
      setState(DOOR_CLOSING);            
    } 
    break;
  }    

  case DOOR_CLOSING: {        
    if (checkAndSetJustEntered()){
      Logger.log(F("[DO] closing door"));
      pHangar->closeDoor();
    }

    if (elapsedTimeInState() > DOOR_TIME){
      Logger.log(F("[DO] back to READY"));
      setState(READY);
    }
    break;       
  }
  }
}

void DroneOperationTask::setState(State s){
  state = s;
  stateTimestamp = millis();
  justEntered = true;
}

long DroneOperationTask::elapsedTimeInState(){
  return millis() - stateTimestamp;
}

bool DroneOperationTask::checkAndSetJustEntered(){
  bool bak = justEntered;
  if (justEntered){
    justEntered = false;
  }
    return bak;
}
