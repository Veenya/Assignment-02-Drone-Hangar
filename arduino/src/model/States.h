/* Maybe remove? */

#ifndef __STATES__
#define __STATES__

enum class DroneState { REST, WAITING, TAKING_OFF, OPERATING, LANDING, NORMAL, WAITING_FOR_RESET_ALARM }; // TODO Rivedere qui e in DroneRemoteTask
enum class HangarState { NORMAL, PRE_ALARM, ALARM };
enum class DoorState { OPEN, OPENING, CLOSED, CLOSING };
#endif
