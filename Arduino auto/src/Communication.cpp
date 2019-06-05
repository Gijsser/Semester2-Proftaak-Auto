#include "Communication.h"

SoftwareSerial Bluetooth(10, 11);

ComState ComStatus = WAITING_FOR_MESSAGE;
LastRecieved LastMessage = SERIALCOM;
CommunicationState ConStatus = NOK;

void communication_Test_connection(unsigned long * sinceLastMessage){
  static unsigned long timeNoBeatAck = 0;
  unsigned long timer;
  if (ConStatus == NOK){
    timer = 1000;
  }
  else{
    timer = 1000;
  }
  if ((millis()- *sinceLastMessage) > timer){
    communication_send_message("BEAT", BLUETOOTHCOM);
    *sinceLastMessage = millis();
    //Serial.println("Beat");
    timeNoBeatAck++;
    if(timeNoBeatAck>5){
      ConStatus = NOK;
    }
  }
}

int communication_read_message(String * incommingMessage) {
  if (Bluetooth.available() > 0) {
    int incomingByte = Bluetooth.read();
    if (ComStatus == WAITING_FOR_MESSAGE) {
      if (incomingByte == MESSAGE_START) {
        ComStatus = READING_MESSAGE_BLUETOOTH;
        *incommingMessage = "";
        return 0;
      }
    }
    else if (ComStatus == READING_MESSAGE_BLUETOOTH) {
      if (incomingByte == MESSAGE_END) {
        ComStatus = WAITING_FOR_MESSAGE;
        LastMessage = BLUETOOTHCOM;
        return 1;
      } else {
        *incommingMessage += (char)incomingByte;
        return 0;
      }
    }
  }

  else if(Serial.available() > 0){
      int incomingByte = Serial.read();
      if (ComStatus == WAITING_FOR_MESSAGE) {
        if (incomingByte == MESSAGE_START) {
          ComStatus = READING_MESSAGE_SERIAL;
          * incommingMessage = "";
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
          * incommingMessage += (char)incomingByte;
          return 0;
        }
      }
    }
  return 0;
}

void communication_parse_message (String *Parsed, String * incomingMessage) {
  String toParse = * incomingMessage;

  int delimiterIndex = toParse.indexOf(DELIMITER); // als mid-marker

  if (delimiterIndex > 0) {
    Parsed[0] = toParse.substring(0, delimiterIndex);
    Parsed[1] = toParse.substring(delimiterIndex + 1, (toParse.length()));
    //Serial.println(Parsed[1]);
  } else {
    Parsed[0] = toParse;
  }
}

void comminucation_bluettooth_start(){
  Bluetooth.begin(9600);
}

void communication_send_message(String message, int value, LastRecieved sendTo){
  char buffer[50] = "";
  sprintf(buffer, "%c%s:%i%c", MESSAGE_START,message.c_str(), value, MESSAGE_END);
  switch (sendTo) {
    case BLUETOOTHCOM:
    Bluetooth.write(buffer);
    break;
    case SERIALCOM:
    Serial.write(buffer);
    break;
    case BOTH:
    Serial.write(buffer);
    Bluetooth.write(buffer);
    break;
  }
}
