[x] Pir (drone presence detector)
[x] sonar (drone distance detector)
[x] servomotr (hangar door)
       IL bro usa timer1 e timer2, perché su Arduino UNO l libreria Servo 
       di Arduino usa Timer1
       ma Timer1 potrebbe essere usato da altre librerie o sensori del progetto 
       (ad es. segnali PWM, librerie di distanza, ecc.)
       Quindi, quando Timer1 è occupato si usa una libreria alternativa che controlla servo tramite Timer2, lasciando libero Timer1.
[x] lcd
    non servono classi
[x] 3 leds
    pure qui aggiunge classi che non mi serviranno, faccio solo 3.
[x] button (il madlad ne fa 4!!! wtf! io solo 3 come i pdf)
    button.h
    buttonimpl.h
    buttonimpl.cpp
    (NO: button.cpp)
[x] temperature sensor
    nel suo file CASINO... ora spiego:
        - TempSensor.h -> common interface used by the rest of the code
        - TempSensorLM35.* -> implementation for LM35 sensor
        - TempSensorTMP36.* -> implementation for TMP36 sensor
        - TempSensorImpl.h -> duplicate/old header for LM35, can be removed

we are gonna need a lot of files...

---
## Led
- `Light.h`
- `Led.h`
- `Led.cpp`

## Button
- `Button.h`
- `ButtonImpl.h`
- `ButtonImpl.cpp`

## PIR
- `PresenceSensor.h`
- `Pir.h`
- `Pir.cpp`

## Proximity Sensor
- `ProximitySensor.h`
- `Sonar.h`
- `Sonar.cpp`

## Temperature Sensor
- `TempSensor.h`
- `TempSensorLM35.h`
- `TempSensorLM35.cpp` 
- `TempSensorTMP36.h`
- `TempSensorTMP36.cpp` 
- `TempSensorDHT11.h`
- `TempSensorDHT11.cpp` 

## Servo motor
- `servo_motor.h`
- `servo_motor_impl.h`
- `servo_motor_impl.cpp`
- `ServoTimer2.h`
- `ServoTimer2.cpp`