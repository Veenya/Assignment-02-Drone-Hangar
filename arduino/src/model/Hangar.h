#ifndef __HANGAR__
#define __HANGAR__

#include "States.h"
#include "config.h"
#include "HWPlatform.h"

class Hangar {

public:
  Hangar(HWPlatform* hw);

  void init();
  void sync();

  /* --------- Stato drone --------- */

  void setDroneState(DroneState state);
  DroneState getDroneState();

  // void setDroneInside(bool inside);
  // bool isDroneInside();

  /* --------- Porta hangar --------- */

  void openDoor();
  void closeDoor();
  bool isDoorOpen();

  /* --------- Letture sensori --------- */

  // distanza dal sonar (DDD), in metri
  float getDistance();

  // presenza sopra l’hangar dal PIR (DPD)
  bool isDroneAbove();

  // temperatura interna (°C)
  float getTemperature();

  /* --------- Stato hangar / allarmi --------- */

  void setHangarState(HangarState state);
  HangarState getHangarState();

  private:
  HWPlatform* hw;

  DroneState droneState;
  HangarState hangarState;

  bool droneInside;
  bool doorOpen;

  float lastDistance;
  float currentTemp;
  float lastTemperature;
};

#endif
