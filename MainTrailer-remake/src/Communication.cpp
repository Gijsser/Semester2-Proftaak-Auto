#include <Arduino.h>
#include <SoftwareSerial.h>

#include "Communication.h"
#include "Trailer.h"

SoftwareSerial Bluetooth(11, 12);

void communication_startup(){
    Bluetooth.begin(9600);
}

void communication_check_available(){
  if(Bluetooth.available() > 0 ){
    communication_read_message(Bluetooth.readStringUntil(MESSAGE_END));
  }
  else if (Serial.available() > 0){
    communication_read_message(Serial.readStringUntil(MESSAGE_END));
  }
}

void communication_read_message(String message){
  if(message[0] == MESSAGE_START){ // als start-marker
    char buffer[50]= "";
    String Parsed[2];

    parse_message(message, Parsed, 2);

    if (Parsed[0] == "TRAILER_ASSIST_ON") {
      trailer_set_state(Assist);
    }
    if (Parsed[0] == "TRAILER_ASSIST_OFF") {
      trailer_set_state(Sound);
    }
    if (Parsed[0] == "SENSOR_LEFT_STATUS"){
      trailer_set_distance(0, Parsed[1].toInt());
      sprintf(buffer, MESSAGE_START+"SENSOR_LEFT_STATUS:%d"+MESSAGE_END, Parsed[1].toInt());
      Bluetooth.write(buffer);
    }
    if (Parsed[0] == "SENSOR_MIDDLE_LEFT_STATUS"){
      trailer_set_distance(1, Parsed[1].toInt());
      sprintf(buffer, MESSAGE_START+"SENSOR_MIDDLE_LEFT_STATUS:%d"+MESSAGE_END, Parsed[1].toInt());
      Bluetooth.write(buffer);
    }
    if (Parsed[0] == "SENSOR_MIDDLE_RIGHT_STATUS"){
      trailer_set_distance(2, Parsed[1].toInt());
      sprintf(buffer, MESSAGE_START+"SENSOR_MIDDLE_RIGHT_STATUS:%d"+MESSAGE_END, Parsed[1].toInt());
      Bluetooth.write(buffer);
    }
    if (Parsed[0] == "SENSOR_RIGHT_STATUS"){
      trailer_set_distance(3, Parsed[1].toInt());
      sprintf(buffer, MESSAGE_START+"SENSOR_RIGHT_STATUS:%d"+MESSAGE_END, Parsed[1].toInt());
      Bluetooth.write(buffer);
    }
  }
}
void communication_send_Stuursensor( int distance){
  char buffer[50]= "";
  sprintf(buffer, MESSAGE_START+"Stuursensor_STATUS:%d"+MESSAGE_END, distance);
      //Bluetooth.write(buffer);
      Serial.write(buffer);
}

void parse_message(String unparsed, String *Parsed, int size){ //kijken bij watch.c
  String parsed;

  for (uint8_t i = 1; i < unparsed.length(); i++) {
    parsed = parsed + unparsed[i];
  }

  int delimiterIndex = parsed.indexOf(DELIMITER); // als mid-marker

  if (delimiterIndex > 0) {
    Parsed[0] = parsed.substring(0, delimiterIndex);
    Parsed[1] = parsed.substring(delimiterIndex + 1, (parsed.length()));
    Serial.println(Parsed[1]);
  } else {
    Parsed[0] = parsed;
  }
}
