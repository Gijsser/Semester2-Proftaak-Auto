#include <Arduino.h>

#include "Sensors.h"
#include "Communication.h"

#define MAX_TIME (5000)

typedef enum { OFF = 0, SOUND = 1, ASSIST = 2 } TrailerState;

typedef enum {  NOK = 0, OK = 1 } CommunicationState;

TrailerState TrailerStatus = ASSIST;
CommunicationState ConStatus = NOK;
unsigned long sinceLastMessage = 0;

void setup(){
  Serial.begin(9600);
  comminucation_bluettooth_start();
}

void loop(){


  switch (ConStatus) {
    case NOK:
    if (communication_read_message() == 1){
      String Parsed[2];
      communication_parse_message(Parsed,2);
      Serial.println(Parsed[0]);
      if(Parsed[0] == "ACK"){
        Serial.println("ACK");
        ConStatus = OK;
        sinceLastMessage = millis();
      }
    }
    break;

    case OK:
    Serial.println("CON OK");
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
      communication_send_message(Parsed[1], Parsed[2].toInt(), BOTH);
      sinceLastMessage = millis();
    }
    break;
  }

  if ((millis()-sinceLastMessage) > MAX_TIME){
    communication_send_message("BEAT", BLUETOOTHCOM);
    sinceLastMessage = millis();
    Serial.println("Beat");
    ConStatus = NOK;
  }

  get_steeringwheel_position();
}
