#ifndef __DASHBOARD__
#define __DASHBOARD__

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "HWPlatform.h"
#include "States.h"

class Dashboard {
public:
  Dashboard(HWPlatform* hw);

  void init();   // opzionale, per impostare lo schermo iniziale

  // --- messaggi principali su LCD/LED ---

  void showDroneInside();
  void showDroneOut();
  void showTakeOff();
  void showLanding();
  void showPreAlarm();
  void showAlarm();

  // --- gestione LED L2 / L3 ---

  // L2 lampeggia: ogni chiamata inverte lo stato
  void blinkL2();

  // Imposta L2 / L3 esplicitamente
  void setL2(bool on);
  void setL3(bool on);

private:
  HWPlatform* hw;

  LiquidCrystal_I2C* lcd;
  Led* l1;
  Led* l2;
  Led* l3;

  bool l2State;
};

#endif
