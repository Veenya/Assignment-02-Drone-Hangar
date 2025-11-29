# A cosa serve la cartella `kernel`?

NON è un kernel reale, è solo un namespace organizzativo del progetto.

Contiene tutto ciò che riguarda la “logica di sistema” del tuo progetto, cioè:
## 1. Task

File tipo `Task.h`, `Task.cpp`
Definiscono una classe Task o simile, che rappresenta un’attività da eseguire periodicament (es. leggere un sensore, controllare un attuatore…).

## 2. Scheduler

File tipo `Scheduler.h`, `Scheduler.cpp`
Uno scheduler semplice che gestisce più task, decidendo quale eseguire e quando.

È tipicamente un cooperative scheduler, molto comune in Arduino:
- chiama periodicamente `update()`
- controlla se i vari task devono essere eseguiti
- evita che tu debba usare mille delay

## 1. Messaging System

File tipo MsgService.h, MsgService.cpp
Gestiscono messaggi tra task o la comunicazione con seriale, sensori ecc.
Spesso hanno una coda FIFO interna.

## 1. Logging
`Logger.h`, `Logger.cpp`
Funzioni utili per stampare messaggi di debug:

```cpp
Logger.log("Sensor value: " + String(val));
```

---

# Come si usa nel progetto?

Tipicamente, nel tuo `main.cpp` avrai:

```cpp
#include "kernel/Scheduler.h"
#include "kernel/Task.h"

Scheduler sched;

void setup() {
    sched.init(100); // tick di 100 ms
    sched.addTask(new SomeTask());
}

void loop() {
    sched.schedule();
}
```
