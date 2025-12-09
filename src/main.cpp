#include <Arduino.h>
#include "kernel/Scheduler.h"
#include "model/HWPlatform.h"
#include "model/Hangar.h"

// --- Task di debug che apre/chiude la porta ogni 3 secondi ---
class DoorDebugTask : public Task {
public:
  DoorDebugTask(Hangar* pHangar) : pHangar(pHangar) {}

  void init(int period) override {
    Task::init(period);    // period in ms (usato dallo Scheduler)
    nextToggleTime = millis() + 3000;
    doorOpen = false;
  }

  void tick() override {
    unsigned long now = millis();

    if (now >= nextToggleTime) {
      doorOpen = !doorOpen;
      if (doorOpen) {
        Serial.println("[DBG] openDoor()");
        pHangar->openDoor();
      } else {
        Serial.println("[DBG] closeDoor()");
        pHangar->closeDoor();
      }
      nextToggleTime = now + 3000;   // prossimo toggle tra 3 secondi
    }
  }

private:
  Hangar* pHangar;
  bool doorOpen;
  unsigned long nextToggleTime;
};

// --- oggetti globali ---
Scheduler sched;
HWPlatform hw;
Hangar hangar(&hw);
DoorDebugTask doorTask(&hangar);

void setup() {
  Serial.begin(9600);
  delay(2000);

  Serial.println("=== DOOR DEBUG TASK ===");

  hw.init();        // inizializza servo, LCD, LED, ecc.
  hangar.init();    // chiude porta logica/fisica

  sched.init(100);          // base period 100 ms
  doorTask.init(100);       // tick ogni 100 ms
  sched.addTask(&doorTask);
}

void loop() {
  sched.schedule();
}
