#include <Arduino.h>
#include <SoftwareSerial.h>

#include "Communication.h"

SoftwareSerial Bluetooth(10, 11);

ComState ComStatus = WAITING_FOR_MESSAGE;
LastRecieved LastMessage = SERIALCOM;

String incomingMessage = "";

int communication_read_message() {
  if (Bluetooth.available() > 0) {
    int incomingByte = Bluetooth.read();
    if (ComStatus == WAITING_FOR_MESSAGE) {
      if (incomingByte == '\x05') {
        ComStatus = READING_MESSAGE;
        incomingMessage = "";
        return 0;
      }
    }
    if (ComStatus == READING_MESSAGE) {
      if (incomingByte == '\x04') {
        ComStatus = WAITING_FOR_MESSAGE;
        LastMessage = BLUETOOTHCOM;
        return 1;
      } else {
        incomingMessage += (char)incomingByte;
        return 0;
      }
    }
  }
  else if(Serial.available() > 0){
      int incomingByte = Serial.read();
      if (ComStatus == WAITING_FOR_MESSAGE) {
        if (incomingByte == '\x05') {
          ComStatus = READING_MESSAGE;
          incomingMessage = "";
          return 0;
        }
      }
      if (ComStatus == READING_MESSAGE) {
        if (incomingByte == '\x04') {
          ComStatus = WAITING_FOR_MESSAGE;
          LastMessage = SERIALCOM;
          return 1;
        } else {
          incomingMessage += (char)incomingByte;
          return 0;
        }
      }
    }
  return 0;
}

void communication_send_sensor(int sensor, int distance) {
  char buffer[50] = "";
  switch (sensor) {
  case 0:
    sprintf(buffer, "\x05\x02\x01\x01SENSOR_LEFT_STATUS:%d\x03\x04", distance);
    Bluetooth.write(buffer);
    // Serial.write(buffer);
    return;
  case 1:
    sprintf(buffer, "\x05\x02\x01\x01SENSOR_MIDDLE_LEFT_STATUS:%d\x03\x04",
            distance);
    Bluetooth.write(buffer);
    // Serial.write(buffer);
    return;
  case 2:
    sprintf(buffer, "\x05\x02\x01\x01SENSOR_MIDDLE_RIGHT_STATUS:%d\x03\x04",
            distance);
    Bluetooth.write(buffer);
    // Serial.write(buffer);
    return;
  case 3:
    sprintf(buffer, "\x05\x02\x01\x01SENSOR_RIGHT_STATUS:%d\x03\x04", distance);
    Bluetooth.write(buffer);
    // Serial.write(buffer);
    return;
  }
}

void communication_parse_message (String *Parsed,
                                 int size) { // kijken bij watch.c
  String parsed;

  for (uint8_t i = 1; i < incomingMessage.length(); i++) {
    parsed = parsed + incomingMessage[i];
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

void comminucation_bluettooth_start(){
  Bluetooth.begin(9600);
}

void communication_send_beat(){
  char buffer[50] = "";
  sprintf(buffer, MESSAGE_START + "BEAT" + MESSAGE_END);
  Bluetooth.write(buffer);
}

void communication_forward_message(){

  switch (LastMessage) {
    case SERIALCOM:
    Bluetooth.print(MESSAGE_START);
    Bluetooth.print(incomingMessage);
    Bluetooth.print(MESSAGE_END);
    break;
    case BLUETOOTHCOM:
    Serial.print(MESSAGE_START);
    Serial.print(incomingMessage);
    Serial.print(MESSAGE_END);
    break;
  }
}
