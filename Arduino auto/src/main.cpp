#include <Arduino.h>

#include "Sensors.h"
#include "Communication.h"

#define MAX_TIME (5000)


typedef enum {
  OFF = 0,
  SOUND = 1,
  ASSIST = 2
} TrailerState;

typedef enum
{
  NOK = 0,
  OK = 1
} CommunicationState;

TrailerState TrailerStatus = OFF;
CommunicationState ConStatus = NOK;

void setup(){
  Serial.begin(9600);
  comminucation_bluettooth_start();
}

void loop(){
  static unsigned long sinceLastMessage = millis();
  if (ConStatus == NOK){
    communication_send_beat();
    if (communication_read_message() == 1){
      String Parsed[2];
      communication_parse_message(Parsed,2);
      if(Parsed[0] == "ACK"){
        ConStatus = OK;
        sinceLastMessage = millis();
      }
    }
    delay(200);
  }
  if (ConStatus == OK){
    if(communication_read_message() == 1){
      String Parsed [2];
      communication_parse_message(Parsed,2);
      if (Parsed[0] == "TRL_OFF"){
        TrailerStatus = OFF;
      }
      if (Parsed[0] == "TRL_SOUND"){
        TrailerStatus = SOUND;
      }
      if (Parsed[0] == "TLR_ASSIST"){
        TrailerStatus = ASSIST;
      }
      communication_forward_message();
      sinceLastMessage = millis();
    }
  }
  if (sinceLastMessage > MAX_TIME){
    ConStatus = NOK;
  }
}
