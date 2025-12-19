#include "Dashboard.h"
#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

Dashboard::Dashboard(Hangar* pHangar): pHangar(pHangar){
}

void Dashboard::init(){
  maintenanceDoneNotified = false;
  dischargeCmdRequested = false;
}

void Dashboard::notifyNewState(){
  String st;
  if (pHangar->getHangarState() == HangarState::NORMAL){ // TODO Rivedere segnaposto
    st = "2";
  } else if (pHangar->getHangarState() == HangarState::NORMAL){ // TODO Rivedere segnaposto
    st = "1";
  } else {   
    st = "0";
  }
  int wasteLevel = pHangar->getDistance(); // TODO Rivedere segnaposto
  float currentTemp = pHangar->getDistance(); // TODO Rivedere segnaposto
  // TODO Rivedere messaggio mandato
  MsgService.sendMsg(String("cw:st:") + st + ":" + String(wasteLevel).substring(0,5) + ":" +  String(currentTemp).substring(0,5));  
}

void Dashboard::sync(){
  if (MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg();
    if (msg != NULL){
      Logger.log("Received msg: " + msg->getContent());
      if (msg->getContent() == "ok"){ // TODO Rivedere segnaposto
        maintenanceDoneNotified = true;
      } else if (msg->getContent() == "di"){ // TODO Rivedere segnaposto
        dischargeCmdRequested = true;
      }
      delete msg;
    }  
  }
}

bool Dashboard::checkAndResetDischargeRequest(){
  bool com = this->dischargeCmdRequested;
  dischargeCmdRequested = false;
  return com;
}

bool Dashboard::checkAndResetMaintenanceDone(){
  bool com = this->maintenanceDoneNotified;
  maintenanceDoneNotified = false;
  return com;
}
