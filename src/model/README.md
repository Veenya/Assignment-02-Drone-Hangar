```
src/
  model/
    HWPlatform.h / HWPlatform.cpp
    Hangar.h / Hangar.cpp
    Dashboard.h / Dashboard.cpp
    UserPanel.h / UserPanel.cpp
    States.h
```

---

La cartella **`model/`** deve rappresentare il *mondo logico* dell’hangar, NON l’hardware grezzo.

```text
src/
  model/
    HWPlatform.h / .cpp
    Hangar.h / .cpp
    HangarDoor.h / .cpp
    Dashboard.h / .cpp
    UserPanel.h / .cpp
    States.h      (solo enum/stati, opzionale)
```

---

### 1️⃣ `HWPlatform` – ponte verso i `devices/`

Contiene **tutti gli oggetti device** e usa i pin di `config.h`.

Esempio:

```cpp
class HWPlatform {
public:
  HWPlatform();

  Sonar*        getDDD();      // Drone Distance Detector
  PresenceSensor* getDPD();    // PIR
  ServoMotor*   getHangarServo();
  TempSensor*   getTempSensor();
  Lcd*          getLcd();
  Led*          getL1();
  Led*          getL2();
  Led*          getL3();
  Button*       getResetButton();

private:
  Sonar ddd;
  PresenceSensor dpd;
  ServoMotor hangarServo;
  TempSensorDHT11 temp;
  Lcd lcd;
  Led l1, l2, l3;
  Button resetBtn;
};
```

Tutti gli altri model usano `HWPlatform` per accedere all’hardware.

---

### 2️⃣ `States.h` – stati logici

Per non impazzire con le FSM:

```cpp
enum class DroneState { REST, TAKING_OFF, OPERATING, LANDING };
enum class HangarState { NORMAL, PRE_ALARM, ALARM };
```

Questi enum li userai in `Hangar`.

---

### 3️⃣ `Hangar` – cervello del sistema

È il **modello logico dell’hangar**: sa se il drone è dentro/fuori, se la porta è aperta, se c’è allarme, ecc.

Usa i sensori di `HWPlatform` (DDD, DPD, TempSensor) ma NON parla direttamente con pin.

Esempio di interfaccia:

```cpp
class Hangar {
public:
  Hangar(HWPlatform* hw);

  void update();        // legge sensori, aggiorna stati (distanza, temp, ecc.)

  // Stato drone
  void setDroneState(DroneState s);
  DroneState getDroneState() const;
  bool isDroneInside() const;

  // Porta
  void openDoor();
  void closeDoor();
  bool isDoorOpen() const;

  // Temperatura / allarmi
  void checkTemperature();     // usa TempSensor, gestisce PRE/ALARM con T3/T4
  HangarState getHangarState() const;
  bool isOperationAllowed() const; // blocca nuovi takeoff/landing in pre-alarm/alarm

  // parametri D1, T1, Temp1, Temp2, ecc. li tieni come membri o li passi al costruttore
private:
  HWPlatform* hw;
  DroneState droneState;
  HangarState hangarState;

  bool droneInside;
  bool doorOpen;

  // timer logici per T1,T3,T4 (contatori di ms)
  unsigned long tTakeoffOk;
  unsigned long tPreAlarm;
  unsigned long tAlarm;
};
```

Le **FSM dei task** (takeoff/landing/health-check) useranno questi metodi.

---

### 4️⃣ `HangarDoor` – wrapper del servo (facoltativo)

Se vuoi separare un po’:

```cpp
class HangarDoor {
public:
  HangarDoor(ServoMotor* servo);
  void open();
  void close();
  bool isOpen() const;
private:
  ServoMotor* servo;
  bool openFlag;
};
```

Puoi anche inglobarlo dentro `Hangar` se non vuoi un file in più.

---

### 5️⃣ `Dashboard` – LCD + LED

Si occupa di **mostrare** lo stato, zero logica “di decisione”.

```cpp
class Dashboard {
public:
  Dashboard(HWPlatform* hw);

  void showDroneInside();
  void showDroneOut();
  void showTakeOff();
  void showLanding();
  void showPreAlarm();
  void showAlarm();

  void blinkL2();     // usato dai task con periodo 0.5 s
  void setL2(bool on);
  void setL3(bool on);

private:
  HWPlatform* hw;
  bool l2State;
};
```

I task chiamano `Dashboard` per aggiornare LCD e LED.

---

### 6️⃣ `UserPanel` – bottone + (eventuale) DPD lato utente

Incapsula input locali (bottone reset, eventuali altri tasti).

```cpp
class UserPanel {
public:
  UserPanel(HWPlatform* hw);

  bool isResetPressed();
  // se ti serve, altri metodi: es. isOperatorButtonPressed()

private:
  HWPlatform* hw;
};
```

I task di gestione atterraggio/errore usano `UserPanel` per sapere se il reset è stato premuto.

---

### Collegamento ai `tasks/`

Nei tuoi `*Task.cpp` avrai qualcosa tipo:

* `WasteDisposalTask` → qui sarà `TakeOffTask` / `LandingTask` o un unico `DroneManTask`:

  * usa `Hangar` per comandare porta e controllare distanza
  * usa `Dashboard` per messaggi e LED
  * usa `MsgService` per parlare con la DRU

* `ContainerHealthCheckTask` → diventa `TemperatureHealthTask`:

  * chiama `hangar.checkTemperature();`
  * in base a `getHangarState()` chiama `Dashboard.showPreAlarm/Alarm()`
  * blocca sblocco operazioni via `Hangar::isOperationAllowed()`


