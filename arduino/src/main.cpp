#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

void setup() {
    MsgService.init();
    delay(2000);
    MsgService.sendMsg("lo:arduino ready");
    Logger.log(F("Test"));
}


void loop() {
  MsgService.sendMsg("PING");
  //Logger.log(F("PING\n"));             // manda "lo:PING" ogni secondo
  delay(1000);
}
