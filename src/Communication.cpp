#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>
#include "Communication.h"

SoftwareSerial Bluetooth(10, 11);

ComState ComStatus = WAITING_FOR_MESSAGE;
LastRecieved LastMessage = SERIALCOM;

String incomingMessage = "";

int communication_read_message() {
  if (Bluetooth.available() > 0) {
    int incomingByte = Bluetooth.read();
    if (ComStatus == WAITING_FOR_MESSAGE) {
      if (incomingByte == MESSAGE_START) {
        ComStatus = READING_MESSAGE_BLUETOOTH;
        incomingMessage = "";
        return 0;
      }
    }
    else if (ComStatus == READING_MESSAGE_BLUETOOTH) {
      if (incomingByte == MESSAGE_END) {
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
        if (incomingByte == MESSAGE_START) {
          ComStatus = READING_MESSAGE_SERIAL;
          incomingMessage = "";
          return 0;
        }
      }
      else if (ComStatus == READING_MESSAGE_SERIAL) {
        if (incomingByte == MESSAGE_END) {
          ComStatus = WAITING_FOR_MESSAGE;
          LastMessage = SERIALCOM;
          return 1;
        }
        else {
          incomingMessage += (char)incomingByte;
          return 0;
        }
      }
    }
  return 0;
}

void communication_parse_message (String *Parsed, int size) { 
  String parsed;

  for (uint8_t i = 0; i < incomingMessage.length(); i++) {
    parsed = parsed + incomingMessage[i];
  }

  int delimiterIndex = parsed.indexOf(DELIMITER); // als mid-marker

  if (delimiterIndex > 0) {
    Parsed[0] = parsed.substring(0, delimiterIndex);
    Parsed[1] = parsed.substring(delimiterIndex + 1, (parsed.length()));
    //Serial.println(Parsed[1]);
  } else {
    Parsed[0] = parsed;
  }
}

void comminucation_bluettooth_start(){
  Bluetooth.begin(9600);
}

void communication_send_message(String message, int value){
  char buffer[50] = "";
  switch (LastMessage) {
    case SERIALCOM:
    sprintf(buffer, "%c%s:%i%c", MESSAGE_START,message.c_str(), value, MESSAGE_END);
    Bluetooth.write(buffer);
    break;
    case BLUETOOTHCOM:
    sprintf(buffer, "%c%s:%i%c", MESSAGE_START,message.c_str(), value, MESSAGE_END);
    Serial.write(buffer);
    break;
  }
}