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


Scheduler scheduler;


HWPlatform* pHWPlatform;
UserPanel* pUserPanel;
Hangar* pHangar;
CommunicationCenter* pCommunicationCenter;


void setup() {
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
}


void loop() {
	scheduler.schedule();
	// float distance = pHangar->getDistance();
	// MsgService.sendMsg(String(distance).substring(0,5));
}
