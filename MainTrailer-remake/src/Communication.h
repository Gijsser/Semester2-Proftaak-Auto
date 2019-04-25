#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#define MESSAGE_START ('\x05\x02\x01\x01')
#define MESSAGE_END ('\x03\x04')
#define DELIMITER (':')
void communication_startup();
void communication_check_available();
void communication_read_message(String message);
void communication_send_Stuursensor( int distance);
void parse_message(String unparsed, String *Parsed, int size );

#endif
