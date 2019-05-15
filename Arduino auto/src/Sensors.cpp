#include <Arduino.h>

#include "Sensors.h"
#include "Communication.h"

int lastPosition = 0;

void get_steeringwheel_position(){
  int newPosition = analogRead(STEERING_WHEEL);
  if (newPosition != lastPosition){
    communication_send_message("WHEEL_POS", newPosition);
    lastPosition = newPosition;
  }
}
