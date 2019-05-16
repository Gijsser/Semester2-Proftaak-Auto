#include <Arduino.h>

#include "Sensors.h"
#include "Communication.h"

#define MAX_TIME (5000)

typedef enum { OFF = 0, SOUND = 1, ASSIST = 2 } TrailerState;



TrailerState TrailerStatus = ASSIST;

void setup(){
  Serial.begin(9600);
  comminucation_bluettooth_start();
}

void loop(){

  communication_Test_connection();
  switch (get_connect_status()) {
    case 0:
    if (communication_read_message() == 1){
      String Parsed[2];
      communication_parse_message(Parsed,2);
      Serial.println(Parsed[0]);
      if(Parsed[0] == "ACK"){
        Serial.println("ACK");
        ConStatus = OK;
        timeNoBeatAck = 0;
        sinceLastMessage = millis();
      }
    }
    break;

    case 1:
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
      timeNoBeatAck = 0;
    }
    break;
  }



  get_steeringwheel_position();
}
