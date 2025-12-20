#include "TemperatureTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

#define MAXTEMP      40      // es: 40°C, metti il valore che vuoi
#define MAXTEMPTIME  10000   // ms sopra soglia prima di entrare in allarme

TemperatureTask::TemperatureTask(Hangar* pHangar) : 
    pHangar(pHangar) {
    setState(NORMAL);
}

void TemperatureTask::tick(){
    pHangar->sync();

    float temp = pHangar->getTemperature(); //! controlla se il prof lo mette

    switch (state){    
    case NORMAL: {
        // TODO: possibilmente togliere l'if
        if (this->checkAndSetJustEntered()){ //? Serve?
            Logger.log(F("[TEMP] normal"));
            pHangar->setHangarState(HangarState::NORMAL);
        }

        if (temp > MAXTEMP){
            setState(PRE_ALARM);
        }
        break;
    }

    case PRE_ALARM: {        
        if (checkAndSetJustEntered()){
            Logger.log(F("[TEMP] pre-alarm"));
        }
        
        if (temp < MAXTEMP){
            // la temperatura è tornata sotto soglia
            setState(NORMAL);
        } else if (elapsedTimeInState() > MAXTEMPTIME){
            // troppo tempo sopra soglia -> allarme
            setState(ALARM);
        }
        break;       
    }

    case ALARM: {
        if (checkAndSetJustEntered()){
            Logger.log(F("[TEMP] alarm"));
            // qui metti lo stato di allarme / manutenzione
            pHangar->setHangarState(HangarState::ALARM);
        }

        // Esci dall'allarme quando qualcuno rimette l'hangar in stato normale
        // TODO: controlla se va col tasto
        if (pHangar->getHangarState() == HangarState::NORMAL){
            setState(NORMAL);
        }
        break;
    }    
    }
}

void TemperatureTask::setState(State s){ 
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long TemperatureTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool TemperatureTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}