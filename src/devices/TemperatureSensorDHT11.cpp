#include "TemperatureSensorDHT11.h"
#include "Arduino.h"  
//#include "DHT.h"

#define DHTTYPE DHT11   

TempSensorDHT11::TempSensorDHT11(uint8_t pin)
  : dht(pin, DHTTYPE) {
  dht.begin();
}

float TempSensorDHT11::getTemperature() {
  float t = dht.readTemperature();   // °C di default

  if (isnan(t)) {
    // lettura fallita: gestisci come preferisci:
    // - return NAN;
    // - oppure un valore di fallback
    return NAN;
  }

  return t;
}

float TempSensorDHT11::getHumidity() {
  float h = dht.readHumidity();

  if (isnan(h)) {
    return NAN;
  }

  return h;
}
