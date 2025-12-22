#include "TemperatureTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

#define MAXTEMP      40      // es: 40°C, metti il valore che vuoi
#define MAXTEMPTIME  10000   // ms sopra soglia prima di entrare in allarme

TemperatureTask::TemperatureTask(Hangar* pHangar, UserPanel* pUserPanel) : 
    pHangar(pHangar) {
    setState(HangarState::NORMAL);
}

void TemperatureTask::tick(){
    pHangar->sync();

    float temp = pHangar->getTemperature(); //! controlla se il prof lo mette

    switch (state){    
    case HangarState::NORMAL: {
        // Logger.log(F("[TEMP] normal"));
        pHangar->setHangarState(HangarState::NORMAL);

        if (temp > MAXTEMP){
            setState(HangarState::PRE_ALARM);
        }
        break;
    }

    case HangarState::PRE_ALARM: {        
        Logger.log(F("[TEMP] pre-alarm"));
        
        if (temp < MAXTEMP){
            // la temperatura è tornata sotto soglia
            setState(HangarState::NORMAL);
        } else if (elapsedTimeInState() > MAXTEMPTIME){
            // troppo tempo sopra soglia -> allarme
            setState(HangarState::ALARM);
            pUserPanel->displayAlarm();
            
        }
        break;       
    }

    case HangarState::ALARM: {
        Logger.log(F("[TEMP] alarm"));
            pHangar->setHangarState(HangarState::ALARM);

        // Esci dall'allarme quando qualcuno rimette l'hangar in stato normale
        // TODO: controlla se va col tasto
        if (pHangar->getHangarState() == HangarState::NORMAL){
            setState(HangarState::NORMAL);
        }
        break;
    }    
    }
}

void TemperatureTask::setState(HangarState state){ 
    this->state = state;
    stateTimestamp = millis();
    justEntered = true;
}

long TemperatureTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}