/* #############################################
   #############################################
   ############### SERVO TEST ##################
   #############################################
   #############################################
*/


#include <Arduino.h>
int servoPin = 2; // digital pin 2

void pulseServo(int servoPin, int pulseLen){
  digitalWrite(servoPin, HIGH); // set pin high
  delayMicroseconds(pulseLen);  // for pulseLen ms
  digitalWrite(servoPin, LOW);  // set pin low
  delay(15);                    // wait 15 micos
}

int c; // counter
enum { MINUS_90, PLUS_90 } state;

void setup(){
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
  c = 0;
}

void loop(){
  c++;
  switch(state){    // switch(state) chooses what to do 
    case MINUS_90:  // depending on the current position
    pulseServo(servoPin, 250); // set -90
    if(c > 100){      // after each pulse check this
      Serial.println("--> +90"); // else change direction
      state = PLUS_90;          // and change state
      c = 0;
    }
    break;
   case PLUS_90:
    pulseServo(servoPin, 2250);
    if (c > 100){
      Serial.println("--> -90");
      state = MINUS_90;
      c = 0;
    }
  }
}
