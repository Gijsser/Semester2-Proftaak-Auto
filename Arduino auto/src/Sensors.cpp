#include <Arduino.h>

#include "Sensors.h"
#include "Communication.h"

void get_steeringwheel_position(int  * lastPosition){
  static unsigned long timer = 0;
  int newPosition = analogRead(STEERING_WHEEL);
  newPosition = map(newPosition, 0, 1023, 0, 180);
  if ((newPosition != *lastPosition) && (millis() - timer > 500) && ConStatus == OK){
    communication_send_message("WHEEL_POS", newPosition, BOTH);
    *lastPosition = newPosition;
    timer = millis();
  }
}
