#include "Arduino.h"

#include "Hangar.h"
#include "config.h"   // per eventuali angoli porta ecc.

// opzionale: definisci qui gli angoli della porta se vuoi
#ifndef DOOR_OPEN_ANGLE
#define DOOR_OPEN_ANGLE   90
#endif

#ifndef DOOR_CLOSED_ANGLE
#define DOOR_CLOSED_ANGLE 0
#endif

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
  auto motor = hw->getHangarDoorMotor();

  if (motor) {
    motor->setPosition(DOOR_CLOSED_ANGLE);  // forza chiusura
  }

  doorOpen = false;

  droneInside = true;
  droneState = DroneState::REST;
  hangarState = HangarState::NORMAL;
}

/* --------- Stato drone --------- */

void Hangar::setDroneState(DroneState s) {
  droneState = s;
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
  // restituisci semplicemente l'ultima distanza letta in sync()
  return lastDistance;
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
  // restituisci semplicemente l'ultima temperatura letta in sync()
  return currentTemp;
}

/* --------- Stato hangar / allarmi --------- */

void Hangar::setHangarState(HangarState s) {
  hangarState = s;
}

HangarState Hangar::getHangarState() const {
  return hangarState;
}


void Hangar::sync() {
  // temperatura
  auto ts = hw->getTempSensor();
  if (ts) {
    currentTemp = ts->getTemperature();
  } else {
    currentTemp = NAN;
  }

  // distanza
  auto sonar = hw->getDDD();
  if (sonar) {
    float d = sonar->getDistance();
    if (d == NO_OBJ_DETECTED) {
      d = 1000.0f;   // valore "molto lontano", da adattare
    }
    lastDistance = d;
  } else {
    lastDistance = NO_OBJ_DETECTED;
  }

  // se vuoi che lastTemperature contenga sempre l'ultima lettura:
  lastTemperature = currentTemp;
}


