#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "model/UserPanel.h"
#include "model/CommunicationCenter.h"
#include "tasks/CommunicationTask.h"
#include "tasks/DoorTask.h"
#include "tasks/TemperatureTask.h"
// #include "tasks/TestHWTask.h"
#include <Servo.h>

// #include "avr8-stub.h" // debug
// #include "app_api.h"   // debug

Scheduler scheduler;


HWPlatform* pHWPlatform;
UserPanel* pUserPanel;
Hangar* pHangar;
CommunicationCenter* pCommunicationCenter;


void setup() {

	// debug_init();

	MsgService.init();
	scheduler.init(100);
	Logger.log(":::::: Drone Hangar ::::::");
	pHWPlatform = new HWPlatform();
	pHWPlatform->init();

	pUserPanel = new UserPanel(pHWPlatform);
	pUserPanel->init();

	pHangar = new Hangar(pHWPlatform);
	pHangar->init();

	pCommunicationCenter = new CommunicationCenter(pHangar);
	pCommunicationCenter->init();

	Task* pCommunicationTask = new CommunicationTask(pCommunicationCenter, pHangar);
	pCommunicationTask->init(COMMUNICATION_PERIOD);

	Task* pDoorTask = new DoorTask(pCommunicationCenter, pHangar, pUserPanel);
	pDoorTask->init(DOOR_TASK);

	Task* pTemperatureTask = new TemperatureTask(pHangar, pUserPanel);
	pTemperatureTask->init(TEMPERATURE_TASK);

	scheduler.addTask(pCommunicationTask);
	scheduler.addTask(pDoorTask);
	scheduler.addTask(pTemperatureTask);
	// motor.attach(3);
}




void loop() {
	// Serial.println("Pre scheduler");
	scheduler.schedule();
	// Serial.println("Post scheduler");

	// float distance = pHangar->getDistance();
	// MsgService.sendMsg(String(distance).substring(0,5));

	// pHangar->openDoor();
	// MsgService.sendMsg("OPEN");
	// delay(1000);
	// pHangar->closeDoor();
	// MsgService.sendMsg("CLOSE");
	// delay(1000);

	// pHWPlatform->getHangarDoorMotor()->setPosition(90);     // Vai a 90°
    // delay(1000);         // Aspetta 1 secondo per completare il movimento
    // pHWPlatform->getHangarDoorMotor()->setPosition(0);      // Vai a 0°
    // delay(1000);         // Aspetta 1 secondo

}

