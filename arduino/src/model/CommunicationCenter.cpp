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
      if (msgContent == "to" || msgContent == "la"){ // Take off or landing
        openDoorNotification = true;
      } 
      delete msg;
    }  
  }
}

bool CommunicationCenter::checkAndResetOpenDoorRequest(){
  bool com = this->openDoorNotification;
  openDoorNotification = false;
  return com;
}

