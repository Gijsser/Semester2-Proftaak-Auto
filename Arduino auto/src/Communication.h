#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#define MESSAGE_START ('#')
#define MESSAGE_END (';')
#define DELIMITER (':')

typedef enum {
  WAITING_FOR_MESSAGE = 0,
  READING_MESSAGE_BLUETOOTH = 1,
  READING_MESSAGE_SERIAL = 2
} ComState;
typedef enum {
  BLUETOOTHCOM = 0,
  SERIALCOM = 1,
  BOTH = 2
} LastRecieved;
typedef enum {
  NOK = 0,
  OK = 1
} CommunicationState;
extern CommunicationState ConStatus;
extern unsigned long sinceLastMessage;
extern int timeNoBeatAck;

int get_connect_status();
int communication_read_message();
void communication_parse_message(String *Parsed, int size );
void comminucation_bluettooth_start();
void communication_send_message(String message, int value = 0, LastRecieved sendTo = BOTH);
void communication_Test_connection();

#endif
