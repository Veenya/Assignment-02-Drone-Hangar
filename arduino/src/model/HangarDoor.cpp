#include "HangarDoor.h"

HangarDoor::HangarDoor(ServoMotor* doorMotor)
  : motor(doorMotor),
    openAngle(DOOR_OPEN_ANGLE),
    closedAngle(DOOR_CLOSED_ANGLE),
    openFlag(false) {
}

void HangarDoor::setOpenAngle(int angle) {
  if (angle < DOOR_OPEN_ANGLE)   angle = DOOR_OPEN_ANGLE;
  if (angle > DOOR_CLOSED_ANGLE) angle = DOOR_CLOSED_ANGLE;
  openAngle = angle;
}

void HangarDoor::setClosedAngle(int angle) {
  if (angle < DOOR_OPEN_ANGLE)   angle = DOOR_OPEN_ANGLE;
  if (angle > DOOR_CLOSED_ANGLE) angle = DOOR_CLOSED_ANGLE;
  closedAngle = angle;
}

void HangarDoor::open() {
  if (!motor) return;

  motor->setPosition(openAngle);
  openFlag = true;
}

void HangarDoor::close() {
  if (!motor) return;

  motor->setPosition(closedAngle);
  openFlag = false;
}

bool HangarDoor::isOpen() const {
  return openFlag;
}

bool HangarDoor::isClosed() const {
  return !openFlag;
}
