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
    /*
    Whenever the drone is inside the hangar (whether at rest, during take-off, or during landing), 
    the temperature monitoring system is active to check for potential problems. 
    If a temperature ≥ Temp1 is detected for more than T3 seconds, the system enters a pre-alarm state. 
    In this state, new take-offs and landings are suspended until the system returns to normal operation. 
    If a take-off or landing is already in progress, it is allowed to complete. If the temperature drops below Temp1, 
    the system returns to normal operation. If a temperature Temp2 > Temp1 is detected for more than T4 seconds, 
    the HD door is closed (if it was open), the L3 indicator light turns on, and the LCD displays ALARM. 
    If the drone is outside the hangar, the ALARM message is also sent to the drone via DRU. 
    All operations are suspended until the RESET button is pressed by an operator. When RESET is pressed, 
    it is assumed that all issues have been resolved, and the system returns to the normal state.
    */
    this->droneState = pHangar->getDroneState();
    if (pHangar->getHangarState() == HangarState::ALARM && (this->droneState != DroneState::TAKING_OFF || this->droneState != DroneState::LANDING)) {

        if (pUserPanel) {
            pUserPanel->sync(); // aggiorno stato del pannello (bottone reset)
            if (pUserPanel->isResetPressed()) {
                pHangar->setHangarState(HangarState::NORMAL);
                Logger.log(F("[DR] Hangar is not in alarm state anymore"));
            }
        } 

    } else if (this->doorState == DoorState::CLOSED) {
        // --- controlla se c'è una richiesta di TAKE-OFF dal DRU ---
        /*
        Take-off phase: The drone activates the hangar door opening command by sending a message through the DRU subsystem. 
        Upon receiving the command, the HD door opens, the LCD displays TAKE OFF, and the system waits for the drone to exit. 
        To determine when the drone has left, the DDD is used: when the measured distance is greater than D1 for more than T1 seconds, 
        it is assumed that the drone has exited, and the HD door is closed. The LCD then displays DRONE OUT.
        */
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
        /*
        Landing phase: When the drone approaches the hangar, it sends the opening command (via DRU). 
        If, upon receiving the command, the DPD detects the presence of the drone, 
        the HD door opens and the LCD displays LANDING. 
        The system then waits for the drone to enter and land. When the distance measured by the DDD is less 
        than D2 for more than T2 seconds, it is assumed that the drone has landed, and the door is closed. 
        The LCD then displays DRONE INSIDE.
        */
        /*
        During the take-off and landing phases, L2 blinks, with period 0.5 second -- otherwise it is off.
        */
        if (pCommunicationCenter && pCommunicationCenter->checkAndResetLandingRequest()) {
            Logger.log(F("[DR] landing request from DRU"));
            pHangar->setDroneState(DroneState::LANDING);
            this->setDoorState(DoorState::OPENING);
            this->stateTimestamp = millis();
            pHangar->openDoor();
            Logger.log(F("[DO] opening door"));
            // TODO ApriPorta ecc..
        }


    } else if (this->doorState == DoorState::OPENING && elapsedTimeInState() > DOOR_TIME) {
        // TODO Se la porta si sta aprendo da n secondi
        this->setDoorState(DoorState::OPEN);
        this->stateTimestamp = millis();
        Logger.log(F("[DO] Door is Open"));

    } else if (this->doorState == DoorState::CLOSING && elapsedTimeInState() > DOOR_TIME) {
        // TODO Se la porta si sta aprendo da n secondi
        this->setDoorState(DoorState::CLOSED);
        Logger.log(F("[DO] Door is Closed"));

    } else if (this->doorState == DoorState::OPEN && elapsedTimeInState() > SPILLING_MAX_TIME) { // TODO Check drone
        this->setDoorState(DoorState::CLOSING);
        this->stateTimestamp = millis();
        pHangar->closeDoor();
        Logger.log(F("[DO] closing door"));
        // TODO Se la porta è aperta da n secondi e il drone è volato via chiudi
    } 
}

void DoorTask::setDoorState(DoorState state) {
    this->doorState = state;
    stateTimestamp = millis();
}

long DoorTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

