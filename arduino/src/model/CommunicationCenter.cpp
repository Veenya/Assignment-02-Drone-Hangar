#include "CommunicationCenter.h"
#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

CommunicationCenter::CommunicationCenter(Hangar* pHangar): pHangar(pHangar){
}

void CommunicationCenter::init(){
  openDoorNotification = false;
}

void CommunicationCenter::notifyNewState(){
  HangarState hangarState = pHangar->getHangarState();
  String hangarStateStr;
  if (hangarState == HangarState::ALARM) {
    hangarStateStr = "2";
  } else if (hangarState == HangarState::PRE_ALARM) {
    hangarStateStr = "1";
  } else {   // NORMAL
    hangarStateStr = "0";
  }

  droneState = pHangar->getDroneState();
  // droneState =DroneState::OPERATING;
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
        openDoorNotification = true; // TODO decidere quale tenere
        takeOffNotification = true;
      } else if (msgContent == "la") { // Landing
        openDoorNotification = true;
        landingNotification = true;
      }
      delete msg;
    }  
  }
}

bool CommunicationCenter::checkAndResetOpenDoorRequest(){
  bool request = this->openDoorNotification;
  openDoorNotification = false;
  return request;
}

bool CommunicationCenter::checkAndResetTakeOffRequest(){
  bool request = this->takeOffNotification;
  takeOffNotification = false;
  return request;
}

bool CommunicationCenter::checkAndResetLandingRequest(){
  bool request = this->landingNotification;
  landingNotification = false;
  return request;
}

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
