#include <Arduino.h>
#include "devices/Sonar.h"
#include "config.h" 


// timeout in microsecondi (es. 30 ms)
const long MAX_TIME = 30000;

Sonar sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN, MAX_TIME);

void setup() {
  Serial.begin(9600);
  // opzionale: imposta temperatura diversa da 20°
  sonar.setTemperature(20);  
}

void loop() {
  float d = sonar.getDistance();
  if (d == NO_OBJ_DETECTED) {
    Serial.println("Nessun oggetto rilevato");
  } else {
    Serial.print("Distanza: ");
    Serial.print(d);
    Serial.println(" m");
  }
  delay(200);
}
