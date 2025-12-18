#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

void setup() {
    MsgService.init();
    delay(2000);
    MsgService.sendMsg("lo:arduino ready");
}


void loop() {
  Logger.log(F("PING\n"));             // manda "lo:PING" ogni secondo
  delay(1000);
}
