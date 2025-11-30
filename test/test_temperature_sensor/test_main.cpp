/* #############################################
   #############################################
   ############ TEMPERATUTE TEST ###############
   ################ DHT 11 #####################
   #############################################
*/
/*
#include <Arduino.h>
#define TEMP A0

void setup(){
  Serial.begin(9600);
  pinMode(TEMP, INPUT);
}

void loop(){
  int tread = analogRead(TEMP);                // valore 0-1023
  float voltage = tread * (5.0 / 1023.0);      // in volt
  float tempC = (voltage - 0.5) * 100.0;       // formula TMP36

  Serial.print("raw: ");
  Serial.print(tread);
  Serial.print(" | V: ");
  Serial.print(voltage, 3);   // 3 decimali
  Serial.print(" V | Temp: ");
  Serial.print(tempC, 2);     // 2 decimali

  // opzione per stampare il simbolo grado (se vuoi provarlo)
  // Serial.print(" "); Serial.write(176); Serial.println("C");

  // oppure più sicuro (compatibilità): niente simbolo grado
  Serial.println(" C");
  delay(500);
}

*/
#include <Arduino.h>
#include "DHT.h"

#define TEMP_PIN A0
#define DHTTYPE DHT11

DHT dht(TEMP_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Error: no data");
  } else {
    Serial.print("T: ");
    Serial.print(t);
    Serial.print(" °C   H: ");
    Serial.print(h);
    Serial.println(" %");
  }

  delay(2000);
}