[x] Pir (drone presence detector)
[x] sonar (drone distance detector)
[] servomotr (hangar door)
[x] lcd
    non servono classi
[x] 3 leds
    pure qui aggiunge classi che non mi serviranno, faccio solo 3.
[x] button (il madlad ne fa 4!!! wtf! io solo 3 come i pdf)
    button.h
    buttonimpl.h
    buttonimpl.cpp
    (NO: button.cpp)
[] temperature sensor
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
- TempSensor.h
- TempSensorLM35.h
- TempSensorLM35.cpp 
- TempSensorTMP36.h
- TempSensorTMP36.cpp 