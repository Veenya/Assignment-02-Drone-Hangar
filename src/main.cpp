#include <Arduino.h>
#include "kernel/Scheduler.h"
#include "kernel/MsgService.h"

#include "model/HWPlatform.h"
#include "model/Hangar.h"
#include "model/UserPanel.h"

#include "tasks/DroneOperationTask.h"

// --- oggetti globali ---
Scheduler sched;
HWPlatform hw;
Hangar hangar(&hw);
UserPanel userPanel(&hw);

// Task: solo operazione porta
DroneOperationTask droneOpTask(&hangar, &userPanel);

void setup() {
  // Inizializza il servizio di messaggi (e quindi la Serial)
  MsgService.init();          // dentro fa Serial.begin(115200)

  hw.init();                  // crea sensori/attuatori, LCD "DRONE INSIDE", L1 ON
  hangar.init();              // stato logico iniziale: porta chiusa, drone dentro
  userPanel.init();           // azzera stato bottone

  // Inizializza scheduler
  sched.init(100);            // base period = 100 ms

  // Inizializza il task (periodico ogni 100 ms)
  droneOpTask.init(100);
  sched.addTask(&droneOpTask);

  // messaggino di avvio
  Serial.println("=== TEST DroneOperationTask (porta + bottone) ===");
  Serial.println("Premi il bottone per far aprire/chiudere la porta.");
}

void loop() {
  sched.schedule();
}
