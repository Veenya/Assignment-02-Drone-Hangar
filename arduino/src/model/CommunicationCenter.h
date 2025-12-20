#ifndef __COMMUNICATIONCENTER__
#define __COMMUNICATIONCENTER__

#include "config.h"
#include "HWPlatform.h"
#include "model/Hangar.h"


/*
 * Classe che comunica con il con il DRU (Drone Remote Unit) sul PC.
 * - Riceve comandi testuali via seriale (TAKEOFF, LANDING, RESET).
 * - Espone metodi "checkAndReset..." per i Task.
 * - Invia periodicamente lo stato corrente al DRU.
 */
class CommunicationCenter {

public:
  CommunicationCenter(Hangar* pHangar);

  void init();
  
  void notifyNewState();
  
  bool checkAndResetOpenDoorRequest();
  bool checkAndResetTakeOffRequest(); //? Serve la distinzione?
  bool checkAndResetLandingRequest(); //? Serve la distinzione?
  bool checkAndResetAlarmResetRequest(); //? Serve?
  bool notifyAlarm(); //? Serve?
  


  void sync();

private:
  Hangar* pHangar;
  bool openDoorNotification;
};

#endif
