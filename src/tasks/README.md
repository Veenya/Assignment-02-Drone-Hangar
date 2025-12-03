```text
src/
  tasks/
    DroneRemoteTask.h / .cpp      ← comandi TAKEOFF/LAND via seriale
    DroneOperationTask.h / .cpp   ← porta, sonar, PIR, LED L2, LCD
    TemperatureTask.h / .cpp      ← gestione allarmi temperatura
    TestHWTask.h / .cpp           ← test iniziale optional

```


```text
src/
  tasks/
    TemperatureTask.h / .cpp
    DroneRemoteTask.h / .cpp   (+ UserPanelTask)
    TestHWTask.h / .cpp                 // Riusare 
    DroneOperationTask.h / .cpp     
```

---

## `TestHWTask` 

`TestHWTask` serve per verificare:
- sonar
- pir
- servo porta
- LCD
- temperatura
- LED
- bottone reset

## `DroneOperationTask` 
ex `WasteDisposalTask` faceva:
- aprire la porta
- aspettava che il rifiuto venisse buttato
- accendeva i led
- ...

## `TemperatureTask`
ex `ContainerHealthCheckTask` faceva:
- lettura temperatura
- gestire pre-allarme
- ...

## `DroneRemoteTask` (+ `UserPanel`)
ex `OperatorManTask` faceva:
- gestione bottoni operatore
- gestione messaggi PC
- ...