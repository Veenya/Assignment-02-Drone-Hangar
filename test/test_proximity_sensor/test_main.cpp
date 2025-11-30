/* #############################################
   #############################################
   ######### PROXIMITY SENSOR TEST #############
   #############################################
   #############################################
*/


#include <Arduino.h>
const int trigPin = 8; // digital pin 8
const int echoPin = 7; // digital pin 7

// Suposing a temperature of 20° 
const int temperature = 20;
const float vs = 331.5 + 0.6*temperature; // speed of sound in m/s

void setup(){
  Serial.begin(9600); // start serial communcation at 9600 baud
  
  // we send a trigger pulse to trigPin
  pinMode(trigPin, OUTPUT); // connected to TRIG on sensor
  // we read the echo pulse on echoPin
  pinMode(echoPin, INPUT);  // connected ot ECHO on sensor
}

float getDistance(){
  // Sending impulse
  // This pulse tells the sensor to emit an ultrasonic "ping" 
  digitalWrite(trigPin, LOW);  // set low briefly to make
  delayMicroseconds(3);        // sure it starts at 0
  digitalWrite(trigPin, HIGH); // set high for 5 s
  delayMicroseconds(5);       
  digitalWrite(trigPin, LOW);  // set low again 
  float tUS = pulseIn(echoPin, HIGH);
  
  float t = tUS / 1000.0 / 1000.0 / 2; // round-trip time of the sound
  float d = t*vs; // distance in meters
  return d;
}

void loop() {
  float d = getDistance();
  Serial.println(d);
  delay(200);
}
