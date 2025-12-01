/* Contiene tutti gli oggetti device e usa i pin di config.h. */
#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"

#include "devices/button/Button.h"
#include "devices/led/Led.h"
#include "devices/pir/Pir.h"
#include "devices/proximity_sensor/Sonar.h"
#include "devices/temperature_sensor/TempSensor.h"
#include "devices/servo_motor/servo_motor.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class HWPlatform {

public:
  HWPlatform();

  void init();   // inizializza tutto l'hardware (lcd, pins, ecc.)
  void test();

  // --- accesso ai device ---

  // Drone Distance Detector (sonar dentro l’hangar)
  Sonar* getDDD();

  // Drone Presence Detector (PIR sopra l’hangar)
  Pir* getDPD();

  // Motore porta hangar
  ServoMotor* getHangarDoorMotor();

  // Sensore temperatura (DHT11 tramite TempSensorDHT11)
  TempSensor* getTempSensor();

  // LCD dell’operatore
  LiquidCrystal_I2C* getOperatorLcd();

  // LED
  Led* getL1();    // verde
  Led* getL2();    // verde blink durante takeoff/landing
  Led* getL3();    // rosso allarme

  // Bottone di reset allarme
  Button* getResetButton();

private:
  // istanze concrete dei device
  Sonar* pDdd;
  Pir* pDpd;
  ServoMotor* pDoorMotor;
  TempSensor* pTempSensor;           // in .cpp la costruirai come TempSensorDHT11
  LiquidCrystal_I2C* pLcd;
  Led* pL1;
  Led* pL2;
  Led* pL3;
  Button* pResetButton;
};

#endif
