#include "HWPlatform.h"

#include "devices/button/ButtonImpl.h"
#include "devices/led/Led.h"
#include "devices/pir/Pir.h"
#include "devices/proximity_sensor/Sonar.h"
#include "devices/temperature_sensor/TemperatureSensorDHT11.h"
#include "devices/servo_motor/servo_motor_impl.h"

HWPlatform::HWPlatform()
  : pDdd(nullptr),
    pDpd(nullptr),
    pDoorMotor(nullptr),
    pTempSensor(nullptr),
    pLcd(nullptr),
    pL1(nullptr),
    pL2(nullptr),
    pL3(nullptr),
    pResetButton(nullptr) {
}

void HWPlatform::init() {
  // --- crea i sensori / attuatori usando i pin da config.h ---

  // Sonar per distanza del drone (DDD)
  pDdd = new Sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN, MAX_TIME_US); 
  // se non hai DDD_MAX_TIME_US, puoi definirlo in config.h (es. 30000L)

  // PIR per presenza drone (DPD)
  pDpd = new Pir(PIR_PIN);

  // Servo porta hangar
  pDoorMotor = new ServoMotorImpl(SERVO_PIN);
  pDoorMotor->on();    // attacca subito il servo

  // Sensore temperatura (DHT11) dietro interfaccia TempSensor
  pTempSensor = new TempSensorDHT11(TEMP_PIN);

  // LCD I2C operatore
  pLcd = new LiquidCrystal_I2C(LCD_ADDR, 16, 2);
  pLcd->init();
  pLcd->backlight();
  pLcd->clear();
  pLcd->setCursor(0, 0);
  pLcd->print("DRONE INSIDE");   // stato iniziale richiesto

  // LED
  pL1 = new Led(L1_PIN);
  pL2 = new Led(L2_PIN);
  pL3 = new Led(L3_PIN);

  // Assicuriamoci che all'inizio siano nello stato di default
  pL1->switchOn();     // ad esempio "sistema acceso"
  pL2->switchOff();
  pL3->switchOff();

  // Bottone reset allarme
  pResetButton = new ButtonImpl(BUTTON_PIN);
}

void HWPlatform::test() {
  // mini test hardware (puoi cambiare/estendere come vuoi)
  if (pLcd) {
    pLcd->clear();
    pLcd->setCursor(0, 0);
    pLcd->print("HW TEST");
  }
  if (pL1) pL1->switchOn();
  if (pL2) pL2->switchOn();
  if (pL3) pL3->switchOn();
}

// --- getters ---

Sonar* HWPlatform::getDDD() {
  return pDdd;
}

Pir* HWPlatform::getDPD() {
  return pDpd;
}

ServoMotor* HWPlatform::getHangarDoorMotor() {
  return pDoorMotor;
}

TempSensor* HWPlatform::getTempSensor() {
  return pTempSensor;
}

LiquidCrystal_I2C* HWPlatform::getOperatorLcd() {
  return pLcd;
}

Led* HWPlatform::getL1() {
  return pL1;
}

Led* HWPlatform::getL2() {
  return pL2;
}

Led* HWPlatform::getL3() {
  return pL3;
}

Button* HWPlatform::getResetButton() {
  return pResetButton;
}
