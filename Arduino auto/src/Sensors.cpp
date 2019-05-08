#include <Arduino.h>

#include "Sensors.h"
#include "Communication.h"



int get_steeringwheel_position(){
  return analogRead(STEERING_WHEEL);
}
