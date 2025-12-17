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
    // aggiorna i valori (temperatura / distanza ecc.)
    pHangar->sync();

    float temp = pHangar->getTemperature();

    switch (state){    
    case NORMAL: {
        if (checkAndSetJustEntered()){
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
            // se hai uno stato PRE_ALARM nell'enum, puoi metterlo qui
            // pHangar->setHangarState(HangarState::PRE_ALARM);
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
        if (pHangar->getHangarState() == HangarState::NORMAL){
            setState(NORMAL);
        }
        break;
    }    
    }
}

void TemperatureTask::setState(int s){
    //state state = s;
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