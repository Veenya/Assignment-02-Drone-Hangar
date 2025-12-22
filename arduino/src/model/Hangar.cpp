#include "Arduino.h"

#include "Hangar.h"
#include "config.h"   // per eventuali angoli porta ecc.

bool SVILUPPO = true;

Hangar::Hangar(HWPlatform* hw)
  : pHW(hw),
    droneState(DroneState::REST),
    hangarState(HangarState::NORMAL),
    droneInside(true),   // da specifica: stato iniziale = dentro
    doorOpen(false) {
}

void Hangar::init() {
    L1isOn = false;
    L2isBlinking  = false;
    L3isOn  = false;


    pHW->getHangarDoorMotor()->motorOn();
    closeDoor();
    if (SVILUPPO) {
        Serial.println("Door and Led TEST");

        pHW->getL1()->switchOn();
        pHW->getL2()->switchOn();
        pHW->getL3()->switchOn();
        
        delay(1000);
        
        openDoor(); // TODO test collegamento pin e alimentazione, togliere alla fine dello sviluppo
        
        delay(1000);

        closeDoor();
        pHW->getL1()->switchOff();
        pHW->getL2()->switchOff();
        pHW->getL3()->switchOff();
        
    }
    Serial.println("Door is READY");
    droneInside = true;
    L1isOn = true;
    droneState = DroneState::REST;
    hangarState = HangarState::NORMAL;
}

/* --------- Stato drone --------- */

void Hangar::setDroneState(DroneState state) {
    this->droneState = state;
}

DroneState Hangar::getDroneState() {
    return this->droneState;
}

// void Hangar::setDroneInside(bool inside) {
//   droneInside = inside;
// }

// bool Hangar::isDroneInside() const {
//   return droneInside;
// }

/* --------- Porta hangar --------- */

void Hangar::openDoor() {
  auto motor = pHW->getHangarDoorMotor();
  // if (motor && !doorOpen) {
  if (motor) {
    motor->setPosition(DOOR_OPEN_ANGLE);  // es. 90°
    doorOpen = true;
  } else {
    Serial.println("NO MOTOR");
  }
}

void Hangar::closeDoor() {
  auto motor = pHW->getHangarDoorMotor();
  // if (motor && doorOpen) {
  if (motor) {
    motor->setPosition(DOOR_CLOSED_ANGLE);  // es. 0°
    doorOpen = false;
  } else {
    Serial.println("NO MOTOR");
  }
}

bool Hangar::isDoorOpen() {
  return doorOpen;
}

/* --------- Letture sensori --------- */

float Hangar::getDistance() {
  auto sonar = pHW->getDDD();
  if (!sonar) {
    return SONAR_NO_OBJ_DETECTED;
  }
  return sonar->getDistance();
}

bool Hangar::isDroneAbove() {
  auto pir = pHW->getDPD();
  if (!pir) {
    return false;
  }
  return pir->isDetected();
}

float Hangar::getTemperature() {
  auto temperatureSensor = pHW->getTempSensor();
  if (!temperatureSensor) {
    return NAN;
  }
  return temperatureSensor->getTemperature();
}

/* --------- Stato hangar / allarmi --------- */

void Hangar::setHangarState(HangarState state) {
    this->hangarState = state;
}

HangarState Hangar::getHangarState() {
    return this->hangarState;
}


void Hangar::setL1On() {
    this->L1isOn = true;
}
void Hangar::setL2Blinking() {
    this->L2isBlinking = true;
}
void Hangar::setL3On() {
    this->L3isOn = true;
}

void Hangar::setL1Off() {
    this->L1isOn = false;
}
void Hangar::setL2Off() {
    this->L2isBlinking = false;
}
void Hangar::setL3Off() {
    this->L3isOn = false;
}

void Hangar::manageLeds() {
    if (L1isOn) {
        pHW->getL1()->switchOn();
    } else {
        pHW->getL1()->switchOff();
    }
    if (L2isBlinking) {
        pHW->getL2()->switchOn(); // TODO: fare il blink
    } else {
        pHW->getL2()->switchOff();
    }
    if (L3isOn) { // LED ROSSO
        pHW->getL3()->switchOn();
    } else {
        pHW->getL3()->switchOff();
    }

}


void Hangar::sync(){
    manageLeds();

    float dist = lastDistance; 
    currentTemp = lastTemperature;
    
    currentTemp = pHW->getTempSensor()->getTemperature();
    dist = pHW->getDDD()->getDistance();
    // if (dist == SONAR_NO_OBJ_DETECTED){
    //   dist = 1000; // TODO: cambia
    // }
    lastDistance = dist; 
    lastTemperature = currentTemp; 
}

