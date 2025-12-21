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
    closeDoor();                   // chiude fisicamente la porta
    droneInside = true;
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

bool Hangar::isDoorOpen() {
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
  return pir->isDetected();
}

float Hangar::getTemperature() {
  auto temperatureSensor = hw->getTempSensor();
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


void Hangar::sync(){
    float dist = lastDistance; 
    currentTemp = lastTemperature;
    
    currentTemp = hw->getTempSensor()->getTemperature();
    dist = hw->getDDD()->getDistance();
    // if (dist == SONAR_NO_OBJ_DETECTED){
    //   dist = 1000; // TODO: cambia
    // }
    lastDistance = dist; 
    lastTemperature = currentTemp; 
}

