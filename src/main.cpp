/* #############################################
   #############################################
   ########## OOP LEDS AND BUTTON TEST #########
   #############################################
   #############################################
*/
/*
#include <Arduino.h>

#include "config.h"
#include "devices/Led.h"
#include "devices/ButtonImpl.h"

// NOTE: config.h defines L3_pin (lowercase "pin")
// so we use that exact name here.

// LED objects
Led led1(L1_PIN);   // green
Led led2(L2_PIN);   // green
Led led3(L3_PIN);   // red

// Button object
ButtonImpl button(BUTTON_PIN);

void setup() {
  Serial.begin(9600);
  // pinMode is already called in Led and ButtonImpl constructors
}

void loop() {
  if (button.isPressed()) {
    led1.switchOn();
    led2.switchOn();
    led3.switchOn();
    Serial.println("ON");
  } else {
    led1.switchOff();
    led2.switchOff();
    led3.switchOff();
    Serial.println("OFF");
  }
}
*/


/* #############################################
   #############################################
   ######## NON OOP LEDS AND BUTTON TEST #######
   #############################################
   #############################################
*/


/*LEDs and Button TEST*/
/*
#include <Arduino.h>

#define BUTTON 10
#define L1 13 // green
#define L2 12 // green
#define L3 11 // red


void setup()
{
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int buttonState = digitalRead(BUTTON);
  if (buttonState == HIGH){
    digitalWrite(L1, HIGH);
    digitalWrite(L2, HIGH);
    digitalWrite(L3, HIGH);
    Serial.println("ON");
  } else {
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
    digitalWrite(L3, LOW);
    Serial.println("OFF");
  }
}
*/

/* #############################################
   #############################################
   ################# LCD TEST ##################
   #############################################
   #############################################
*/

/*LCD screen TEST*/
/*
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Creates LCD object for I2C display
void setup() {
  Serial.begin(9600);
  // Initializes screen
  lcd.init();
  lcd.backlight();
}

void loop() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
}
*/

/* #############################################
   #############################################
   ############### SERVO TEST ##################
   #############################################
   #############################################
*/

/*
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
*/

/* #############################################
   #############################################
   ######### PROXIMITY SENSOR TEST #############
   #############################################
   #############################################
*/

/*
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
*/

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


/* #############################################
   #############################################
   ############ TEMPERATUTE TEST ###############
   #############################################
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
