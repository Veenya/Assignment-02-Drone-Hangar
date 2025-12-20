#include "DoorTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

DoorTask::DoorTask(CommunicationCenter* pCommunicationCenter, Hangar* pHangar, UserPanel* pUserPanel)
    :   pCommunicationCenter(pCommunicationCenter),
        pHangar(pHangar),
        pUserPanel(pUserPanel) {
    setDoorState(DoorState::CLOSED);
}

void DoorTask::tick() {    
    if (pHangar->getHangarState() == HangarState::ALARM) {

        if (pUserPanel) {
            pUserPanel->sync(); // aggiorno stato del pannello (bottone reset)
            if (pUserPanel->isResetPressed()) {
                pHangar->setHangarState(HangarState::NORMAL);
                Logger.log(F("[DR] Hangar is not in alarm state anymore"));
            }
        } 

    } else if (this->state == DoorState::CLOSED) {
        // --- controlla se c'è una richiesta di TAKE-OFF dal DRU ---
        if (pCommunicationCenter && pCommunicationCenter->checkAndResetTakeOffRequest()) {
            Logger.log(F("[DR] take-off request from DRU"));
            // il drone parte dal REST dentro l'hangar
            pHangar->setDroneState(DroneState::TAKING_OFF);
            this->setDoorState(DoorState::OPENING);
            this->stateTimestamp = millis();
            pHangar->openDoor();
            Logger.log(F("[DO] opening door"));
            // TODO ApriPorta ecc..
        }

        // --- controlla se c'è una richiesta di LANDING dal DRU ---
        if (pCommunicationCenter && pCommunicationCenter->checkAndResetLandingRequest()) {
            Logger.log(F("[DR] landing request from DRU"));
            pHangar->setDroneState(DroneState::LANDING);
            this->setDoorState(DoorState::OPENING);
            this->stateTimestamp = millis();
            pHangar->openDoor();
            Logger.log(F("[DO] opening door"));
            // TODO ApriPorta ecc..
        }


    } else if (this->state == DoorState::OPENING && elapsedTimeInState() > DOOR_TIME) {
        // TODO Se la porta si sta aprendo da n secondi
        this->setDoorState(DoorState::OPEN);
        this->stateTimestamp = millis();
        Logger.log(F("[DO] Door is Open"));

    } else if (this->state == DoorState::CLOSING && elapsedTimeInState() > DOOR_TIME) {
        // TODO Se la porta si sta aprendo da n secondi
        this->setDoorState(DoorState::CLOSED);
        Logger.log(F("[DO] Door is Closed"));

    } else if (this->state == DoorState::OPEN && elapsedTimeInState() > SPILLING_MAX_TIME) { // TODO Check drone
        this->setDoorState(DoorState::CLOSING);
        this->stateTimestamp = millis();
        pHangar->closeDoor();
        Logger.log(F("[DO] closing door"));
        // TODO Se la porta è aperta da n secondi e il drone è volato via chiudi
    } 
}

void DoorTask::setDoorState(DoorState state) {
    this->state = state;
    stateTimestamp = millis();
}

long DoorTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

