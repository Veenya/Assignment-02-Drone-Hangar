#include "CommunicationCenter.h"
#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

CommunicationCenter::CommunicationCenter(Hangar* pHangar): pHangar(pHangar){
}

void CommunicationCenter::init(){
  openDoorNotification = false;
  takeOffNotification = false;
  landingNotification = false;
  alarmNotification = false;
  resetAlarmsNotification = false;
}

void CommunicationCenter::notifyNewState(){
  this->hangarState = pHangar->getHangarState();
  String hangarStateStr;
  if (hangarState == HangarState::ALARM) {
    hangarStateStr = "2";
  } else if (hangarState == HangarState::PRE_ALARM) {
    hangarStateStr = "1";
  } else {   // NORMAL
    hangarStateStr = "0";
  }

  droneState = pHangar->getDroneState();
  String droneStateStr = "-1";
  if (droneState == DroneState::REST) {
      droneStateStr = "0";
  } else if (droneState == DroneState::OPERATING) {
    droneStateStr = "1";
  } else {
    droneStateStr = "2";
  }
  droneDistance = pHangar->getDistance(); // solitamente tra 0 e 0.2
  // int droneDistance = 10;
  currentTemp = pHangar->getTemperature();

  // stato del hangar, stato drone, distanza drone, temperatura
  MsgService.sendMsg(String("STATE,") + hangarStateStr + "," + droneStateStr + "," + String(droneDistance).substring(0,5) + "," +  String(currentTemp).substring(0,5));  
}

void CommunicationCenter::sync(){
  if (MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg();
    if (msg != NULL){
      String msgContent = msg->getContent();
      Logger.log("Received msg: " + msgContent);
      if (msgContent == "to" ){ // Take off
        openDoorNotification = true;
        takeOffNotification = true;
      } else if (msgContent == "la") { // Landing
        openDoorNotification = true;
        landingNotification = true;
      } else if (msgContent == "ao") { // Alarm on
        pHangar->setHangarState(HangarState::ALARM);
        this->hangarState = HangarState::ALARM;
        // alarmNotification = true;
      } else if (msgContent == "af") { // Alarm off
        pHangar->setHangarState(HangarState::NORMAL);
        this->hangarState = HangarState::NORMAL;
        // resetAlarmsNotification = true;
      }
      delete msg;
    }  
  }
}

//* OPEN DOOR
bool CommunicationCenter::checkAndResetOpenDoorRequest(){
  bool request = this->openDoorNotification;
  openDoorNotification = false;
  return request;
}

//* TAKEOFF
bool CommunicationCenter::checkAndResetTakeOffRequest(){
  bool request = this->takeOffNotification;
  takeOffNotification = false;
  return request;
}
bool CommunicationCenter::checkTakeOffRequest(){
  return this->takeOffNotification;
}

//* LANDING
bool CommunicationCenter::checkAndResetLandingRequest(){
  bool request = this->landingNotification;
  landingNotification = false;
  return request;
}

bool CommunicationCenter::checkLandingRequest(){
  return this->landingNotification;
}

//* ALLARM
bool CommunicationCenter::checkAndResetAlarmRequest() {
  bool request = this->resetAlarmsNotification;
  resetAlarmsNotification = false;
  return request;
}

bool CommunicationCenter::notifyAlarm(){
  bool request = this->alarmNotification;
  alarmNotification = false;
  return request;
}
