/* #############################################
   #############################################
   ################ PIR TEST ###################
   #############################################
   #############################################
*/


#include <Arduino.h>
#define PIR 9

void setup(){
  Serial.begin(9600);
  pinMode(PIR, INPUT);
}

void loop(){
  int val = digitalRead(PIR);
  Serial.println(val);
  if(val == HIGH){
    Serial.println("HIGH");
  } else {
    Serial.println("LOW");
  }
}
