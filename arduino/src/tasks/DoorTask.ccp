#include "DoorTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

// puoi decidere un periodo di invio stato, ad es. 500 ms
#define STATE_UPDATE_PERIOD 500

DoorTask::DoorTask(CommunicationCenter* pCommunicationCenter, Hangar* pHangar)
    : pCommunicationCenter(pCommunicationCenter),
        pHangar(pHangar) {
    setDoorState(DoorState::CLOSED);
}

void DoorTask::tick(){    
    if (pHangar->getHangarState() == HangarState::ALARM) {
            // TODO fai cose e non aprire la porta

    } else if (this->state == DoorState::CLOSED) {
        // --- controlla se c'è una richiesta di TAKE-OFF dal DRU ---
        if (pCommunicationCenter && pCommunicationCenter->checkAndResetTakeOffRequest()) {
        Logger.log(F("[DR] take-off request from DRU"));
        // il drone parte dal REST dentro l'hangar
        pHangar->setDroneState(DroneState::TAKING_OFF);
        this->setDoorState(DoorState::OPENING);
        this->stateTimestamp = millis();
        // TODO ApriPorta ecc..
        }

        // --- controlla se c'è una richiesta di LANDING dal DRU ---
        if (pCommunicationCenter && pCommunicationCenter->checkAndResetLandingRequest()) {
        Logger.log(F("[DR] landing request from DRU"));
        pHangar->setDroneState(DroneState::LANDING);
        this->setDoorState(DoorState::OPENING);
        this->stateTimestamp = millis();
        // TODO ApriPorta ecc..
        }

        // --- se l'hangar va in allarme, passiamo allo stato di attesa reset ---
        if (pHangar->getHangarState() == HangarState::ALARM) {
        pHangar->setDroneState(DroneState::WAITING_FOR_RESET_ALARM);
        }

    } else if (this->state == DoorState::OPENING) {
        // TODO Se la porta si sta aprendo da n secondi
        this->setDoorState(DoorState::OPEN);
    } else if (this->state == DoorState::CLOSING) {
        // TODO Se la porta si sta aprendo da n secondi
        this->setDoorState(DoorState::CLOSED);
    } else if (this->state == DoorState::OPEN ) { // TODO Check drone
        this->setDoorState(DoorState::CLOSING);
        this->stateTimestamp = millis();
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

