#include "Arduino.h"

#include "Hangar.h"
#include "config.h"   // per eventuali angoli porta ecc.


Hangar::Hangar(HWPlatform* hw)
  : hw(hw),
    droneState(DroneState::REST),
    hangarState(HangarState::NORMAL),
    droneInside(true),   // da specifica: stato iniziale = dentro
    doorOpen(false) {
}

void Hangar::init() {
  // stato iniziale richiesto:
  // - porta chiusa
  // - drone dentro
  // - hangar normale

  closeDoor();                   // chiude fisicamente la porta
  droneInside = true;
  droneState = DroneState::REST;
  hangarState = HangarState::NORMAL;
}

/* --------- Stato drone --------- */

void Hangar::setDroneState(DroneState state) {
  droneState = state;
}

DroneState Hangar::getDroneState() const {
  return droneState;
}

void Hangar::setDroneInside(bool inside) {
  droneInside = inside;
}

bool Hangar::isDroneInside() const {
  return droneInside;
}

/* --------- Porta hangar --------- */

void Hangar::openDoor() {
  auto motor = hw->getHangarDoorMotor();
  if (motor && !doorOpen) {
    motor->setPosition(DOOR_OPEN_ANGLE);  // es. 90°
    doorOpen = true;
  }
}

void Hangar::closeDoor() {
  auto motor = hw->getHangarDoorMotor();
  if (motor && doorOpen) {
    motor->setPosition(DOOR_CLOSED_ANGLE);  // es. 0°
    doorOpen = false;
  }
}

bool Hangar::isDoorOpen() const {
  return doorOpen;
}

/* --------- Letture sensori --------- */

float Hangar::getDistance() {
  auto sonar = hw->getDDD();
  if (!sonar) {
    return SONAR_NO_OBJ_DETECTED;
  }
  return sonar->getDistance();
}

bool Hangar::isDroneAbove() {
  auto pir = hw->getDPD();
  if (!pir) {
    return false;
  }
  // adatta al nome del metodo reale del tuo Pir
  return pir->isDetected();   // oppure pir->isPresent(), pir->isMotionDetected()...
}

float Hangar::getTemperature() {
  auto ts = hw->getTempSensor();
  if (!ts) {
    return NAN;
  }
  return ts->getTemperature();
}

/* --------- Stato hangar / allarmi --------- */

void Hangar::setHangarState(HangarState s) {
  hangarState = s;
}

HangarState Hangar::getHangarState() const {
  return hangarState;
}


void Hangar::sync(){
  float dist = lastDistance; 
  currentTemp = lastTemperature;
  
    currentTemp = hw->getTempSensor()->getTemperature();
    dist = hw->getDDD()->getDistance();
    if (dist == SONAR_NO_OBJ_DETECTED){
      dist = 1000; // TODO: cambia
    }
    lastDistance = dist; 
    lastTemperature = currentTemp; 
}

