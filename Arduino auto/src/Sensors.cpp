#include <Arduino.h>

#include "Sensors.h"
#include "Communication.h"

int lastPosition = 0;
unsigned long timerstreeringwheel=0;

void get_steeringwheel_position(){
  int newPosition = analogRead(STEERING_WHEEL);
  newPosition = map(newPosition, 0, 1023, 0, 180);
  if ((newPosition != lastPosition)&&(millis()-timerstreeringwheel>500)&&ConStatus ==OK){
    communication_send_message("WHEEL_POS", newPosition, BOTH);
    lastPosition = newPosition;
    timerstreeringwheel = millis();
  }
}
