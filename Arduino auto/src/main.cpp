#include <Arduino.h>

#include "Sensors.h"
#include "Communication.h"

#define MAX_TIME (5000)

typedef enum { OFF = 0, SOUND = 1, ASSIST = 2 } TrailerState;

TrailerState TrailerStatus = SOUND;
bool waitingForAck = false;

void setup(){
  Serial.begin(9600);
  comminucation_bluettooth_start();
}

void loop(){
  static String incommingMessage = "";
  static unsigned long timeSinceLastMessage = 0;
  static int lastSteeringWheelPos = 0;

  if(waitingForAck == false){
    if(millis() - timeSinceLastMessage > 2000){
      communication_send_message("BEAT");
      waitingForAck = true;
    }
}

  switch (ConStatus) {
    case 0:
      if (communication_read_message(&incommingMessage)){
        String Parsed[2];
        communication_parse_message(Parsed, &incommingMessage);
        Serial.println(Parsed[0]);
        if(Parsed[0] == "ACK"){
          Serial.println("ACK");
          waitingForAck = false;
          ConStatus = OK;
        }
      }
    break;

    case 1:
      //Serial.println("CON OK");
      if(communication_read_message(&incommingMessage)){
        String Parsed [2];
        communication_parse_message(Parsed, &incommingMessage);
        if (Parsed[0] == "TRL_OFF"){
          TrailerStatus = OFF;
          communication_send_message(Parsed[0], Parsed[1].toInt(), BLUETOOTHCOM);
        }
        else if (Parsed[0] == "TRL_SOUND"){
          TrailerStatus = SOUND;
          communication_send_message(Parsed[0], Parsed[1].toInt(), BLUETOOTHCOM);
        }
        else if (Parsed[0] == "TLR_ASSIST"){
          TrailerStatus = ASSIST;
          communication_send_message(Parsed[0], Parsed[1].toInt(), BLUETOOTHCOM);
        }
        else if (
          Parsed[0] == "TRAILER_CONECTION"||
          Parsed[0] == "SENSOR_LEFT_STATUS"||
          Parsed[0] == "SENSOR_MIDDLE_LEFT_STATUS"||
          Parsed[0] == "SENSOR_MIDDLE_RIGHT_STATUS"||
          Parsed[0] == "SENSOR_RIGHT_STATUS")
          {
          communication_send_message(Parsed[0], Parsed[1].toInt(), SERIALCOM);
          communication_send_message("Ack",0,BLUETOOTHCOM);
        }
      }
    break;
  }
  if(TrailerStatus == ASSIST){
      get_steeringwheel_position(&lastSteeringWheelPos);
  }
  //delay(100);
}
