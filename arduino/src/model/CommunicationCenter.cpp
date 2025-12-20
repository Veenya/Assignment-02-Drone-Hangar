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
  String st;
  if (pHangar->getHangarState() == HangarState::ALARM) {
    st = "2";
  } else if (pHangar->getHangarState() == HangarState::PRE_ALARM) {
    st = "1";
  } else {   
    st = "0";
  }
  int droneDistance = pHangar->getDistance();
  float currentTemp = pHangar->getTemperature();

  MsgService.sendMsg(String("STATE,") + st + "," + String(droneDistance).substring(0,5) + "," +  String(currentTemp).substring(0,5));  
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
